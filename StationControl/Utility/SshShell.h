#pragma once
class CSshShell : public CWnd
{
	DECLARE_DYNAMIC(CSshShell)

public:
	CSshShell();
	~CSshShell();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	// SSH�\����
	typedef struct
	{
		CString strBatchPath;			// �o�b�`�t�@�C���p�X
		CString strUserName;			// SSH���O�C�����[�U��
		CString strServerName;			// SSH�ڑ���T�[�o���܂���IP�A�h���X
		CString strKeyFilePath;			// �閧���t�@�C���p�X
		CString strRemShellName;		// ���s���郊���[�g�V�F����
		CString strRemShellParam;		// ���s���郊���[�g�V�F���R�}���h�ւ̈����B�����𕡐��L�q����ꍇ�A"�ł�����
	} structSshParam;

protected:
	
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	structSshParam	m_stSshParam;
//	deque<CWinThread*> threadList;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	static int ExecShell(structSshParam& sshParam, BOOL bWaitComplete = TRUE);
#if 0
	int ExecShell2(structSshParam& sshParam);

	void ExecShellThread(structSshParam& stParam);

	void SetParam(structSshParam param)		{ m_stSshParam = param; }

	static UINT ThreadProc(LPVOID pParam);
	void ThreadProc();
#endif

protected:
	DECLARE_MESSAGE_MAP()
//	afx_msg LRESULT OnCompleteSsh(WPARAM wParam, LPARAM lParam);
	/* ------------------------------------------------------------------------------------ */
};

