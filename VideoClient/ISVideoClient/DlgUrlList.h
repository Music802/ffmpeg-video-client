#pragma once

#include "FcListCtrlEx.h"
#include "IsSystem.h"

// CDlgAddressList �Ի���

class CDlgUrlList : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUrlList)

public:
	CDlgUrlList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUrlList();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ADDRESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()


private:
	CFcListCtrlEx	m_lstAdress;
	CIsOptions*		m_pIsOptions;
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
};
