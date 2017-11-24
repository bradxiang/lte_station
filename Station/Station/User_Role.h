#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CUser_Role 对话框

class CUser_Role : public CDialog
{
	DECLARE_DYNAMIC(CUser_Role)

public:
	CUser_Role(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUser_Role();

// 对话框数据
	enum { IDD = IDD_USER_ROLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();										//
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnSize(UINT nType, int cx, int cy);					//
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);//
	afx_msg void OnBnClickedMscKpiExcel();								//
	afx_msg void OnBnClickedAddMscKpi2();								//
	afx_msg void OnBnClickedButton23();									//
	afx_msg void OnBnClickedButton25();									//
	afx_msg void OnBnClickedCheck1();									//
	afx_msg void OnBnClickedAll();										//
	afx_msg void OnBnClickedNull();										//
	afx_msg void OnBnClickedSave();										//


	//自定义
	void		My_LoadData();											//呈现：数据表信息
	void		My_LoadData_Power();									//呈现：权限信息
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	bool		My_New_Name(CString v_sName);							//名称重复检查
	void		My_GetPower();											//获得：权限
	void		My_GetPower_OK(CString v_sRolie_ID);					//获得：授权权限

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CMyListCtrl		m_cList;											//列表
	CMyListCtrl		m_cList_Power;										//列表
	CButton			m_cRole_Flag;										//标志：扩展显示
	CEdit			m_cAdd_Role_Name;									//增加：角色名称
	CEdit			m_cAdd_Role_Marker;									//增加：角色备注
	CEdit			m_cMod_Role_Marker;									//修改：角色备注
	CEdit			m_cMod_Role_Name;									//修改：角色名称
	CEdit			m_cDel_Role_Name;									//删除：角色名称
	CEdit			m_cSum;												//Role总合

	//自定义
	int				v_iList_State;										//标志：0：查询；	1：增加；	2：修改；
	int				v_iList_Item;										//选中的条目序号
	CString			v_sList_Item_ID;									//选中条目的ID

	//权限：容器
	mUSER_Power		v_mUSER_Power;										//用户权限容器

};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
