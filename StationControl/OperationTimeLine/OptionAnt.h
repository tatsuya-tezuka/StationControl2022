#pragma once


// COptionAnt �_�C�A���O

class COptionAnt : public CDialogEx
{
	DECLARE_DYNAMIC(COptionAnt)

public:
	COptionAnt(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COptionAnt();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DIALOG_OPTIONANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	int m_nAntMode;
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
