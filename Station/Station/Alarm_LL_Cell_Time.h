#pragma once
#include "afxwin.h"
#include "afxdtctl.h"


// CAlarm_LL_Cell_Time �Ի���

class CAlarm_LL_Cell_Time : public CDialog
{
	DECLARE_DYNAMIC(CAlarm_LL_Cell_Time)

public:
	CAlarm_LL_Cell_Time(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAlarm_LL_Cell_Time();

// �Ի�������
	enum { IDD = IDD_ALARM_LL_CELL_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//�Զ���


	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CButton m_cDown;
	CButton m_cUp;
	CButton m_cTime_1;
	CButton m_cTime_2;
	CButton m_cTime_3;
	CButton m_cTime_4;
	CButton m_cTime_5;
	CDateTimeCtrl m_cDate_1;
	CDateTimeCtrl m_cDate_2;
	CDateTimeCtrl m_cDate_3;
	CDateTimeCtrl m_cDate_4;
	CDateTimeCtrl m_cDate_5;
	CComboBox m_cHour_1;
	CComboBox m_cHour_2;
	CComboBox m_cHour_3;
	CComboBox m_cHour_4;
	CComboBox m_cHour_5;
	CComboBox m_cCal_1;
	CComboBox m_cCal_2;
	CComboBox m_cCal_3;
	CComboBox m_cCal_4;
	CComboBox m_cCal_5;
	CEdit m_cValues_1;
	CEdit m_cValues_2;
	CEdit m_cValues_3;
	CEdit m_cValues_4;
	CEdit m_cValues_5;

	//�Զ���
	Alarm_LL_Cell*		v_dDate_Select;									//����ѡ��
	bool				v_bDown,v_bUp;									//��־��������Ч(���С�����)

	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
};
//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
