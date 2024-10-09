#pragma once
#include "afxwin.h"


// CSelectStation ダイアログ

class CSelectStation : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectStation)

public:
	CSelectStation(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CSelectStation();

// ダイアログ データ
	enum { IDD = IDD_DIALOG_SELECT_STATION };

	BYTE	mEnableStation;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
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
			// 局選択、局固定
			m_nStationBefore = nStation - eFixedSelectStation_Usuda64;
			m_bFixedStation = true;
		}
		else if (nStation >= eStation_Usuda64 && nStation <= eStation_Uchinoura20)
		{
			// 局選択、局非固定
			m_nStationBefore = nStation;
			m_bFixedStation = false;
		}
		else
		{
			// 局未選択で非固定か、それ以外
			m_nStationBefore = mStationViewOrder[0];
			m_bFixedStation = false;
		}
	}

	BYTE ChgOrderEnebleStation(BYTE enableStation)
	{
		BYTE byteStation = 0;
		if (enableStation & (1 << eStation_Usuda64))
		{
			// 臼田64m
			byteStation |= (1 << eStationCnv_Usuda64);
		}
		if (enableStation & (1 << eStation_Usuda642))
		{
			// 臼田54m
			byteStation |= (1 << eStationCnv_Usuda642);
		}
		if (enableStation & (1 << eStation_Uchinoura34))
		{
			// 内之浦34m
			byteStation |= (1 << eStationCnv_Uchinoura34);
		}
		if (enableStation & (1 << eStation_Uchinoura20))
		{
			// 臼田54m
			byteStation |= (1 << eStationCnv_Uchinoura20);
		}

		return byteStation;
	}

};
