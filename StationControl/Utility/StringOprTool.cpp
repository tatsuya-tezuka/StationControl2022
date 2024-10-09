/*============================================================================*/
/*! StringOprTool.cpp

-# �����񑀍�c�[��
*/
/*============================================================================*/
#include "stdafx.h"
#include "..\StationControl\StationControl.h"
#include "StringOprTool.h"


/*============================================================================*/
/*! CStringOprTool

-# �R���X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CStringOprTool::CStringOprTool()
{
}


/*============================================================================*/
/*! CStringOprTool

-# �f�X�g���N�^

@param  �Ȃ�
@retval �Ȃ�
*/
/*============================================================================*/
CStringOprTool::~CStringOprTool()
{
}


/*============================================================================*/
/*! CStringOprTool

-# �R�����g�A�󔒂̍폜

@param  strDst�F�폜��̕�����
@param  comment�F�R�����g����
@param  strSrc�F���̕�����
@retval TRUE�F�R�����g�A�󔒍폜�����^FALSE�F�R�����g�A�󔒂��Ȃ�����
*/
/*============================================================================*/
BOOL CStringOprTool::TrimComment(CString& strDst, TCHAR comment, CString& strSrc)
{
	int bPos =	strSrc.Find(comment);
	if (bPos >= 0)
	{
		strDst = strSrc.Left(bPos);
	}
	else
	{
		strDst = strSrc;
	}
	strDst = strDst.Trim();

	if (strDst.Compare(strSrc) == 0)
		return FALSE;
	else
		return TRUE;
}


/*============================================================================*/
/*! CStringOprTool

-# �����̋󔒂��P�ɒu��������

@param  strSrcDst�F������
@retval TRUE�F�u���������^FALSE�F�u�������Ȃ�����
*/
/*============================================================================*/
BOOL CStringOprTool::ReplaceMultipleSpace(CString& strSrcDst)
{
	BOOL bRet = FALSE;
	int nReplaced = 0;
	do
	{
		nReplaced = strSrcDst.Replace(_T("  "), _T(" "));
		if (nReplaced != 0)
		{
			bRet = TRUE;
		}
	} while (nReplaced != 0);

	return bRet;
}

