#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CConLTEBusyIndicator ������ͼ

class CConLTEBusyIndicator : public CFormView
{
	DECLARE_DYNCREATE(CConLTEBusyIndicator)

protected:
	CConLTEBusyIndicator();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CConLTEBusyIndicator();

public:
	enum { IDD = IDD_CON_LTE_BUSY_INDICATOR };
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
	//�Զ���
	mFrame_Window::iterator	 v_pIterator_Frame;							//���壺������
	CString			         v_sFrame_Name;								//��������
	CString		        	 v_sFilename;	
	//������æʱָ��ṹ��
	struct  Busy_Indicator
	{  
		CString			cellname;											//С������	
		float			wireless_Utility;                                       //����������
		float			UpOctDl;											//С���û��������ֽ���
		float			UpOctUl;											//С���û��������ֽ���
		float           EffectiveConnMean;									//��ЧRRC����ƽ����
		float			PdschPrbMeanTot;                                    //����PDSCHPRB����ƽ����
		float			PdschPrbTotMeanDl;									//����PDSCHPRBռ��ƽ����
		float			PuschPrbMeanTot;                                    //����PUSCHPRB����ƽ����
		float			PuschPrbTotMeanUl;									//����PUSCHPRBռ��ƽ����

	};  
	int					v_iWorking;											 //��������
	map<CString,map<CString,CString>> file_content_;						 //�ļ�����
	list<Busy_Indicator> busy_indicator_;									 //æʱָ��
	map_PM_Cell			Pm_Cell_;										 //�������������ݡ������ȡ���������ݡ�
	//�Զ���
	//�̣߳�����
	CWinThread* v_pThread_Input;												//�߳�ָ��
	static UINT My_Thread_Inoput(LPVOID lparam);								//�̣߳�����
	int			v_iThread_Exit;													//�߳��˳���ǣ���1��������0���߳�ֹͣ��2��ǿ���˳���
	bool		My_ExecuteSQL(CString v_sSql);													//ִ��SQL����
	void		My_Input_Main();																//����_������
	void		My_File();																		//�ļ�ѡ��
	void		My_Input_Cell();																//����
	void		OutputCellBusyIndicator();														//����æʱ����ָ��
	void		ExcecuteBulkinsert(CString url,CString table_suffix,int control_count);			//ִ��bulkinsert���
	void        ShowList();																		//List��ʾcsvָ��
	CSSplitter	m_SplitterPane_Main;															//�ִ�����ֱ�ߡ�
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();											//��ʼ��
	virtual BOOL PreTranslateMessage(MSG* pMsg);							//��ϢԤ����
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonFileSelect();
	afx_msg void OnBnClickedButtonInputDatabase();
	afx_msg void OnBnClickedButtonOutputDatabase();
	CListCtrl m_cList;
	CEdit m_cFile_Name;
	CButton m_cFile;
	CButton m_cInput;
	CButton m_cOutput;
	afx_msg void OnBnClickedButtonReadCsv();
	CButton m_cFileRead;
};


