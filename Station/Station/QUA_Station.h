#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"
#include "webbrowser2.h"												//类CWebBrowser2所在的头文件

// CQUA_Station 窗体视图

class CQUA_Station : public CFormView
{
	DECLARE_DYNCREATE(CQUA_Station)

protected:
	CQUA_Station();           // 动态创建所使用的受保护的构造函数
	virtual ~CQUA_Station();

public:
	enum { IDD = IDD_QUA_STATION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	//容器：Tree_Cell
	struct  Tree_Cell_Struct
	{
		bool	v_bStop;												//故障标志
		CString	v_sStation;												//小区机房
	};  
	typedef map<CString,Tree_Cell_Struct>	mTree_Cell;					//容器：小区信息
	//容器：Tree_BTS
	struct  Tree_BTS_Struct
	{
		bool		v_bBreak;											//断链标志
		CString		v_sBTS_Name;										//中文名
		CString		v_sCover;											//覆盖类型
		CString		v_sAgency;											//代维
		mTree_Cell	v_mTree_Cell;										//小区信息
	};  
	typedef map<CString,Tree_BTS_Struct> mTree_BTS;						//容器：BTS

	//容器：Tree_Room
	struct  Tree_Room_Struct
	{
		CString		v_sCover_Type;										//覆盖类型
		CString		v_sCover_Type_Cell;									//覆盖类型
		CString		v_sRoom_Cell;										//机房(小区)
		double		v_dLongitude_Cell;									//经度(小区)
		double		v_dLatitude_Cell;									//纬度(小区)
		double		v_dLongitude;										//经度_BTS
		double		v_dLatitude;										//纬度_BTS
		int			v_iType;											//机房类型：0：本站；1：拉远；2：主站(机房在拉远)；
		mTree_BTS	v_mTree_BTS;										//容器：BTS
	};  
	typedef map<CString,Tree_Room_Struct> mTree_Room;					//容器：机房

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();										//初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnDestroy();											//
	afx_msg void OnSize(UINT nType, int cx, int cy);					//
	afx_msg void OnBnClickedQuary();									//
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);	//树Item变化相应
	afx_msg void OnBnClickedExcel2();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnLvnGetdispinfoList3(NMHDR *pNMHDR, LRESULT *pResult);

	//自定义
	void		My_Query_Station();										//查询：站址
	void		My_LoadData_Station_Data(CString v_sSql);				//装载站址信息
	void		My_LoadData_Tree(CString v_sRoom,CString v_sStation_Cover,double v_dLongitude,double v_dLatitude);	//装载机房信息
	void		My_Show_Tree(CString v_sRoom);							//显示机房信息
	void		My_LoadData_Tree_Cell(CString v_sRoom,CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS);		//获取：小区信息【BTS\NodeB\GT BTS】
	void		My_LoadData_Tree_Cell_Remote(CString v_sRoom,int v_iType);		//获取：小区信息_拉远【BTS\NodeB\GT BTS】
	CString		My_Get_Station_BTS(CString v_sBTS,int v_iType,Tree_BTS_Struct* v_cTree_BTS,CString* v_sCover_Type,double* v_dLongitude,double* v_dLatitude);	//获取：基站机房【BTS\NodeB\GT BTS】
	void		My_Clear();												//容器清除
	void		My_ShowList_BTS(CString v_s1,CString v_s2,CString v_s3,CString v_s4,CString v_s5,CString v_s6);		//列表显示

	//分窗
	CSSplitter		m_SplitterPane_Main;								//垂直线
	CSSplitter		m_SplitterPane_Bottom;								//垂直线

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//------------------------------------------------------------------------------------------------------
	//	变量定义
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList_Station;									//Station信息
	CMyListCtrl		m_cList_BTS;
	CEdit			m_cSum;												//网元数量
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;
	CButton			m_cQuery;
	CButton			m_cExcel_NodeB;
	CButton			m_cMap_Get;
	CButton			m_cBTS;

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item_Room;									//选中的列表Item [Room]
	int				v_iCol_Lon,v_iCol_Lat;								//列号:经度、纬度

	mTree_Room		v_mTree_Room;										//机房容器
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
