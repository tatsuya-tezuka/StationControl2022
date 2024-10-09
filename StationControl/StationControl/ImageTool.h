//******************************************************************************
// @file	ImageTool.h
// @brief	画面キャプチャ用のクラス
// @date	2012/04/01
// @author	SCC
//******************************************************************************
#pragma once

// CImageTool コマンド ターゲット

class CImageTool
{
public:
	CImageTool();
	virtual ~CImageTool();

//-----------------------------------------------------------------------------
//	メンバ関数
//-----------------------------------------------------------------------------
public:
	bool	CaptureRect(const CRect& rect, CBitmap& bitmap);
	bool	SaveImageToFile(CWnd* pwnd, CBitmap& bitmap, CString fileName);

protected:
	HANDLE	DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal );
	BOOL	WriteDIB( TCHAR* szFile, HANDLE hDIB, CBitmap& bitmap);
	
//-----------------------------------------------------------------------------
//	メンバ変数
//-----------------------------------------------------------------------------
public:
protected:
};


