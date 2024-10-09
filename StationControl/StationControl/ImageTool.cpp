// ImageTool.cpp : �����t�@�C��
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


// CImageTool �����o�֐�
/*============================================================================*/
/*! �A�v���P�[�V����

	-# �w���`�̃L���v�`�����擾����

	@param	rect				��`
	@param	bitmap				�r�b�g�}�b�v
	@retval	bool

*/
/*============================================================================*/
bool CImageTool::CaptureRect(const CRect& rect, CBitmap& bitmap)
{
	try{
		// �f�o�C�X�R���e�L�X�g�̍쐬
		CDC screenDC;
		CDC memDC;
		screenDC.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
		memDC.CreateCompatibleDC(&screenDC);
		// �r�b�g�}�b�v�̍쐬
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
/*! �C���[�W�c�[��

	-#�r�b�g�}�b�v�C���[�W���t�@�C���ɕۑ�����

	@param	pwnd		�E�B���h�E�n���h��
	@param	bitmap		�r�b�g�}�b�v�C���[�W
	@param	fileName	�o�̓t�@�C��
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
/*! �C���[�W�c�[��

	-# DDB�`������DIB�`���ւ̕ϊ�

	@param	bitmap			�r�b�g�}�b�v�C���[�W
	@param	dwCompression	�r�b�g�}�b�v
	@param	pPal			�p���b�g
	@retval	HANDLE	DIB�n���h��

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

	// ���̊֐��̓r�b�g�t�B�[���h�������Ɏ��ĂȂ�
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// �p���b�g���񋟂���Ă��Ȃ���΃f�t�H���g�p���b�g���g��
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// �r�b�g�}�b�v�����擾
	bitmap.GetObject(sizeof(bm),(TCHAR*)&bm);

	// BITMAPINFOHEADER��������
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

	// BITMAPINFOHEADER�ƃJ���[�e�[�u���̃T�C�Y���v�Z����
	int nColors = 0;
	if(bi.biBitCount <= 8)
		nColors = (1 << bi.biBitCount);
	if( nColors > 256 ) 
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// DIB�𓾂邽�߂ɂ̓f�o�C�X�R���e�L�X�g���K�v
	hDC = ::GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// BITMAPINFOHEADER�ƃJ���[�e�[�u����ێ�����̂ɕK�v�ȃ��������m��
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

	// lpBits��NULL�ɂ���GetDIBits���Ăяo���A
	// �f�o�C�X�h���C�o�� biSizeImage �t�B�[���h���v�Z������
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;
	GlobalUnlock(hDIB);

	// �����h���C�o��biSizeImage�t�B�[���h�𖄂߂Ă���Ȃ�������A���͂Ōv�Z����
	// �C���[�W�̊e�X�L�������C����DWORD(32�r�b�g)���E�ɔz�u�����
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
						* bi.biHeight;

		// �������k���g����Ȃ�A���ʓI�ɃT�C�Y���傫���Ȃ邩������Ȃ�
		// ���̓_���l�����ăT�C�Y�𑝂₵�Ă���
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// ���ׂẴr�b�g��ێ��ł���悤�Ƀo�b�t�@���Ċ��蓖�Ă���
	dwLen += bi.biSizeImage;
	HANDLE	hDIB_tmp = hDIB;
	if (handle = GlobalReAlloc(hDIB_tmp, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);
		// ���̃p���b�g��I��������
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL,hDC);
		return NULL;
	}

	// �r�b�g�}�b�v�̃r�b�g����擾����
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDIB);
	if (!lpbi){
		GlobalFree(hDIB);
		SelectPalette(hDC,hPal,FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	// �Ō��DIB�𓾂�
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// �J�n�X�L�������C��
				(DWORD)bi.biHeight,		// # �X�L�������C����
				(LPBYTE)lpbi 			// �r�b�g�}�b�v�r�b�g��̃A�h���X
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// BITMAPINFO�̃A�h���X
				(DWORD)DIB_RGB_COLORS);		// �J���[�e�[�u���ɂ�RGB���g��

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
/*! �C���[�W�c�[��

	-# DIB�`���C���[�W���t�@�C���ɏo�͂���

	@param	szFile			�t�@�C����
	@param	hDIB			DIB�n���h��
	@param	bitmap			�r�b�g�}�b�v�n���h��
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

	// BITMAPFILEHEADER�̃t�B�[���h��������
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= (DWORD)GlobalSize(hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	// �t�@�C���w�b�_����������
	file.Write( &hdr, sizeof(hdr) );

	// DIB�̃w�b�_�ƃr�b�g�����������
	file.Write(lpbi, (UINT)GlobalSize(hDIB));
	file.Close();

	return TRUE;
}
