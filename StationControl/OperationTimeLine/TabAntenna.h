#pragma once


// CTabAntenna �_�C�A���O

class CTabAntenna : public CDialogEx
{
	DECLARE_DYNCREATE(CTabAntenna)

public:
	CTabAntenna(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CTabAntenna();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_TAB_ANTENNA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
};
