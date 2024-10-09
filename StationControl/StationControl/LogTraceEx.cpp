// LogTrace.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "LogTraceEx.h"

#include <ctime>
#include <locale.h>

CCriticalSection	CLogTraceEx::mSection;			//!< �������ݗp�N���e�B�J���Z�N�V����
CStdioFile			CLogTraceEx::mFile;				//!< ���O�t�@�C��
CStdioFile			CLogTraceEx::mFileXSL;			//!< XML�p�̃X�^�C���V�[�g
CString				CLogTraceEx::mFilePath;			//!< ���O�t�@�C���p�X���I�[��
CString				CLogTraceEx::mFileName;			//!< ���O�t�@�C����
CString				CLogTraceEx::mBuffer;				//!< ��������buffer
DWORD				CLogTraceEx::mFileSize;			//!< �t�@�C���T�C�Y���
DWORD				CLogTraceEx::mBufferWriteLimit;	//!< buffer��������臒l
bool				CLogTraceEx::mLogEnable;			//!< ���O�o�͉\���̃t���O
DWORD				CLogTraceEx::mLogLevel;			//!< ���O�o�͑Ώۃ��O���x��
DWORD				CLogTraceEx::mLogType;			//!< ���O�o�̓^�C�v

////////////////////////////////////////////////////////////////////////////////
// CLogDateTime
////////////////////////////////////////////////////////////////////////////////
/*============================================================================*/
/*! ���O���ԏ���
    �R���X�g���N�^
    	
    @param[in]
    	
    @retval �Ȃ�
*/
/*============================================================================*/
CLogDateTime::CLogDateTime()
{
    // Local Time �̎擾
    GetLocalTime(&mSystemTime);
}

/*============================================================================*/
/*! ���O���ԏ���
    �R���X�g���N�^
    	
    @param[in]	inDt	�w�莞��
    	
    @retval �Ȃ�
*/
/*============================================================================*/
CLogDateTime::CLogDateTime(const CString& inDt)
{
    // �ϊ��p�o�b�t�@ (Performance UP)
    CString buffer;
    buffer = inDt;
    // �t�F�[���Z�[�t�ŁC�K�v�������ȏ�ɂ��Ă���
    //                  YYYYMMDDhhmmssccc
    const TCHAR* zero = _T("00000000000000000");
    buffer += zero;

    // ���l�ւ̕ϊ��ׁ̈CNULL��}�����Ă���
    //   �����̂Ō�납������B�Ō���͊��ɓ����Ă���̂œ���Ȃ�
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhhmmssccc")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhhmmss")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhhmm")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDDhh")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMMDD")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYYMM")), _T("\0"));
    buffer.Insert((long)STRLEN(_T("YYYY")), _T("\0"));

    // �ϊ��p�|�C���^
    TCHAR* ptr;
    ptr = (TCHAR*)(LPCTSTR)buffer;

    // Year
    mSystemTime.wYear = (unsigned short)_wtoi(ptr);
	ptr += wcslen(_T("YYYY\0"))+1;
    // Month
    mSystemTime.wMonth = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("MM\0"))+1;
    // Day
    mSystemTime.wDay = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("DD\0"))+1;
    // Hour
    mSystemTime.wHour = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("hh\0"))+1;
    // Minute
    mSystemTime.wMinute = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("mm\0"))+1;
    // Second
    mSystemTime.wSecond = (unsigned short)_wtoi(ptr);
    ptr += wcslen(_T("ss\0"))+1;
    // mSecond
    mSystemTime.wMilliseconds = (unsigned short)_wtoi(ptr);

    // mSystemTime�̐������`�F�b�N
    if( (mSystemTime.wYear<1970 || mSystemTime.wYear>3000) || 
        (mSystemTime.wMonth<1   || mSystemTime.wMonth>12) || 
        (mSystemTime.wDay<1     || mSystemTime.wDay>31) ){

        // �������`�F�b�N�ŃG���[�ɂȂ����̂ŁA���ݎ����ɒu��������B
        GetLocalTime( &mSystemTime );
    }
}

/*============================================================================*/
/*! ���O���ԏ���
    �f�X�g���N�^
    	
    @param[in]
    	
    @retval �Ȃ�
*/
/*============================================================================*/
CLogDateTime::~CLogDateTime(void)
{
}

//============================================================================
/// @brief	�����̔�r���Z���s���܂�
/// @param
/// @return
//============================================================================
bool CLogDateTime::operator < (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) < 0);
}
bool CLogDateTime::operator > (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) > 0);
}
bool CLogDateTime::operator <= (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) <= 0);
}
bool CLogDateTime::operator >= (CLogDateTime& inDateTime) const
{
    return (_subSystemTime(mSystemTime, inDateTime.getSystemTime()) >= 0);
}
//============================================================================
/// @brief	�~���b�P�ʂ̑����Z
/// @param	prm1	inAdd	: �~���b
/// @return
//============================================================================
void CLogDateTime::addMilliSec(long inAdd)
{
    // ���邤�N�Ƃ��̌v�Z���ʓ|�Ȃ̂ŁC������ CTime�N���X�Ɍv�Z������
    //   CTime�ł́Cmsec���v�Z�ł��Ȃ��̂ŁC���������O�����B
    //   �����Ɨǂ����@���邩�ȁ`�B

    // MSEC�v�Z���オ��^������ۑ��p
    long carrySec, milliSec;

    ///// msec �͎��O�v�Z
    milliSec = mSystemTime.wMilliseconds;
    milliSec += inAdd; // nemoto
    // msec�� 0~999 �ɕ␳���āC���オ��C��������Z�o
    if(milliSec >= 0) {
        for(carrySec = 0; milliSec >= 1000; carrySec++, milliSec -= 1000);
    }
    else {
        for(carrySec = 0; milliSec < 0; carrySec--, milliSec += 1000);
    }

    // mSystemTime�̐������`�F�b�N
    if( (mSystemTime.wYear<1970 || mSystemTime.wYear>3000) || 
        (mSystemTime.wMonth<1   || mSystemTime.wMonth>12) || 
        (mSystemTime.wDay<1     || mSystemTime.wDay>31) ){
        // �G���[���́A�������Ȃ�
    }
    else{
        // �v�Z�pCTime�C���X�^���X�쐬�Ɠ����Ɍv�Z
        CTime t(mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond + carrySec);
        // System Time �ɖ߂�
        t.GetAsSystemTime(mSystemTime);
        mSystemTime.wMilliseconds = (unsigned short)milliSec;
    }
}
//============================================================================
/// @brief	DateTime_t �^�Ŏ擾
/// @param
/// @return	CString	����������
//============================================================================
CString CLogDateTime::getDateTime() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d%02d%02d%02d%02d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond, mSystemTime.wMilliseconds / 10);
    return str;
}
//============================================================================
/// @brief	�~���b�Ŏ擾
/// @param
/// @return	CString	����������
//============================================================================
CString CLogDateTime::getDateTimeMSOrder() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d%02d%02d%02d%03d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond, mSystemTime.wMilliseconds);
    return str;
}
//============================================================================
/// @brief	�b�܂ł̌`�Ŏ擾
/// @param
/// @return	CString	����������
//============================================================================
CString CLogDateTime::getDateTimeSecondOrder() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d%02d%02d%02d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond);
    return str;
}
//============================================================================
/// @brief	���O�t�@�C�����p�̓����Ŏ擾
/// @param
/// @return	CString	����������
//============================================================================
CString CLogDateTime::getDateTimeForLogFile() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d%02d%02d_%02d%02d%02d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond);
    return str;
}
//============================================================================
/// @brief	���O�p�̃^�C���X�^���v�����Ŏ擾
/// @param
/// @return	CString	����������
//============================================================================
CString CLogDateTime::getDateTimeForLogStamp() const
{ 
    CString str;
    //str.Preallocate(64);
    str.Format(_T("%04d/%02d/%02d %02d:%02d:%02d.%03d\0"), mSystemTime.wYear, mSystemTime.wMonth, mSystemTime.wDay, mSystemTime.wHour, mSystemTime.wMinute, mSystemTime.wSecond, mSystemTime.wMilliseconds);
    return str;
}
//============================================================================
/// @brief	100ms Order�̎��Ԃ��擾
/// @param
/// @return	CString	����������
//============================================================================
unsigned long CLogDateTime::getTime100msOrder() const
{
    return ((((mSystemTime.wHour * 60) + mSystemTime.wMinute) * 60) + mSystemTime.wSecond) * 10 + (mSystemTime.wMilliseconds / 100);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////// Private

//============================================================================
/// @brief	System Time �̌v�Z
/// @param	prm1	s1	: SYSTEMTIME����
/// @param	prm2	s2	: SYSTEMTIME����
/// @return	long	�v�Z����
//============================================================================
long CLogDateTime::_subSystemTime(const SYSTEMTIME& s1, const SYSTEMTIME& s2) const {
    // �v�Z�p�o�b�t�@
    unsigned long days1, days2, msecs1, msecs2;

    // ���ꂼ��̓��P�ʂł̒l���v�Z����
    days1 = (s1.wYear * 13 + s1.wMonth) * 32 + s1.wDay;
    days2 = (s2.wYear * 13 + s2.wMonth) * 32 + s2.wDay;
    // ���ꂼ��� msec �P�ʂł̒l���v�Z����
    msecs1 = ((s1.wHour * 60 + s1.wMinute) * 60 + s1.wSecond) * 1000 + s1.wMilliseconds;
    msecs2 = ((s2.wHour * 60 + s2.wMinute) * 60 + s2.wSecond) * 1000 + s2.wMilliseconds;

    // �܂��͓��ȏ�Ŕ�r
    if(days1 > days2) {
        return 1;
    }
    else if(days1 < days2) {
        return -1;
    }
    // ���ȏ�ō�������������Cmsec�Ŕ�r����
    else {
        if(msecs1 > msecs2) {
            return 1;
        }
        else if(msecs1 < msecs2) {
            return -1;
        }
        else {
            return 0;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// CLogTraceEx
////////////////////////////////////////////////////////////////////////////////
/*============================================================================*/
/*! ���O�g���[�X�֘A
    �R���X�g���N�^
    	
    @param[in]
    	
    @retval �Ȃ�
*/
/*============================================================================*/
CLogTraceEx::CLogTraceEx()
{
	mLogEnable = false;
}

/*============================================================================*/
/*! ���O�g���[�X�֘A
    �f�X�g���N�^
    	
    @param[in]
    	
    @retval �Ȃ�
*/
/*============================================================================*/
CLogTraceEx::~CLogTraceEx()
{
    if(mFile.m_hFile != CFile::hFileNull){
		_write(_T(""), true);
		Close();
	}
}

// CLogTrace �����o�[�֐�

//============================================================================
/// @brief	���O�t�@�C���̍쐬�y�я�����
/// @param	prm1	pFilePath	: ���O�o�͗p�t�@�C���p�X���i�I�[���j
/// @param	prm2	pFileName	: ���O�o�͗p�t�@�C�����i�g���q�Ȃ��j
/// @param	prm3	dwLogLevel	: ���O�o�͗p���x��
/// @param	prm4	dwLogType	: ���O�o�̓^�C�v
/// @return	bool	���������ꍇ��true��Ԃ��B����ȊO��false��Ԃ�
//============================================================================
bool CLogTraceEx::Create( const TCHAR* pFilePath, const TCHAR* pFileName, const DWORD dwLogLevel, const DWORD dwLogType)
{
	mLogLevel = dwLogLevel;
	mLogType = dwLogType;
	mFilePath = pFilePath;
	mFileName = pFileName;
    mFileSize = 1024 * 1024 * 10;
	mBufferWriteLimit = LOG_BUFFER_MAX * 8 / 10;
	mLogEnable = false;

	if(mLogLevel >= nLogEx::none)
		return false;

	if(_open() == false)
		return false;

	mLogEnable = true;
	return true;
}
void CLogTraceEx::Delete()
{
	mLogEnable = false;
}
//============================================================================
/// @brief	���O�t�@�C���̃N���[�Y
/// @param
/// @return
//============================================================================
void CLogTraceEx::Close()
{
	_write(_T(""), true);
	_close();
}
//============================================================================
/// @brief	���O�t�@�C���̏�������
/// @param	prm1	inSource		: �\�[�X�t�@�C����
/// @param	prm2	inClass			: �N���X��
/// @param	prm3	inMethod		: �֐���
/// @param	prm4	inDescription	: ����
/// @param	prm5	inDetails		: �ڍ�
/// @param	prm6	inLevel			: ���O���x��
/// @return
//============================================================================
void CLogTraceEx::Write(const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    mSection.Lock();

    try {
        // Log Level �m�F
        if(mLogLevel >= inLevel){
            if(mLogLevel >= nLogEx::none) {
			    mSection.Unlock();
				return;
            }

            // 1 line �쐬
            CString line;

			switch(mLogType){
				case	nLogEx::html:
					_makeHtmlLine(line, inSource, inClass, inMethod, inDescription, inDetails, inLevel);
					break;
				case	nLogEx::text:
					_makeTextLine(line, inSource, inClass, inMethod, inDescription, inDetails, inLevel);
					break;
				case	nLogEx::xml:
					_makeXmlLine(line, inSource, inClass, inMethod, inDescription, inDetails, inLevel);
					break;
			}

			// ���s�̒ǉ�
            line += _T("\r\n");

            //### Log �������ݔ��f ###//
            _write(line, true);
        }
    }
    catch(...) {
    }

    mSection.Unlock();
}
//============================================================================
/// @brief	���O�t�@�C���̍쐬�y�я�����
/// @param	prm1	pFileName		: ���O�o�͗p�t�@�C�����i�g���q�Ȃ��j
/// @param	prm2	dwLogLevel		: ���O�o�͗p���x��
/// @return bool	���������ꍇ��true��Ԃ��B����ȊO��false��Ԃ�
//============================================================================
bool CLogTraceEx::_open()
{
	// Log File �� Open �ƁC�m�F
    if(mFile.m_hFile == CFile::hFileNull) {
        // FILE OPEN
        CFileException e;
        // Path�̍쐬
        CString path;

		// ���ݓ����̎擾
	    CLogDateTime dt;
		path.Format(_T("%s%s_%s"), (LPCTSTR)mFilePath, (LPCTSTR)mFileName, (LPCTSTR)dt.getDateTimeForLogFile());

		switch(mLogType){
			case	nLogEx::html:	path += LOG_FILE_EXTENSION_HTML;	break;
			case	nLogEx::text:	path += LOG_FILE_EXTENSION_TEXT;	break;
			case	nLogEx::xml:	path += LOG_FILE_EXTENSION_XML;		break;
		}

		// File Open
        mFile.Open(path, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite, &e);
        if(e.m_cause != CFileException::none) {
            return false;
        }

		// �w�b�_�[�o��
		CString title;
		int i;
		switch(mLogType){
			case	nLogEx::html:
				title.Format(_T("%s%s%s"), (LPCTSTR)LOG_HTML_HEADER_1, (LPCTSTR)LOG_TITLE, (LPCTSTR)LOG_HTML_HEADER_2);
				_writetext(mFile, title);
				// �w�b�_�[�^�C�g���o��
 				for(i=0; i<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); i++){
					title.Format(LOG_HTML_HEADER_3, _LogHeadString[i]);
					_writetext(mFile, title);
				}
				break;
			case	nLogEx::text:
				break;
			case	nLogEx::xml:
				_createXSL();
				title.Format(_T("%s%s%s"), (LPCTSTR)LOG_XML_HEADER_1, (LPCTSTR)LOG_XML_HEADER_2, (LPCTSTR)LOG_XML_HEADER_3);
				_writetext(mFile, title);
				break;
		}

		mBuffer.Empty();
	}

	return true;
}
//============================================================================
/// @brief	���O�t�@�C���̃N���[�Y
/// @param
/// @return
//============================================================================
void CLogTraceEx::_close()
{
	try
	{
		// Log Open ���Ă���Ƃ�����
		if(mFile.m_hFile != CFile::hFileNull) {
			// �t�b�^�[�o��
			CString title;
			switch(mLogType){
				case	nLogEx::html:
					title.Format(_T("%s"), (LPCTSTR)LOG_HTML_FOOTER);
					_writetext(mFile, title);
					break;
				case	nLogEx::text:
					break;
				case	nLogEx::xml:
					title.Format(_T("%s"), (LPCTSTR)LOG_XML_FOOTER);
					_writetext(mFile, title);
					break;
			}

			// �t�@�C���N���[�Y
			mFile.Flush();
			mFile.Close();
		}
	}
	catch (CFileException* e)
	{
		_close();
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		_close();
		e->Delete();
	}
	catch (...)
	{
		_close();
	}
}
//============================================================================
/// @brief	XML�p�̃X�^�C���V�[�g�̍쐬
/// @param
/// @return
//============================================================================
void CLogTraceEx::_createXSL()
{
/*
http://www.envinfo.uee.kyoto-u.ac.jp/user/susaki/envinfo/xml_if.html
*/

	int	i,j;
	// Log File �� Open �ƁC�m�F
    if(mFileXSL.m_hFile == CFile::hFileNull) {
        // FILE OPEN
        CFileException e;
        // Path�̍쐬
        CString path;

		path.Format(_T("%s%s"), (LPCTSTR)mFilePath, (LPCTSTR)LOG_FILE_XSL);

		// File Open
        mFileXSL.Open(path, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, &e);
        if(e.m_cause != CFileException::none) {
            return;
        }

		// �w�b�_�[�o��
		CString title;
		title.Format(_T("%s%s%s"), (LPCTSTR)LOG_XSL_HEADER_1, (LPCTSTR)LOG_XSL_HEADER_2, (LPCTSTR)LOG_XSL_HEADER_3);
		_writetext(mFileXSL, title);
		title.Format(LOG_XSL_HEADER_4, LOG_TITLE, LOG_TITLE);
		_writetext(mFileXSL, title);
		// �e�[�u����`
		title.Format(_T("  <table border=\"0\" cellspacing=\"1\" cellpadding=\"0\">\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("   <tr>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("    <td bgcolor=\"#dddddd\">\r\n"));	// �{�[�_�[�J���[
		_writetext(mFileXSL, title);
		title.Format(_T("     <table border=\"0\" cellpadding=\"3\" cellspacing=\"1\">\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("      <tr>\r\n"));
		_writetext(mFileXSL, title);
 		for(i=0; i<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); i++){
			title.Format(_T("       <td bgcolor=\"#ffffff\"><b>%s</b></td>\r\n"), (LPCTSTR)_LogHeadString[i]);
			_writetext(mFileXSL, title);
		}
		title.Format(_T("      </tr>\r\n"));
		_writetext(mFileXSL, title);
		// ��������
		title.Format(_T("      <xsl:for-each select=\"LogFile/LogItem\">\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("        <xsl:choose>\r\n"));
		_writetext(mFileXSL, title);
		// ���O���x���̌�����������
		for(i=0; i<nLogEx::none; i++){
			title.Format(_T("          <xsl:when test=\"contains(Description, '%s')\">\r\n"), LogLevelStr[i].mark);
			_writetext(mFileXSL, title);
			title.Format(_T("            %s\r\n"), LogLevelStr[i].headerxml);
			_writetext(mFileXSL, title);
 			for(j=0; j<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); j++){
				title.Format(_T("            <td valign=\"top\"><nobr><xsl:value-of select=\"%s\" /></nobr></td>\r\n"), (LPCTSTR)_LogHeadString[j]);
				_writetext(mFileXSL, title);
			}
			title.Format(_T("            %s\r\n"), LogLevelStr[i].footerxml);
			_writetext(mFileXSL, title);
			title.Format(_T("          </xsl:when>\r\n"));
			_writetext(mFileXSL, title);
		}
		// ��L�ȊO�̏ꍇ�̒�`
		title.Format(_T("          <xsl:otherwise>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("            <tr bgcolor=\"#ffffff\" style=\"color:Black\">\r\n"));
		_writetext(mFileXSL, title);
 		for(i=0; i<sizeof(_LogHeadString) / sizeof(_LogHeadString[0]); i++){
			title.Format(_T("            <td valign=\"top\"><nobr><xsl:value-of select=\"%s\" /></nobr></td>\r\n"), (LPCTSTR)_LogHeadString[i]);
			_writetext(mFileXSL, title);
		}
		title.Format(_T("            </tr>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("          </xsl:otherwise>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("        </xsl:choose>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("      </xsl:for-each>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("     </table>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("    </td>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("   </tr>\r\n"));
		_writetext(mFileXSL, title);
		title.Format(_T("  </table>\r\n"));
		_writetext(mFileXSL, title);

		title.Format(_T("%s"), (LPCTSTR)LOG_XSL_FOOTER);
		_writetext(mFileXSL, title);

		// �t�@�C���N���[�Y
        mFileXSL.Flush();
        mFileXSL.Close();
	}
}
//============================================================================
/// @brief	HTML�p�^�O�̍쐬
/// @param	prm1	outLine			: �^�O������
/// @param	prm2	inSource		: �\�[�X�t�@�C����
/// @param	prm3	inClass			: �N���X��
/// @param	prm4	inMethod		: �֐���
/// @param	prm5	inDescription	: ����
/// @param	prm6	inDetails		: �ڍ�
/// @param	prm7	inLevel			: ���O���x��
/// @return
//============================================================================
void CLogTraceEx::_makeHtmlLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    CLogDateTime dt;

    // \t �� <BR>�ɕϊ�
    CString convDetails = inDetails;
    convDetails.Replace(_T("\t"), _T("<BR>"));
    // Table�ɏo��
    CString info;
    outLine.Format(_T("<TR BGCOLOR=#FFFFFF><TD NOWRAP><FONT SIZE=-2>%s%s%s</FONT></TD><TD NOWRAP>%s%s%s</TD><TD NOWRAP>%s%s%s</TD><TD NOWRAP>%s%s%s%s</TD><TD COLSPAN=2 NOWRAP>%s%s%s</TD></TR>\r\n"),
		LogLevelStr[inLevel].header, (LPCTSTR)dt.getDateTimeForLogStamp(), LogLevelStr[inLevel].footer,
        LogLevelStr[inLevel].header, inClass, LogLevelStr[inLevel].footer,
        LogLevelStr[inLevel].header, inMethod, LogLevelStr[inLevel].footer,
        LogLevelStr[inLevel].header, LogLevelStr[inLevel].mark, inDescription, LogLevelStr[inLevel].footer,
		LogLevelStr[inLevel].header, (LPCTSTR)convDetails, LogLevelStr[inLevel].footer);
}
//============================================================================
/// @brief	XML�p�^�O�̍쐬
/// @param	prm1	outLine			: �^�O������
/// @param	prm2	inSource		: �\�[�X�t�@�C����
/// @param	prm3	inClass			: �N���X��
/// @param	prm4	inMethod		: �֐���
/// @param	prm5	inDescription	: ����
/// @param	prm6	inDetails		: �ڍ�
/// @param	prm7	inLevel			: ���O���x��
/// @return
//============================================================================
void CLogTraceEx::_makeXmlLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    CLogDateTime dt;

    // Table�ɏo��
    CString info;
    outLine.Format(_T("  <LogItem>\r\n    <%s>%s</%s>\r\n    <%s>%s</%s>\r\n    <%s>%s</%s>\r\n    <%s>%s%s</%s>\r\n    <%s>%s</%s>\r\n  </LogItem>\r\n"),
		(LPCTSTR)_LogHeadString[0], (LPCTSTR)dt.getDateTimeForLogStamp(), (LPCTSTR)_LogHeadString[0],
		(LPCTSTR)_LogHeadString[1], inClass, (LPCTSTR)_LogHeadString[1],
		(LPCTSTR)_LogHeadString[2], inMethod, (LPCTSTR)_LogHeadString[2],
		(LPCTSTR)_LogHeadString[3], LogLevelStr[inLevel].mark, inDescription, (LPCTSTR)_LogHeadString[3],
		(LPCTSTR)_LogHeadString[4], inDetails, (LPCTSTR)_LogHeadString[4]);
}
//============================================================================
/// @brief	TEXT�p�^�O�̍쐬
/// @param	prm1	outLine			: �^�O������
/// @param	prm2	inSource		: �\�[�X�t�@�C����
/// @param	prm3	inClass			: �N���X��
/// @param	prm4	inMethod		: �֐���
/// @param	prm5	inDescription	: ����
/// @param	prm6	inDetails		: �ڍ�
/// @param	prm7	inLevel			: ���O���x��
/// @return
//============================================================================
void CLogTraceEx::_makeTextLine(CString& outLine, const TCHAR* inSource, const TCHAR* inClass, const TCHAR* inMethod, const TCHAR* inDescription, const TCHAR* inDetails, const DWORD inLevel)
{
    CLogDateTime dt;

    outLine.Format(_T("%s\t%s[%s::%s]\t(%s)\t%s\t%s"),
		(LPCTSTR)dt.getDateTimeForLogStamp(),
        inSource, inClass, inMethod, LogLevelStr[inLevel].name, inDescription, inDetails);
}
//============================================================================
/// @brief	���O�̏�������
/// @param	prm1	inLine			: �^�O������
/// @param	prm2	inForce			: ������������
/// @return
//============================================================================
void CLogTraceEx::_write(const CString& inLine, bool inForce)
{
	if(mLogEnable == false)
		return;

	if(mFile.m_hFile == CFile::hFileNull) {
		if(_open() == false)
			return;
	}

	try
	{
		mBuffer += inLine;
		// log �������ݔ��f
		if(((DWORD)mBuffer.GetLength() >= mBufferWriteLimit || inForce == true)){
			_writetext(mFile, mBuffer);
			mBuffer.Empty();
			//mBuffer.Preallocate( LOG_BUFFER_MAX );

			// �ő�T�C�Y�`�F�b�N
			// �ő�T�C�Y�𒴂������UClose���Ď���č쐬����
			if(mFile.GetLength() > mFileSize) {
				_close();
			}
		}
	}
	catch (CFileException* e)
	{
		mBuffer.Empty();
		_close();
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		mBuffer.Empty();
		_close();
		e->Delete();
	}
	catch (...)
	{
		mBuffer.Empty();
		_close();
	}
}
//============================================================================
/// @brief	���O�̏�������
/// @param	prm1	inLine			: �^�O������
/// @param	prm2	inForce			: ������������
/// @return
//============================================================================
void CLogTraceEx::_writetext(CStdioFile& file, const CString& buf)
{
	try
	{
		//_wsetlocale(LC_ALL,_T("japanese")); 
		CString conv = buf;
		conv.Replace(_T("\r\n"), _T("\n"));
		file.WriteString(conv);
	}
	catch (CFileException* e)
	{
		_close();
		e->Delete();
	}
	catch (CMemoryException* e)
	{
		_close();
		e->Delete();
	}
	catch (...)
	{
		_close();
	}
}
