#pragma once

#include <vector>
using namespace std;

class CShareLbwIf
{
public:
	CShareLbwIf();
	~CShareLbwIf();


	/* ------------------------------------------------------------------------------------ */
	/* 定義                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	static const UINT mItemMax = 6;

	typedef struct{
		char	satellite[64];	// 衛星名
		char	ctrl[64];		// 制御名
		char	value[64];		// 値
	} stItem;

protected:

	/* ------------------------------------------------------------------------------------ */
	/* メンバ変数                                                                           */
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
	/* メンバ関数                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	Create(bool bServer);
	void	Delete();
	void	Send(stItem* stBuf, UINT count);
	bool	Recv(stItem* stBuf, UINT count);

protected:

};

