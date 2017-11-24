#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"



// CQUA_Station_Out 窗体视图

class CQUA_Station_Out : public CFormView
{
	DECLARE_DYNCREATE(CQUA_Station_Out)

protected:
	CQUA_Station_Out();           // 动态创建所使用的受保护的构造函数
	virtual ~CQUA_Station_Out();

public:
	enum { IDD = IDD_QUA_STATION_OUT };
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
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedButton2();

	//自定义
	void		My_Query_Station();										//查询：站址
	void		My_LoadData_Station_Data(CString v_sSql);				//装载站址信息
	void		My_LoadData_Tree(CString v_sRoom);						//装载机房信息
	void		My_LoadData_Tree_Node_Cell(CString v_sNodeb,HTREEITEM v_hBTS_Node);		//获取：小区信息【Nodeb】
	void		My_OutPut();											//明细导出
	void		MyOutPut_Station(mStation_OutPut *v_mStation_OutPut,CString v_sFile_Name,bool v_bFile_Open);	//输出CSV文件

	//分窗
	CSSplitter		m_SplitterPane_Main;								//垂直线
	CSSplitter		m_SplitterPane_Bottom;								//垂直线

	//m_cTree
	CMyMutiTreeCtrl	m_cTree_Station;
	CImageList		v_cImage_Tree;										//树图标：标致
	CImageList		v_cImage_State;										//树图标：状态
	int				v_iItem_Tree;										//选中的Tree条目序号
	HTREEITEM		v_hBTS_Root,v_hBTS_Node;							//根目录句柄、节点句柄；

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入
	void		My_Input_Main();										//导入_主程序

	//------------------------------------------------------------------------------------------------------
	//	变量定义
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList_Station;									//Station信息
	CEdit			m_cSum;												//网元数量
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;										//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CButton			m_cOnline;											//在线选择
	CButton			m_cOK;												//有效站址
	CButton			m_cDetail;											//站址容器

	//自定义
	CString			v_sFrame_Name;										//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	int				v_iWorking;											//工作代号：
	int				v_iList_Item_Room;									//选中的列表Item [Room]
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
