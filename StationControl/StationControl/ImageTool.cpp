// ImageTool.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StationControl.h"
#include "ImageTool.h"


// CImageTool

CImageTool::CImageTool()
{
}

CImageTool::~CImageTool()
{
}


// CImageTool メンバ関数
/*============================================================================*/
/*! アプリケーション

	-# 指定矩形のキャプチャを取得する

	@param	rect				矩形
	@param	bitmap				ビットマップ
	@retval	bool

*/
/*============================================================================*/
bool CImageTool::CaptureRect(const CRect& rect, CBitmap& bitmap)
{
	try{
		// デバイスコンテキストの作成
		CDC screenDC;
		CDC memDC;
		screenDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		memDC.CreateCompatibleDC(&screenDC);
		// ビットマップの作成
		bitmap.CreateCompatibleBitmap(&screenDC, rect.Width(), rect.Height());
		CBitmap* oldbmp = memDC.SelectObject(&bitmap);

		DWORD dwRop = SRCCOPY | CAPTUREBLT;
		BOOL bRet = memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &screenDC, rect.left, rect.top, dwRop);
		memDC.SelectObject(oldbmp);
		memDC.DeleteDC();
		screenDC.DeleteDC();
	} catch(...){
		return false;
	}
	return true;
}

/*============================================================================*/
/*! イメージツール

	-#ビットマップイメージをファイルに保存する

	@param	pwnd		ウィンドウハンドル
	@param	bitmap		ビットマップイメージ
	@param	fileName	出力ファイル
	@retval

*/
/*============================================================================*/
bool CImageTool::SaveImageToFile(CWnd* pwnd, CBitmap& bitmap, CString fileName)
{
	if(pwnd == NULL || pwnd->GetSafeHwnd() == NULL)
		return false;

	CDC* pdc = pwnd->GetDC();
	// Create logical palette if device support a palette
	CPalette pal;
	if( pdc->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries = 
			GetSystemPaletteEntries( pdc->m_hDC, 0, 255, pLP->palPalEntry );

		// Create the palette
		pal.CreatePalette( pLP );

		delete[] pLP;
	}
	pwnd->ReleaseDC(pdc);

	HANDLE hDIB = DDBToDIB( bitmap, BI_RGB, &pal );
	if(hDIB == NULL)
		return false;

	TCHAR buf[_MAX_PATH];
	wsprintf(buf, _T("%s"), (LPCTSTR)fileName);
	// Write it to file
	WriteDIB( buf, hDIB, bitmap);
	GlobalFree(hDIB);
	return true;
}

/*============================================================================*/
/*! イメージツール

	-# DDB形式からDIB形式への変換

	@param	bitmap			ビットマップイメージ
	@param	dwCompression	ビットマップ
	@param	pPal			パレット
	@retval	HANDLE	DIBハンドル

*/
/*============================================================================*/
HANDLE CImageTool::DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal ) 
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB = NULL;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// この関数はビットフィールドを引数に持てない
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// パレットが提供されていなければデフォルトパレットを使う
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// ビットマップ情報を取得
	bitmap.GetObject(sizeof(bm),(TCHAR*)&bm);

	// BITMAPINFOHEADERを初期化
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// BITMAPINFOHEADERとカラーテーブルのサイズを計算する
	int nColors = 0;
	if(bi.biBitCount <= 8)
		nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// DIBを得るためにはデバイスコンテキストが必要
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// BITMAPINFOHEADERとカラーテーブルを保持するのに必要なメモリを確保
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	if (!lpbi){
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	*lpbi = bi;

	// lpBitsをNULLにしてGetDIBitsを呼び出し、
	// デバイスドライバに biSizeImage フィールドを計算させる
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;
	GlobalUnlock(hDIB);

	// もしドライバがbiSizeImageフィールドを埋めてくれなかったら、自力で計算する
	// イメージの各スキャンラインはDWORD(32ビット)境界に配置される
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// もし圧縮が使われるなら、結果的にサイズが大きくなるかもしれない
		// その点を考慮してサイズを増やしておく
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// すべてのビットを保持できるようにバッファを再割り当てする
	dwLen += bi.biSizeImage;
	HANDLE	hDIB_tmp = hDIB;
	if (handle = GlobalReAlloc(hDIB_tmp, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);
		// 元のパレットを選択し直す
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// ビットマップのビット列を取得する
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	if (!lpbi){
		GlobalFree(hDIB);
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	// 最後にDIBを得る
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// 開始スキャンライン
				(DWORD)bi.biHeight,		// # スキャンラインの
				(LPBYTE)lpbi 			// ビットマップビット列のアドレス
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// BITMAPINFOのアドレス
				(DWORD)DIB_RGB_COLORS);		// カラーテーブルにはRGBを使う

	if( !bGotBits ){
		GlobalFree(hDIB);
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	bi = *lpbi;
	GlobalUnlock(hDIB);

	SelectPalette(hDC,hPal,FALSE);
	::ReleaseDC(NULL,hDC);
	return hDIB;
}

/*============================================================================*/
/*! イメージツール

	-# DIB形式イメージをファイルに出力する

	@param	szFile			ファイル名
	@param	hDIB			DIBハンドル
	@param	bitmap			ビットマップハンドル
	@retval	BOOL

*/
/*============================================================================*/
BOOL CImageTool::WriteDIB( TCHAR* szFile, HANDLE hDIB, CBitmap& bitmap)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if( !file.Open( szFile, CFile::modeWrite|CFile::modeCreate) )
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	if (lpbi == NULL)
		return FALSE;

	int nColors = 0;
	if(lpbi->biBitCount <= 8)
		nColors = (1 << lpbi->biBitCount);

	// BITMAPFILEHEADERのフィールドを初期化
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= (DWORD)GlobalSize(hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	// ファイルヘッダを書き込む
	file.Write( &hdr, sizeof(hdr) );

	// DIBのヘッダとビット列を書き込む
	file.Write(lpbi, (UINT)GlobalSize(hDIB));
	file.Close();

	return TRUE;
}
