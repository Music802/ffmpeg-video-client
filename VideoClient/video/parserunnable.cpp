#include "ffmpegdecoder.h"
#include "makeguard.h"

void FFmpegDecoder::parseRunnable()
{
	CHANNEL_LOG(ffmpeg_threads) << "Parse thread started";
	AVPacket packet;
	enum { UNSET, SET, REPORTED } eof = UNSET;

	// detect real framesize
	if (true == m_bIsFile)
		fixDuration();

	startVideoThread();

	for (;;)
	{
		if (boost::this_thread::interruption_requested())
		{
			return;
		}

		int64_t seekDuration = m_seekDuration.exchange(AV_NOPTS_VALUE);
		if (seekDuration != AV_NOPTS_VALUE)
		{
			resetDecoding(seekDuration, false);
		}
		seekDuration = m_videoResetDuration.exchange(AV_NOPTS_VALUE);
		if (seekDuration != AV_NOPTS_VALUE)
		{
			if (!resetDecoding(seekDuration, true))
				return;
		}

		const int readStatus = av_read_frame(m_formatContext, &packet);
		if (readStatus >= 0)
		{
			dispatchPacket(packet);

			eof = UNSET;
		}
		else
		{
			using namespace boost;
			if (eof == SET)
			{
				if (m_videoPacketsQueue.empty()
					&& (lock_guard<mutex>(m_videoFramesMutex), !m_videoFramesQueue.canPop()))
				{
					//m_decoderListener->onEndOfStream();
					eof = REPORTED;
				}
			}
			if (eof == UNSET && readStatus == AVERROR_EOF)
				eof = SET;

			this_thread::sleep_for(chrono::milliseconds(10));
		}

		// Continue packet reading
		if (eof == REPORTED && m_bLoopEnable)
			seekByPercent(0);
	}

	CHANNEL_LOG(ffmpeg_threads) << "Decoding ended";
}

void FFmpegDecoder::dispatchPacket(AVPacket& packet)
{
    auto guard = MakeGuard(&packet, av_packet_unref);

    auto seekLambda = [this]
    {
        return m_seekDuration != AV_NOPTS_VALUE || m_videoResetDuration != AV_NOPTS_VALUE;
    };

    if (seekLambda())
    {
        return; // guard frees packet
    }

    if (packet.stream_index == m_videoStreamNumber)
    { 
        if (!m_videoPacketsQueue.push(packet, seekLambda))
        {
            return; // guard frees packet
        }
    }
    else
    {
		av_packet_unref(&packet);
        return; // guard frees packet
    }

    guard.release();
}

void FFmpegDecoder::startVideoThread()
{
    if (m_videoStreamNumber >= 0)
    {
        m_mainVideoThread.reset(new boost::thread(&FFmpegDecoder::videoParseRunnable, this));
    }
}

bool FFmpegDecoder::resetDecoding(int64_t seekDuration, bool resetVideo)
{
    CHANNEL_LOG(ffmpeg_seek) << __FUNCTION__ << " resetVideo=" << resetVideo;

    const bool hasVideo = m_mainVideoThread != nullptr;

	if (hasVideo)
	{
		if (avformat_seek_file(m_formatContext,
			m_videoStreamNumber,
			0, seekDuration, seekDuration, AVSEEK_FLAG_FRAME) < 0
			&& (seekDuration >= 0 || avformat_seek_file(m_formatContext,
			m_videoStreamNumber,
				0, 0, 0, AVSEEK_FLAG_FRAME) < 0))
		{
			CHANNEL_LOG(ffmpeg_seek) << "Seek failed";
			return false;
		}
	}

    if (hasVideo)
    {
        m_mainVideoThread->interrupt();
    }

    if (hasVideo)
    {
        m_mainVideoThread->join();
    }

    // Reset stuff
    m_videoPacketsQueue.clear();

    m_mainDisplayThread->interrupt();
    m_mainDisplayThread->join();

    // Free videoFrames
    {
        boost::lock_guard<boost::mutex> locker(m_videoFramesMutex);
        m_videoFramesQueue.clear();
        m_frameDisplayingRequested = false;
        ++m_generation;
    }

    m_videoResetting = false;

    if (resetVideo && !resetVideoProcessing())
        return false;

    m_mainDisplayThread.reset(new boost::thread(&FFmpegDecoder::displayRunnable, this));

    if (hasVideo)
    {
        if (m_videoCodecContext)
            avcodec_flush_buffers(m_videoCodecContext);
    }

    if (m_decoderListener)
    {
        m_decoderListener->changedFramePosition(m_startTime, seekDuration, m_duration + m_startTime);
    }

    seekWhilePaused();

    // Restart
    startVideoThread();

    return true;
}

void FFmpegDecoder::fixDuration()
{
    AVPacket packet;
    if (m_duration <= 0)
    {
        m_duration = 0;
        while (av_read_frame(m_formatContext, &packet) >= 0)
        {
            if (packet.stream_index == m_videoStreamNumber)
            {
                if (packet.pts != AV_NOPTS_VALUE)
                {
                    m_duration = packet.pts;
                }
                else if (packet.dts != AV_NOPTS_VALUE)
                {
                    m_duration = packet.dts;
                }
            }
            av_packet_unref(&packet);

            if (boost::this_thread::interruption_requested())
            {
                CHANNEL_LOG(ffmpeg_threads) << "Parse thread broken";
                return;
            }
        }

        if (avformat_seek_file(m_formatContext, m_videoStreamNumber, 0, 0, 0,
                               AVSEEK_FLAG_FRAME) < 0)
        {
            CHANNEL_LOG(ffmpeg_seek) << "Seek failed";
            return;
        }
    }
}
