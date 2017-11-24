/********************************************************************
	created:	2006/07/21
	created:	21:7:2006   22:15
	filename: 	D:\BeiJing\VC6\SmallTech\MFCHtml\ImpIDispatch.cpp
	file path:	D:\BeiJing\VC6\SmallTech\MFCHtml
	file base:	ImpIDispatch
	file ext:	cpp
	author:		万连文
	
	purpose:	重新实现自动化接口，实现和网页交互
*********************************************************************/

#include "stdafx.h"
#include "ImpIDispatch.h"

//#include "Station.h"
//#include "QUA_Station.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString cszCB_IsOurCustomBrowser		= "CB_IsOurCustomBrowser";
CString cszCB_CustomFunctionWithParams	= "CB_Station_Select";

#define	DISPID_CB_IsOurCustomBrowser		1
#define DISPID_CB_CustomFunctionWithParams	2

CImpIDispatch::CImpIDispatch(void)
{
	m_cRef = 0;
}

CImpIDispatch::~CImpIDispatch(void)
{
	ASSERT(m_cRef==0);
}

STDMETHODIMP CImpIDispatch::QueryInterface(REFIID riid, void** ppv)
{
	*ppv = NULL;
	
	if(IID_IDispatch == riid)
	{
        *ppv = this;
	}
	
	if(NULL != *ppv)
    {
		((LPUNKNOWN)*ppv)->AddRef();
		return NOERROR;
	}
	
	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CImpIDispatch::AddRef(void)
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CImpIDispatch::Release(void)
{
	return --m_cRef;
}

STDMETHODIMP CImpIDispatch::GetTypeInfoCount(UINT* /*pctinfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetTypeInfo(
			/* [in] */ UINT /*iTInfo*/,
            /* [in] */ LCID /*lcid*/,
            /* [out] */ ITypeInfo** /*ppTInfo*/)
{
	return E_NOTIMPL;
}

STDMETHODIMP CImpIDispatch::GetIDsOfNames(
			/* [in] */ REFIID riid,
            /* [size_is][in] */ OLECHAR** rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID* rgDispId)
{
	HRESULT		hr		= NOERROR;
	UINT		i		= 0;
	CString		cszName(rgszNames[i]);

	for(i=0; i<cNames; ++i)
	{
		if(cszName == cszCB_IsOurCustomBrowser)
		{
			rgDispId[i] = DISPID_CB_IsOurCustomBrowser;
		}
		else if(cszName == cszCB_CustomFunctionWithParams)
		{
			rgDispId[i] = DISPID_CB_CustomFunctionWithParams;
		}
		else
		{
			// One or more are unknown so set the return code accordingly
			hr = ResultFromScode(DISP_E_UNKNOWNNAME);
			rgDispId[i] = DISPID_UNKNOWN;
		}
	}
	return hr;
}

STDMETHODIMP CImpIDispatch::Invoke(
			/* [in] */ DISPID dispIdMember,
			/* [in] */ REFIID /*riid*/,
			/* [in] */ LCID /*lcid*/,
			/* [in] */ WORD wFlags,
			/* [out][in] */ DISPPARAMS* pDispParams,
			/* [out] */ VARIANT* pVarResult,
			/* [out] */ EXCEPINFO* /*pExcepInfo*/,
			/* [out] */ UINT* puArgErr)
{
//	CQUA_Station* pDlg = (CQUA_Station*)theApp.m_pMainWnd;

	if(dispIdMember == DISPID_CB_IsOurCustomBrowser)
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			VariantInit(pVarResult);
			V_VT(pVarResult) = VT_BOOL;
			V_BOOL(pVarResult) = true;
		}
	}
	
	if(dispIdMember == DISPID_CB_CustomFunctionWithParams) 
	{
		if(wFlags & DISPATCH_PROPERTYGET)
		{
			if(pVarResult != NULL)
			{
				VariantInit(pVarResult);
				V_VT(pVarResult) = VT_BOOL;
				V_BOOL(pVarResult) = true;
			}
		}
		
		if(wFlags & DISPATCH_METHOD)
		{
			// arguments come in reverse order for some reason
			CString	strArg1(pDispParams->rgvarg[3].bstrVal);			// in case you want a CString copy
			CString	strArg2(pDispParams->rgvarg[2].bstrVal);			// in case you want a CString copy
			CString	strArg3(pDispParams->rgvarg[1].bstrVal);			// in case you want a CString copy
			CString	strArg4(pDispParams->rgvarg[0].bstrVal);			// in case you want a CString copy
//			pDlg->My_Station_Select(strArg1, strArg2, strArg3, strArg4);
		}
	}

	return S_OK;
}
