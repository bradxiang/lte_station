#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CConLTEBusyIndicator 窗体视图

class CConLTEBusyIndicator : public CFormView
{
	DECLARE_DYNCREATE(CConLTEBusyIndicator)

protected:
	CConLTEBusyIndicator();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//自定义
	mFrame_Window::iterator	 v_pIterator_Frame;							//窗体：迭代器
	CString			         v_sFrame_Name;								//窗体名称
	CString		        	 v_sFilename;	
	//容器：忙时指标结构体
	struct  Busy_Indicator
	{  
		CString			cellname;											//小区名称	
		float			wireless_Utility;                                       //无线利用率
		float			UpOctDl;											//小区用户面下行字节数
		float			UpOctUl;											//小区用户面上行字节数
		float           EffectiveConnMean;									//有效RRC连接平均数
		float			PdschPrbMeanTot;                                    //下行PDSCHPRB可用平均数
		float			PdschPrbTotMeanDl;									//下行PDSCHPRB占用平均数
		float			PuschPrbMeanTot;                                    //下行PUSCHPRB可用平均数
		float			PuschPrbTotMeanUl;									//下行PUSCHPRB占用平均数

	};  
	int					v_iWorking;											 //工作代号
	map<CString,map<CString,CString>> file_content_;						 //文件内容
	list<Busy_Indicator> busy_indicator_;									 //忙时指标
	map_PM_Cell			Pm_Cell_;										 //容器：性能数据【保存获取的性能数据】
	//自定义
	//线程：导入
	CWinThread* v_pThread_Input;												//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);								//线程：导入
	int			v_iThread_Exit;													//线程退出标记：【1：工作；0：线程停止；2：强制退出】
	bool		My_ExecuteSQL(CString v_sSql);													//执行SQL命令
	void		My_Input_Main();																//导入_主程序
	void		My_File();																		//文件选择
	void		My_Input_Cell();																//性能
	void		OutputCellBusyIndicator();														//导出忙时性能指标
	void		ExcecuteBulkinsert(CString url,CString table_suffix,int control_count);			//执行bulkinsert语句
	void        ShowList();																		//List显示csv指标
	CSSplitter	m_SplitterPane_Main;															//分窗【垂直线】
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();											//初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);							//消息预处理
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


