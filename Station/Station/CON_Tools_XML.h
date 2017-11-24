#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyFile_XML.h"


// CCON_Tools_XML 窗体视图

class CCON_Tools_XML : public CFormView
{
	DECLARE_DYNCREATE(CCON_Tools_XML)

protected:
	CCON_Tools_XML();           // 动态创建所使用的受保护的构造函数
	virtual ~CCON_Tools_XML();

public:
	enum { IDD = IDD_CON_XML_TOOLS };
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
	//屏幕
	virtual void OnInitialUpdate();										//初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理 [截取键盘消息到窗体]
	afx_msg void OnDestroy();											//销毁窗体
	afx_msg void OnSize(UINT nType, int cx, int cy);					//调整尺寸
	afx_msg void OnLvnColumnclickList6(NMHDR *pNMHDR, LRESULT *pResult);//ObjType列表排序(右击列表任意列)
	afx_msg void OnNMClickList6(NMHDR *pNMHDR, LRESULT *pResult);		//点击ObjType列表
	afx_msg void OnLvnColumnclickList4(NMHDR *pNMHDR, LRESULT *pResult);//Object列表排序(右击列表任意列)
	afx_msg void OnBnClickedButton4();									//获取XML文件信息
	afx_msg void OnBnClickedButton1();									//ObjType导出
	afx_msg void OnBnClickedButton2();									//Object导出
	afx_msg void OnNMClickList7(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnBnClickedButton5();

	//自定义
	void		My_Show_Object();										//显示Object信息
	void		My_Show_Object_Data(list_Object* v_pObject);			//显示Object_Data信息
	void		My_Show_Object_Child();									//显示Object_Child信息

	//线程：导入
	CWinThread* v_pThread_Input;										//线程指针
	static UINT My_Thread_Inoput(LPVOID lparam);						//线程：导入

	//分窗
	CSSplitter		m_SplitterPane_Main;								//垂直线
	CSSplitter		m_SplitterPane_Right;								//水平线

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	//屏幕
	CMyListCtrl m_cList_ObjType;										//ObjType列表
	CMyListCtrl m_cList_Object;											//Object列表
	CMyListCtrl m_cList_Child;											//PM子类
	CEdit		m_cFile_Name;											//选中的文件名称
	CButton		m_cOpen_File;											//导出打开
	CComboBox	m_cType;												//文件类型【配置、告警】
	CEdit		m_cSum;													//
	CButton		m_cFile;												//

	//自定义
	CString		v_sFrame_Name;											//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器

	CMyFile_XML	v_cFile_XML;											//XML文件操作
	int			v_iItem_ObjType;										//选中的ObjType条目序号
	int			v_iItem_ObjType_Child;									//选中的ObjType_Child条目序号
	CString		v_sName_ObjType;										//选中的ObjType条目名称
	CString		v_sName_Child;											//选中的Child条目名称
	int			v_iWorking;												//工作状态：0：ObjType；	1：ObjType_Child；
	unsigned int	v_iChild_Col_Max;									//子项：最大列数量
	bool		v_bRead;												//数据类型：0：配置、性能；1：性能直读：
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
