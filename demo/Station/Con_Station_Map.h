#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "MyMutiTreeCtrl.h"
#include "webbrowser2.h"												//类CWebBrowser2所在的头文件
#include <MSHTML.H>


// CCon_Station_Map 窗体视图

class CCon_Station_Map : public CFormView, public IDispatch
{
	DECLARE_DYNCREATE(CCon_Station_Map)

protected:
	CCon_Station_Map();													// 动态创建所使用的受保护的构造函数
	virtual ~CCon_Station_Map();

public:
	IHTMLDocument2*		m_pIHTMLDoc;									//

	enum { IDD = IDD_CON_STATION_MAP};
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	函数定义	
	//------------------------------------------------------------------------------------------------------
	virtual void OnInitialUpdate();											//初始化
	virtual BOOL PreTranslateMessage(MSG* pMsg);							//消息预处理
	afx_msg void OnDestroy();												//
	afx_msg void OnSize(UINT nType, int cx, int cy);						//
	afx_msg void OnBnClickedQuary();										//
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);		//树Item变化相应
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedExcel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedQuary3();
	afx_msg void OnBnClickedExcel2();

	//自定义
	void		My_Query_Station();																				//查询：站址
	void		My_LoadData_Station_Data(CString v_sSql);														//装载站址信息
	void		My_OutPut();																					//明细导出
	void		MyOutPut_Station(mStation_OutPut *v_mStation_OutPut,CString v_sFile_Name,bool v_bFile_Open);	//输出CSV文件
	void		My_Map_Station();																				//地图获取
	void		My_Marker();																					//绘制Marker
	void		GetStationBounds();

	//JS Call MFC
	void		My_Station_Select(CString v_sTop, CString v_sLeft,CString v_sBottom, CString v_sRight);
	afx_msg void OnDocumentCompleteExplorerMain(LPDISPATCH pDisp, VARIANT FAR* URL);

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
	CEdit			m_cSum;												//网元数量
	CEdit			m_cKey;												//Key
	CComboBox		m_cKey_Select;										//
	CButton			m_cQuery;											//
	CButton			m_cExcel;											//
	CButton			m_cOnline;											//在线选择
	CButton			m_cOK;												//有效站址
	CButton			m_cDetail;											//站址容器
	CButton			m_cMarker;
	CButton			m_cClear;

	//自定义
	mFrame_Window::iterator	v_pIterator_Frame;							//窗体：迭代器
	CString			v_sFrame_Name;										//窗体名称
	int				v_iWorking;											//工作代号：
	int				v_iList_Item_Room;									//选中的列表Item [Room]
	bool			v_bShow;											//调整屏幕尺寸标志
	bool			v_bLaod;											//HTML文件调用标志
	bool			time_control;										//控制定时器开关
	CWebBrowser2	m_webBrowser;										//浏览器控件

	//JS调用C++
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT *pctinfo);
	virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo);
	virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId);
	virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr);
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
	virtual ULONG STDMETHODCALLTYPE	AddRef();
	virtual ULONG STDMETHODCALLTYPE Release();
	//测试函数
	DWORD	GetProcessID();
	void	ShowMessageBox(const wchar_t *msg);	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//------------------------------------------------------------------------------------------------------
//	End
//------------------------------------------------------------------------------------------------------
