///////////////////////////////////////////////////////   
//sonystone 2002.4.3   
///////////////////////////////////////////////////////   
// MyRichEdit.cpp : implementation file   
//   
   
#include "stdafx.h"   
#include "MyRichEdit.h"   
   
#ifdef _DEBUG   
#define new DEBUG_NEW   
#undef THIS_FILE   
static char THIS_FILE[] = __FILE__;   
#endif   
   
   
#define ID_RICH_UNDO                    101   
#define ID_RICH_CUT                     102   
#define ID_RICH_COPY                    103   
#define ID_RICH_PASTE                   104   
#define ID_RICH_CLEAR                   105   
#define ID_RICH_SELECTALL               106   
#define ID_RICH_SETFONT                 107   
   
/////////////////////////////////////////////////////////////////////////////   
// CMyRichEdit   
   
CMyRichEdit::CMyRichEdit()   
{   
}   
   
CMyRichEdit::~CMyRichEdit()   
{   
}   
   
   
BEGIN_MESSAGE_MAP(CMyRichEdit, CRichEditCtrl)   
    //{{AFX_MSG_MAP(CMyRichEdit)   
    ON_WM_RBUTTONDOWN()   
    ON_COMMAND(ID_RICH_COPY, OnCopy)   
    ON_COMMAND(ID_RICH_CUT, OnCut)   
    ON_COMMAND(ID_RICH_PASTE, OnPaste)   
    ON_COMMAND(ID_RICH_SELECTALL, OnSelectall)   
    ON_COMMAND(ID_RICH_UNDO, OnUndo)   
    ON_COMMAND(ID_RICH_CLEAR, OnClear)   
    ON_COMMAND(ID_RICH_SETFONT, OnSelectfont)   
    //}}AFX_MSG_MAP   
END_MESSAGE_MAP()   
   
/////////////////////////////////////////////////////////////////////////////   
// CMyRichEdit message handlers   
   
void CMyRichEdit::OnRButtonDown(UINT nFlags, CPoint point)    
{   
    //����Ϊ����   
    SetFocus();   
    //����һ������ʽ�˵�   
    CMenu popmenu;   
    popmenu.CreatePopupMenu();   
    //��Ӳ˵���Ŀ   
  // popmenu.AppendMenu(0, ID_RICH_UNDO, "&Undo");   
  // popmenu.AppendMenu(0, MF_SEPARATOR);   
    popmenu.AppendMenu(0, ID_RICH_CUT, "(&T)����");   
    popmenu.AppendMenu(0, ID_RICH_COPY, "(&C)����");   
    popmenu.AppendMenu(0, ID_RICH_PASTE, "(&P)ճ��");   
  //  popmenu.AppendMenu(0, ID_RICH_CLEAR, "(&D)���");   
 //   popmenu.AppendMenu(0, MF_SEPARATOR);   
    popmenu.AppendMenu(0, ID_RICH_SELECTALL, "(&A)ȫѡ");   
    popmenu.AppendMenu(0, MF_SEPARATOR);   
    popmenu.AppendMenu(0, ID_RICH_SETFONT, "(&F)����");   
   
    //��ʼ���˵���   
    UINT nUndo=(CanUndo() ? 0 : MF_GRAYED );   
    popmenu.EnableMenuItem(ID_RICH_UNDO, MF_BYCOMMAND|nUndo);   
   
    UINT nSel=((GetSelectionType()!=SEL_EMPTY) ? 0 : MF_GRAYED) ;   
    popmenu.EnableMenuItem(ID_RICH_CUT, MF_BYCOMMAND|nSel);   
    popmenu.EnableMenuItem(ID_RICH_COPY, MF_BYCOMMAND|nSel);   
    popmenu.EnableMenuItem(ID_RICH_CLEAR, MF_BYCOMMAND|nSel);   
       
    UINT nPaste=(CanPaste() ? 0 : MF_GRAYED) ;   
    popmenu.EnableMenuItem(ID_RICH_PASTE, MF_BYCOMMAND|nPaste);   
   
    //��ʾ�˵�   
    CPoint pt;   
    GetCursorPos(&pt);   
    popmenu.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x, pt.y, this);   
    popmenu.DestroyMenu();   
    CRichEditCtrl::OnRButtonDown(nFlags, point);   
}   
   
void CMyRichEdit::OnSelectfont()    
{   
    // TODO: Add your command handler code here   
    CHARFORMAT cf;   
    LOGFONT lf;   
    memset(&cf, 0, sizeof(CHARFORMAT));   
    memset(&lf, 0, sizeof(LOGFONT));   
    //�ж��Ƿ�ѡ��������   
    BOOL m_bSelect = (GetSelectionType() != SEL_EMPTY) ? TRUE : FALSE;   
    if (m_bSelect)   
    {   
        GetSelectionCharFormat(cf);   
    }   
    else   
    {   
        GetDefaultCharFormat(cf);   
    }   
    //�õ������������   
    BOOL bIsBold = cf.dwEffects & CFE_BOLD;   
    BOOL bIsItalic = cf.dwEffects & CFE_ITALIC;   
    BOOL bIsUnderline = cf.dwEffects & CFE_UNDERLINE;   
    BOOL bIsStrickout = cf.dwEffects & CFE_STRIKEOUT;   
    //��������   
    lf.lfCharSet = cf.bCharSet;   
    lf.lfHeight = cf.yHeight/15;   
    lf.lfPitchAndFamily = cf.bPitchAndFamily;   
    lf.lfItalic = bIsItalic;   
    lf.lfWeight = (bIsBold ? FW_BOLD : FW_NORMAL);   
    lf.lfUnderline = bIsUnderline;   
    lf.lfStrikeOut = bIsStrickout;   
    sprintf(lf.lfFaceName, cf.szFaceName);   
       
    CFontDialog dlg(&lf);   
    dlg.m_cf.rgbColors = cf.crTextColor;   
    if (dlg.DoModal() == IDOK)   
    {   
        dlg.GetCharFormat(cf);//�����ѡ���������   
        if (m_bSelect)    
            SetSelectionCharFormat(cf); //Ϊѡ���������趨��ѡ����   
        else   
            SetWordCharFormat(cf);  //Ϊ��Ҫ����������趨����   
    }   
        
}   