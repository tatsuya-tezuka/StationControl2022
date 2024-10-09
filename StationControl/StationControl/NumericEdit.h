#pragma once


// CNumericEdit

class CNumericEdit : public CEdit
{
	DECLARE_DYNAMIC(CNumericEdit)

public:
	CNumericEdit();
	virtual ~CNumericEdit();

	void SetRoundPlaceValue(const int RoundPlaceValue)
	{
		m_iRoundPlaceValue = RoundPlaceValue;
	}

	int GetRoundPlaceValue() const
	{
		return m_iRoundPlaceValue;
	}

protected:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	CString m_strDelim;
	int m_iRoundPlaceValue;

	DECLARE_MESSAGE_MAP()
};
