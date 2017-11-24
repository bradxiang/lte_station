#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CLog_DataBase ������ͼ

class CLog_DataBase : public CFormView
{
	DECLARE_DYNCREATE(CLog_DataBase)

protected:
	CLog_DataBase();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CLog_DataBase();

public:
	enum { IDD = IDD_LOG_DATABASE };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();										//��ʼ��
	afx_msg void OnDestroy();											//��������
	afx_msg void OnSize(UINT nType, int cx, int cy);					//��������ߴ�
	afx_msg void OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);//�б�����
	afx_msg void OnBnClickedExcel();									//����
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//��ϢԤ���� [��ȡ������Ϣ������]

	//�Զ���
	void		My_LoadData();														//���֣����ݿ�������Ϣ
	void		My_LoadSize(CString v_sTable,CString v_sAlarm_Name,int v_iType);	//��ȡ��������������ȡ����ʾ
	void		My_LoadSize_Table(CString v_sTable,int v_iNum);						//��ȡ����������Ϣ
	void		My_LoadSize_Show(CString v_sTable,int v_iFlag,int v_iSum);			//��ʾ��������Ϣ��ӵ��б�
	void		My_Data_Reset();													//���ݸ�λ
	CString		My_Format(CString v_sTemp);											//���Ҹ�ʽ��

	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl	m_cList;												//�б�
	CEdit		m_cDataBase;											//���ݿ�����

	//�Զ���
	CString		v_sFrame_Name;											//��������
	mFrame_Window::iterator	v_pIterator_Frame;							//���壺������

	struct Data_Size
	{
		int	v_iRows[12];												//���¼��
		int	v_iReserved;												//ռ������
		int	v_iData;													//��������
		int	v_iIndex_size;												//��������
	};
	Data_Size	v_cData_Size;											//����ʵ��
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

