#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CLog_DataBase 窗体视图

class CLog_DataBase : public CFormView
{
	DECLARE_DYNCREATE(CLog_DataBase)

protected:
	CLog_DataBase();           // 动态创建所使用的受保护的构造函数
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();										//初始化
	afx_msg void OnDestroy();											//窗体销毁
	afx_msg void OnSize(UINT nType, int cx, int cy);					//调整窗体尺寸
	afx_msg void OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult);//列表排序
	afx_msg void OnBnClickedExcel();									//导出
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理 [截取键盘消息到窗体]

	//自定义
	void		My_LoadData();														//呈现：数据库容量信息
	void		My_LoadSize(CString v_sTable,CString v_sAlarm_Name,int v_iType);	//获取：基础表容量获取、显示
	void		My_LoadSize_Table(CString v_sTable,int v_iNum);						//获取：表容量信息
	void		My_LoadSize_Show(CString v_sTable,int v_iFlag,int v_iSum);			//显示：容量信息添加到列表
	void		My_Data_Reset();													//数据复位
	CString		My_Format(CString v_sTemp);											//货币格式化

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl	m_cList;												//列表窗
	CEdit		m_cDataBase;											//数据库容量

	//自定义
	CString		v_sFrame_Name;											//窗体名称
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器

	struct Data_Size
	{
		int	v_iRows[12];												//表记录数
		int	v_iReserved;												//占用容量
		int	v_iData;													//数据容量
		int	v_iIndex_size;												//索引容量
	};
	Data_Size	v_cData_Size;											//容量实例
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------

