#pragma once

// Microsoft Visual C++ で作成された IDispatch ラッパー クラスを生成しました。

// メモ: このファイルの内容を変更しないでください。このクラスが
//  Microsoft Visual C++ で再生成された場合、変更は上書きされます。

/////////////////////////////////////////////////////////////////////////////
// CFapanel1 ラッパー クラス

class CFapanel1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CFapanel1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xDDE1290E, 0xEC7, 0x4C3C, { 0x8E, 0xC7, 0x72, 0xAE, 0x36, 0xF5, 0x5A, 0xDE } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// _DFAPanelBrowser

// Functions
//

	BOOL OpenFile(LPCTSTR FileName)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, FileName);
		return result;
	}
	VARIANT CallPanelMethod(LPCTSTR Name, VARIANT p1, VARIANT p2, VARIANT p3, VARIANT p4, VARIANT p5, VARIANT p6, VARIANT p7, VARIANT p8, VARIANT p9, VARIANT p10)
	{
		VARIANT result;
		static BYTE parms[] = VTS_BSTR VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT VTS_VARIANT ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Name, &p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10);
		return result;
	}
	CString GetLastError()
	{
		CString result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

CString GetFileName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}
void SetFileName(CString propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}
CString GetCurrentDirectory()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}
void SetCurrentDirectory(CString propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}


};
