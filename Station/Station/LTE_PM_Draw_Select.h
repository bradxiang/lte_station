#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "LTE_PM_Draw.h"

// CLTE_PM_Draw_Select �Ի���

class CLTE_PM_Draw_Select : public CDialog
{
	DECLARE_DYNAMIC(CLTE_PM_Draw_Select)

public:
	CLTE_PM_Draw_Select(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLTE_PM_Draw_Select();

// �Ի�������
	enum { IDD = IDD_LTE_PM_Draw_Select };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnCloseupCombo7();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();

	//ListCtrl�϶�������
	afx_msg void OnLvnBegindragListMaster(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnLvnBegindragListSlave(NMHDR *pNMHDR, LRESULT *pResult);		//
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);						//
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);						//

	//�Զ���
	bool	My_ExecuteSQL(CString v_sSql);								//ִ��SQL����
	void	My_PM_Insert(CString v_sType,CString v_sPM_Name);			//���ӣ�ѡ�е�����


	//------------------------------------------------------------------------------------------------------
	//	��������	
	//------------------------------------------------------------------------------------------------------
	CComboBox			m_cType;
	CMyListCtrl			m_cList;
	CMyListCtrl			m_cList_PM;
	CEdit				m_cSum;

	//�Զ���
	mLTE_PM_Type*		v_mLTE_PM_Type;									//���ܲ���
	int					v_iSelect_PM_Type;								//ѡ��������������
	int					v_iList_Item;									//�б�ѡ��
	int					v_iList_Item_OK;								//�б�ѡ��

protected:
	//ListCtrl�϶�����
	//����
	CListCtrl*			m_pDragList;		//Which ListCtrl we are dragging FROM
	CListCtrl*			m_pDropList;		//Which ListCtrl we are dropping ON
	CImageList*			m_pDragImage;		//For creating and managing the drag-image
	BOOL				m_bDragging;		//T during a drag operation
	int					m_nDragIndex;		//Index of selected item in the List we are dragging FROM
	int					m_nDropIndex;		//Index at which to drop item in the List we are dropping ON
	CWnd*				m_pDropWnd;			//Pointer to window we are dropping on (will be cast to CListCtrl* type)
	//����
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);
};

//------------------------------------------------------------------------------------------------------			
//	END
//------------------------------------------------------------------------------------------------------
