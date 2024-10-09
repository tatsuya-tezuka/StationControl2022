// SelectStation.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "StationControl.h"
#include "SelectStation.h"
#include "afxdialogex.h"


// CSelectStation �_�C�A���O

IMPLEMENT_DYNAMIC(CSelectStation, CDialogEx)

CSelectStation::CSelectStation(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectStation::IDD, pParent)
	, m_nStation(0)
	, m_nStationBefore(0)
	, m_bFixedStation(false)
{
	mEnableStation = 0x0f;
}

CSelectStation::~CSelectStation()
{
}

void CSelectStation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nStation);
	// ���\���C���f�b�N�X����V�Y20m, ���V�Y34m, �P�c64m, �P�c54m�̏��ɂ���
	DDX_Control(pDX, IDC_RADIO1, m_btStation[3]);
	DDX_Control(pDX, IDC_RADIO2, m_btStation[2]);
	DDX_Control(pDX, IDC_RADIO3, m_btStation[0]);
	DDX_Control(pDX, IDC_RADIO4, m_btStation[1]);
}


BEGIN_MESSAGE_MAP(CSelectStation, CDialogEx)
END_MESSAGE_MAP()


// CSelectStation ���b�Z�[�W �n���h���[


BOOL CSelectStation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �V�X�e���R�}���hSC_CLOSE�̎g�p�֎~
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
		pSysMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_GRAYED);

	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
		m_btStation[i].SetWindowText(mStationString[i]);
	}

	BYTE	byteFixedStation;
	if (m_bFixedStation)
	{
		// �ǌŒ�
		byteFixedStation = 1 << m_nStationBefore;
	}
	else
	{
		// �ǔ�Œ�
		byteFixedStation = 0x0f;
	}

	// �ǔԂ�\�����֕ϊ�
	m_nStation = mStationViewCnv[m_nStationBefore];

	// �ǑI���̊����񊈐�
	bool bEnableStation = false;
	for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
		if (byteFixedStation & mEnableStation & (1 << i)){
			bEnableStation = true;
		}
		else{
			m_btStation[i].EnableWindow(FALSE);
		}
	}

	// �ǔ�Œ莞�ɋǑI���_�C�A���O�̏�̋ǂ���A�I���\�ȃf�t�H���g�ǂ���������
	if (m_bFixedStation == false){
		BYTE chgOdrEnaStation = ChgOrderEnebleStation(mEnableStation);
		for (int i = 0; i < sizeof(mStationString) / sizeof(mStationString[0]); i++){
			if (chgOdrEnaStation & (1 << i)){
			}
			else{
				if (m_nStation == i)
					m_nStation++;
			}
		}
	}

	if (bEnableStation == false)
	{
		// �I���\�ȋǂ��Ȃ��ꍇ
		EndDialog(IDABORT);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

