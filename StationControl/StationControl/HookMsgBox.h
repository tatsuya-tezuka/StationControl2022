#include <Windows.h>
#include <afxwin.h >


//////////////////////////////////////////////////////////////////
// �t�b�N�n���h���p�̕ϐ����O���[�o���Ő錾����B
static HHOOK MyHookHandle;	// �t�b�N�n���h���ϐ�

// OK�̕�����ƁA�L�����Z���̕�����
// �{�^���̕�����ύX�������ꍇ�͂�����ύX���Ă�������
static LPCWSTR strOK = L"���s";				// OK�{�^���̕���
static LPCWSTR strCancel = L"�L�����Z��";	// �L�����Z���{�^���̕���

// �v���g�^�C�v�錾
static LRESULT CALLBACK MsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam);
static int AfxMessageBoxHooked(LPCSTR message, UINT nType);



////////////////////////////////////////////////////////////////////
//
//		���b�Z�[�W�{�b�N�X���������̂��߂�
//		�t�b�N�֐��i���[�J���t�b�N�p�j
//		�� �O���[�o���֐��ł���K�v������܂��B
//		���̊֐��̒���ς���Ƃ��͐T�d�ɍs���ĉ������B
//
static LRESULT CALLBACK MsgBoxHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// �R�[�h���f
	if (nCode >= 0)
	{
		if (nCode == HCBT_ACTIVATE)
		{
			// OK�{�^��(IDOK)�̓��e������������
			SetDlgItemText((HWND)wParam, IDOK, strOK);
			// �L�����Z���{�^��(IDCANCEL)�̓��e������������
			SetDlgItemText((HWND)wParam, IDCANCEL, strCancel);
			BOOL ret;
			// �t�b�N�֐����A���C���X�g�[��(�t�b�N�����I�j
			ret = UnhookWindowsHookEx(MyHookHandle);

		}
	}
	// ���̃t�b�N�ɓn��
	return CallNextHookEx(MyHookHandle, nCode, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////
//
//		AfxMessageBoxHooked
//
static int AfxMessageBoxHooked(LPCTSTR message, UINT nType)
{
	// �t�b�N�֐�(MsgBoxHookProc)���C���X�g�[������ SetWindowHookEx
	MyHookHandle = SetWindowsHookEx(WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId());
	return (AfxMessageBox(message, nType));
}

/////////////////////////////////////////////////////////////////////
//
//		MessageBoxHooked
//
static int MessageBoxHooked(HWND handle, LPCTSTR message, LPCTSTR title, UINT nType)
{
	// �t�b�N�֐�(MsgBoxHookProc)���C���X�g�[������ SetWindowHookEx
	MyHookHandle = SetWindowsHookEx(WH_CBT, MsgBoxHookProc, NULL, GetCurrentThreadId());
	return (MessageBox(handle, message, title, nType));
}

