#pragma once
#include "afxwin.h"


// CPm_GSM_UpDate_PM 对话框

class CPm_GSM_UpDate_PM : public CDialog
{
	DECLARE_DYNAMIC(CPm_GSM_UpDate_PM)

public:
	CPm_GSM_UpDate_PM(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPm_GSM_UpDate_PM();

// 对话框数据
	enum { IDD = IDD_PM_GSM_UPDATE_PM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);						//消息预处理
	afx_msg void OnCbnCloseupCombo4();
	afx_msg void OnBnClickedOk();

	//自定义
	bool		My_ExecuteSQL(CString v_sSql);							//执行SQL命令
	void		My_Get_PM_Table();										//获取性能表名称

	//------------------------------------------------------------------------------------------------------
	//	变量定义	
	//------------------------------------------------------------------------------------------------------
	CEdit		m_cType;
	CEdit		m_cFormula;
	CEdit		m_cPM_Name;												//性能名称
	CEdit		m_cObjType;
	CEdit		m_cUnit;
	CComboBox	m_cPM_Table;
	CButton		m_cOK;

	//自定义
	CString		v_sPM_Table;											//性能组
	CString		v_sType;												//类型
	CString		v_sPM_Name;												//类型
	CString		v_sUnit;												//单位
	CString		v_sFormula;												//计算公式
	CString		v_sObjType;												//ObjType
	CString		v_sNum;													//唯一标识
	bool		v_bFlag;												//标记：0：修改；1：增加；
	bool		v_bBSC;													//标记：0：Cell；1：BSC；

	mGSM_PM		v_mGSM_PM;												//容器性能表
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
