#ifndef __UIBASE_H__
#define __UIBASE_H__


#pragma once

namespace DuiLib {
	/////////////////////////////////////////////////////////////////////////////////////
	//

#define UI_WNDSTYLE_CONTAINER  (0)
#define UI_WNDSTYLE_FRAME      (WS_VISIBLE | WS_OVERLAPPEDWINDOW)
#define UI_WNDSTYLE_CHILD      (WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)
#define UI_WNDSTYLE_DIALOG     (WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION | WS_DLGFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN)

#define UI_WNDSTYLE_EX_FRAME   (WS_EX_WINDOWEDGE)
#define UI_WNDSTYLE_EX_DIALOG  (WS_EX_TOOLWINDOW | WS_EX_DLGMODALFRAME)

#define UI_CLASSSTYLE_CONTAINER  (0)
#define UI_CLASSSTYLE_FRAME      (CS_VREDRAW | CS_HREDRAW)
#define UI_CLASSSTYLE_CHILD      (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)
#define UI_CLASSSTYLE_DIALOG     (CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS | CS_SAVEBITS)


	/////////////////////////////////////////////////////////////////////////////////////
	//
#ifndef ASSERT
#define ASSERT(expr)  _ASSERTE(expr)
#endif

#ifdef _DEBUG
#ifndef DUITRACE
#define DUITRACE DUI__Trace
#endif
#define DUITRACEMSG DUI__TraceMsg
#else
#ifndef DUITRACE
#define DUITRACE
#endif
#define DUITRACEMSG _T("")
#endif

	void UILIB_API DUI__Trace(LPCTSTR pstrFormat, ...);
	LPCTSTR UILIB_API DUI__TraceMsg(UINT uMsg);

	/////////////////////////////////////////////////////////////////////////////////////
	//

	class UILIB_API CNotifyPump
	{
	public:
		bool AddVirtualWnd(CDuiString strName,CNotifyPump* pObject);
		bool RemoveVirtualWnd(CDuiString strName);
		void NotifyPump(TNotifyUI& msg);
		bool LoopDispatch(TNotifyUI& msg);
		DUI_DECLARE_MESSAGE_MAP()
	private:
		CStdStringPtrMap m_VirtualWndMap;
	};

	class UILIB_API CWindowWnd
	{
	public:
		CWindowWnd();

		HWND GetHWND() const;
		operator HWND() const;

		bool RegisterWindowClass();
		bool RegisterSuperclass();

		HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, HMENU hMenu = NULL);
		HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);
		HWND CreateDuiWindow(HWND hwndParent, LPCTSTR pstrWindowName,DWORD dwStyle =0, DWORD dwExStyle =0);
		HWND Subclass(HWND hWnd);
		void Unsubclass();
		void ShowWindow(bool bShow = true, bool bTakeFocus = true);
		UINT ShowModal();
		void Close(UINT nRet = IDOK);
		void CenterWindow();	// ���У�֧����չ��Ļ
		void SetIcon(UINT nRes);

		LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
		LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
		void ResizeClient(int cx = -1, int cy = -1);

	protected:
		virtual LPCTSTR GetWindowClassName() const = 0;
		virtual LPCTSTR GetSuperClassName() const;
		virtual UINT GetClassStyle() const;

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void OnFinalMessage(HWND hWnd);

		static LRESULT CALLBACK __WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK __ControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		HWND m_hWnd;
		WNDPROC m_OldWndProc;
		bool m_bSubclassed;
	};

	class UILIB_API CharConverter
	{
	protected:
		CharConverter(void);
		~CharConverter(void);

	public:
		/**
		* @brief ���ַ�->���ֽڷ�
		* @param [in] lpWChar Ҫת���Ŀ��ַ��ַ�������NULL��ת����ֻ����ת������ַ�������
		* @param [out] lpMByte ת����Ķ��ֽڷ��ַ���
		* @return ����ת������ַ������ȣ������ַ���������
		*/
		static int WChar2MByte(const wchar_t* lpWChar, char* lpMByte);

		/**
		* @brief ���ֽڷ�->���ַ�
		* @param [in] lpMByte Ҫת���Ķ��ֽڷ��ַ�������NULL��ת����ֻ����ת������ַ�������
		* @param [out] lpWChar ת����Ŀ��ַ��ַ���
		* @return ����ת������ַ������ȣ������ַ���������
		*/
		static int MByte2WChar(const char* lpMByte, wchar_t* lpWChar);

		/**
		* @brief ���ַ�->Utf8
		* @param [in] lpWChar Ҫת���Ŀ��ַ��ַ�������NULL��ת����ֻ����ת������ַ�������
		* @param [out] lpUtf8 ת�����Utf8�ַ���
		* @return ����ת������ַ������ȣ������ַ���������
		*/
		static int WChar2Utf8(const wchar_t* lpWChar, char* lpUtf8);

		/**
		* @brief Utf8->���ַ�
		* @param [in] lpMByte Ҫת����Utf8�ַ�������NULL��ת����ֻ����ת������ַ�������
		* @param [out] lpWChar ת����Ŀ��ַ��ַ���
		* @return ����ת������ַ������ȣ������ַ���������
		*/
		static int Utf82WChar(const char* lpUtf8, wchar_t* lpWChar);

		/**
		* @brief ���ֽڷ�->Utf8
		* @param [in] lpChar Ҫת���Ŀ��ַ��ַ�������NULL��ת����ֻ����ת������ַ�������
		* @param [out] lpUtf8 ת�����Utf8�ַ���
		* @return ����ת������ַ������ȣ������ַ���������
		*/
		static int MByte2Utf8(const char* lpChar, char* lpUtf8);

		/**
		* @brief Utf8->���ֽڷ�
		* @param [in] lpUtf8 Ҫת����Utf8�ַ�������NULL��ת����ֻ����ת������ַ�������
		* @param [out] lpChar ת����Ŀ��ַ��ַ���
		* @return ����ת������ַ������ȣ������ַ���������
		*/
		static int Utf82MByte(const char* lpUtf8, char* lpChar);

		static int RTrim(char* lpChar);
		static int LTrim(char* lpChar);
	protected:
		static int WChar2MByteBase(const wchar_t* lpWChar, char* lpMByte, UINT uPageCode);
		static int MByte2WCharBase(const char* lpMByte, wchar_t* lpWChar, UINT uPageCode);
	};
} // namespace DuiLib


#endif // __UIBASE_H__
