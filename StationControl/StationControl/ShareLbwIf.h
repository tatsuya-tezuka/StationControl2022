#pragma once

#include <vector>
using namespace std;

class CShareLbwIf
{
public:
	CShareLbwIf();
	~CShareLbwIf();


	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mItemMax = 6;

	typedef struct{
		char	satellite[64];	// �q����
		char	ctrl[64];		// ���䖼
		char	value[64];		// �l
	} stItem;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
protected:
	bool	mServer;
	DWORD	m_dwProcessId;
	HANDLE	m_hMap;
	HANDLE	m_hMutex;
	HANDLE	m_hSend;
	HANDLE	m_hRecv;
	char	*m_pBuf;

	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	Create(bool bServer);
	void	Delete();
	void	Send(stItem* stBuf, UINT count);
	bool	Recv(stItem* stBuf, UINT count);

protected:

};

