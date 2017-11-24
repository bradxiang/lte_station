#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
//#include "LTE_PM_Data.h"
#include "MyDataStruct.h"


// LTE_PM_DATA_CASE �Ի���

class LTE_PM_DATA_CASE : public CDialog
{
	DECLARE_DYNAMIC(LTE_PM_DATA_CASE)

public:
	LTE_PM_DATA_CASE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~LTE_PM_DATA_CASE();

// �Ի�������
	enum { IDD = IDD_LTE_PM_DATA_CASE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedenter();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	void	My_PM_Insert(CString v_sType,CString v_sPM_Name);			//���ӣ�ѡ�е�����

	void	My_LoadData_Case();											//��ȡģ��
	void    My_LoadData_Case_All(CString v_sAccount,CString v_sName);	//��ȡģ����ϸ��Ϣ��С�������ܡ�

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//
	CMyListCtrl		m_cList_Cell;										//

	int				v_iList_Item;										//�б�ѡ��
	int				v_iList_Item_OK;									//�б�ѡ��
	int				v_iSelect;											//��־��1�����ܣ�0��С����

	mLTE_PM_Type*	v_mLTE_PM_Type;										//���������ܲ���
	set<CString>	v_mCell;											//������С��
	CButton m_cCheck;
	afx_msg void OnBnClickedCheck1();
	CEdit m_cSum;
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
