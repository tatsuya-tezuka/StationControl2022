//******************************************************************************
// @file	ImageTool.h
// @brief	��ʃL���v�`���p�̃N���X
// @date	2012/04/01
// @author	SCC
//******************************************************************************
#pragma once

// CImageTool �R�}���h �^�[�Q�b�g

class CImageTool
{
public:
	CImageTool();
	virtual ~CImageTool();

//-----------------------------------------------------------------------------
//	�����o�֐�
//-----------------------------------------------------------------------------
public:
	bool	CaptureRect(const CRect& rect, CBitmap& bitmap);
	bool	SaveImageToFile(CWnd* pwnd, CBitmap& bitmap, CString fileName);

protected:
	HANDLE	DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal );
	BOOL	WriteDIB( TCHAR* szFile, HANDLE hDIB, CBitmap& bitmap);
	
//-----------------------------------------------------------------------------
//	�����o�ϐ�
//-----------------------------------------------------------------------------
public:
protected:
};


