#pragma once
#include "afxdtctl.h"
#include "atltime.h"

// CFrequencyCtrlMon �_�C�A���O


class CFrequencyCtrlMon : public CDialogBase
{
	DECLARE_DYNAMIC(CFrequencyCtrlMon)

public:
	CFrequencyCtrlMon(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	CFrequencyCtrlMon(UINT IDD, CWnd* pParent = NULL);
	virtual ~CFrequencyCtrlMon();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_FREQ_CTRL_MONITOR };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum{
		eINT,
		eDOUBLE,
		eSTRING,
	};
	typedef struct{
		const CString		monname;		// �Ď���
		CString&			controlval;		// �R���g���[���l
		const CString		format;			// �t�H�[�}�b�g
		UINT				type;			// ��ʁF0:INT 1:FLOAT 2:DOUBLE 3:STRING
	} stSetMonData;
protected:

	//-----------------------------------------------------------------------------
	//	�����o�ϐ�
	//-----------------------------------------------------------------------------
public:
protected:
	CTrendData	mTrendData;
	CWnd*		mCallWnd;
	CString		mTargetSatellite;
	CString		mTargetPredAnp;
	double		mTxFreqVal;		// ���M���g���l[Hz]
	double		mRxFreqVal[2];	// (S,X)��M���g���l[Hz]
	long		mTXTType;		// 0=S�� 1=X��

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void	SetCallWnd(CWnd* p)	{ mCallWnd = p; }
	virtual BOOL UpdateDlg();

protected:
	void SetData();
	bool _getTxFqVal(char* pbuf);
	bool _getRxFqVal(char* pbuf);
	bool _getXTXTypeVal(char* pbuf);
	bool _convertPredFileName(CString& monPredFileName, CString& satName, CString& anpName);

	void GetEqstatDbValue(CString& strst);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	// �ݒ���g��-U/C
	CString m_strUC;
	// �ݒ���g��-D/C
	CString m_strDC;
	// �ݒ���g��-�R�}���h�L�����A
	CString m_strCmdCareer;
	// �I�t�Z�b�g-���M���g���I�t�Z�b�g
	CString m_strTFreqOffset;
	// �I�t�Z�b�g-�����I�t�Z�b�g
	CString m_strTimeOffset;
	// �\��l���-RTT�iUp Link�j
	CString m_strForecastRTTUp;
	// �\��l���-RTT�iDown Link�j
	CString m_strForecastRTTDw;
	// �h�b�v���⏞-���M�h�b�v��
	CString m_strTDoppler;
	// �h�b�v���⏞-��M�h�b�v��
	CString m_strRDoppler;
	// �h�b�v���⏞-�R�}���h�L�����A
	CString m_strDopplerCmdCareer;
	// �T���@COH���[�h
	CString m_strSpacecraftCOHMode;
	// �\�����g���iTX)
	CString m_strPreFreqTx;
	// �\�����g���iRX)
	CString m_strPreFreqRx;
	// �\�����g���iSC)
	CString m_strPreFreqSc;

	virtual BOOL OnInitDialog();
	CDateTimeCtrl m_PredTimeDayCtrl;
	CTime m_tPredDay;
	CTime m_tPredTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_strRFreqOffset;
};
