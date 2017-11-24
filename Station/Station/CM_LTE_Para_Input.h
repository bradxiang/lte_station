#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CCM_LTE_Para_Input 对话框

//TD_ELEMENT
struct s_CON_Location
{
	CString		v_sAddress;											//地址
	double		v_dLongitude;										//经度
	double		v_dLatitude;										//纬度
};
typedef map <CString,s_CON_Location>	m_CON_Location;				//容器：[站点]


class CCM_LTE_Para_Input : public CDialog
{
	DECLARE_DYNAMIC(CCM_LTE_Para_Input)

public:
	CCM_LTE_Para_Input(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCM_LTE_Para_Input();

// 对话框数据
	enum { IDD = IDD_CM_LTE_PARA_Input };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton6();

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_Input_Main();										//导入_主程序
	void		My_File();												//文件选择
	bool		Get_CSV_Cell(CString v_sFile,mString_Int *v_mHead,bool v_bHead);		//导入CSV文件
	void		My_Input_Cell();										//Cell
	void		My_ShowList_Bug(CString v_sBug_Type,CString v_sBug);	//显示：异常信息

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	int			v_iThread_Exit;											//线程退出标记：2：强制退出；1：工作；0：线程停止

	//分窗
	CSSplitter		m_SplitterPane_Main;								//垂直线

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;
	CMyListCtrl		m_cList_Bug;
	CEdit			m_cFile_Name;										//
	CButton			m_cFile;											//
	CButton			m_cInput;											//
	CString			v_sFilename;										//文件名称
	CEdit			m_cSum;
	CButton			m_cExcel;
	CButton			m_cFile_Open;
	CEdit			m_cMaker;											//

	//自定义
	int				v_iWorking;											//工作代号：
	CString			v_sMaker_OK;										//厂家
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
