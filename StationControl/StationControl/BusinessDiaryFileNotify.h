#pragma once


// CBusinessDiaryFileNotify �_�C�A���O

class CBusinessDiaryFileNotify : public CDialogEx
{
	DECLARE_DYNAMIC(CBusinessDiaryFileNotify)

public:
	CBusinessDiaryFileNotify(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CBusinessDiaryFileNotify();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_DIARY_FILENOTIFY };

	CWinThread*		mpThread;

	bool	IsBusinessDiaryFile();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
