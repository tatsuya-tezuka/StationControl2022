#pragma once
#include "afxwin.h"


// CSelectStation �_�C�A���O

class CSelectStation : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectStation)

public:
	CSelectStation(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSelectStation();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_SELECT_STATION };

	BYTE	mEnableStation;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual void OnCancel(){};

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nStation;
	int m_nStationBefore;
	bool m_bFixedStation;
	CButton m_btStation[eStation_MAX];

	void SetSelectStation(int nStation)
	{
		if (nStation >= eFixedSelectStation_Usuda64 && nStation <= eFixedSelectStation_Uchinoura20)
		{
			// �ǑI���A�ǌŒ�
			m_nStationBefore = nStation - eFixedSelectStation_Usuda64;
			m_bFixedStation = true;
		}
		else if (nStation >= eStation_Usuda64 && nStation <= eStation_Uchinoura20)
		{
			// �ǑI���A�ǔ�Œ�
			m_nStationBefore = nStation;
			m_bFixedStation = false;
		}
		else
		{
			// �ǖ��I���Ŕ�Œ肩�A����ȊO
			m_nStationBefore = mStationViewOrder[0];
			m_bFixedStation = false;
		}
	}

	BYTE ChgOrderEnebleStation(BYTE enableStation)
	{
		BYTE byteStation = 0;
		if (enableStation & (1 << eStation_Usuda64))
		{
			// �P�c64m
			byteStation |= (1 << eStationCnv_Usuda64);
		}
		if (enableStation & (1 << eStation_Usuda642))
		{
			// �P�c54m
			byteStation |= (1 << eStationCnv_Usuda642);
		}
		if (enableStation & (1 << eStation_Uchinoura34))
		{
			// ���V�Y34m
			byteStation |= (1 << eStationCnv_Uchinoura34);
		}
		if (enableStation & (1 << eStation_Uchinoura20))
		{
			// �P�c54m
			byteStation |= (1 << eStationCnv_Uchinoura20);
		}

		return byteStation;
	}

};
