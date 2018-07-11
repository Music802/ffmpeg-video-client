
// ISVideoClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ISVideoClient.h"
#include "ISVideoClientDlg.h"
#include "IsSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CISVideoClientApp

BEGIN_MESSAGE_MAP(CISVideoClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CISVideoClientApp ����

CISVideoClientApp::CISVideoClientApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CISVideoClientApp ����

CISVideoClientApp theApp;


// CISVideoClientApp ��ʼ��

BOOL CISVideoClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	HANDLE hMutex = CreateMutex(NULL, FALSE, _T("ISVideoClient"));
	if (hMutex == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "��ǰ����ϵͳ���Ѿ���������һ��ʵ��", "��ʾ:", MB_OK);
		return FALSE;
	}

	::CoInitialize(NULL);
	CIsSystem* pIsSystem = CIsSystem::GetInstance();
	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
	CPaintManagerUI::SetCurrentPath(CPaintManagerUI::GetInstancePath());
	CPaintManagerUI::SetResourcePath(_T("Skin"));
	//	CPaintManagerUI::SetResourceZip(_T("Main_dlg.zip"));

	CISVideoClientWnd *pFrame = new CISVideoClientWnd("Main_dlg.xml");

	pFrame->Create(NULL, _T("��Ƶ������"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowModal();

	::CoUninitialize();

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

