
// StationDoc.h : CStaionDoc ��Ľӿ�
//


#pragma once


class CStaionDoc : public CDocument
{
protected: // �������л�����
	CStaionDoc();
	DECLARE_DYNCREATE(CStaionDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CStaionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


