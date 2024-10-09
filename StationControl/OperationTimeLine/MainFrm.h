
// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once

#include <vector>
#include "../utility/StationPipe.h"
#include "MFCCaptionBarEx.h"

#include "MessageThread.h"

#include <mmsystem.h>

using namespace std;

enum eMessageTime{
	// �^�p�^�C�����C��
	eMessageTime_TimeLineHandle = (WM_USER + 6000),	// �E�B���h�E�n���h��
	eMessageTime_TimeLineFinish,					// �A�v���I��
	eMessageTime_TimeLinePlan,						// �v�旧��
	eMessageTime_TimeLineForcast,					// �\��l�ꗗ
	eMessageTime_TimeLineEmg,						// �A���e�i������~
	eMessageTime_TimeLineStop,						// ���M�@��~
};

class CMonitors
{
public:
	CMonitors();
	virtual ~CMonitors();

private:
	typedef struct{
		RECT			rect;
		HMONITOR		hMonitor;
		MONITORINFOEX	info;
	} mMonData;
	vector<mMonData> m_MonitorList;

	static BOOL CALLBACK MonitorNumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

public:
	//CMonitor GetMonitor(const int index) const;
	int		GetCount() const { return (int)m_MonitorList.size(); }
	void	AddMonitor(HMONITOR hMonitor, CRect rect, MONITORINFOEX	*info)
	{
		mMonData data;
		data.rect.left = rect.left;
		data.rect.top = rect.top;
		data.rect.right = rect.right;
		data.rect.bottom = rect.bottom;
		data.hMonitor = hMonitor;
		memcpy(&data.info, info, sizeof(MONITORINFOEX));
		m_MonitorList.push_back(data);
	}
	bool	GetMonitor(const UINT nMonNo, RECT *prect, MONITORINFOEX *pinfo)
	{
		vector<mMonData>::iterator itr;
		for (itr = m_MonitorList.begin(); itr != m_MonitorList.end(); itr++){
			if (nMonNo == 0 && (*itr).info.dwFlags == MONITORINFOF_PRIMARY){
				memcpy(prect, &((*itr).rect), sizeof(RECT));
				memcpy(pinfo, &((*itr).info), sizeof(MONITORINFOEX));
				return true;
			}
			if (nMonNo == 1 && (*itr).info.dwFlags != MONITORINFOF_PRIMARY){
				memcpy(prect, &((*itr).rect), sizeof(RECT));
				memcpy(pinfo, &((*itr).info), sizeof(MONITORINFOEX));
				return true;
			}
		}
		return false;
	}
};

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

protected:
	static CEngValCnv*	m_pEngval;
public:
	static CEngValCnv*	GetEngval();
	static void			SetEngval(CEngValCnv* eval);
	static void			DeleteEngval();

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum{
		eStatusSound_Play = 0x01,
		eStatusSound_Stop = 0x02,
		eStatusSound_Mute = 0x04,
	};
protected:
	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CShareMemory		mMem;
	bool				mCloseMessage;
protected:
	CMonitors	mMonitor;

	// ���v���Z�X�ԒʐM
	CStationPipe		mPipeSend;
	CStationPipe		mPipeRecv;
	bool				m_bCreatePipe;
	CStationPipe		mPipeServiceSend[eStation_MAX];
	CStationPipe		mPipeServiceRecv[eStation_MAX];
	bool				m_bCreatePipeService[eStation_MAX];

	CWinThread			*mReadThread;
	bool				mThreadEnd;
	CFont				mCaptionFont;
	UINT				m_nPlayStatusSound;
	MCIDEVICEID			mStatusSoundId;

	CMessageThread*		m_pMessageThread;
	/* ------------------------------------------------------------------------------------ */
	/* �����o�֐�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	bool	GetThreadEnd()	{ return mThreadEnd; }
	void	OnTileHorz();
	void	sendPipe(DWORD command, CString param=_T(""));
	void	SetStationMenuBar();
	void	PlayStatusSound(UINT status);
	void	StopStatusSound();
	void	MuteStatusSound();
	void	OnDirectPrint();
protected:
	// ���v���Z�X�ԒʐM
	static void WINAPI receiveServerCallBack(LPPIPEBUFFER pbuf);
	static void WINAPI receiveClientCallBack(LPPIPEBUFFER pbuf);
	static void WINAPI receiveServerServiceCallBack01(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack02(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack03(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack04(PIPEMONITORACK* pbuf);
	static void WINAPI receiveServerServiceCallBack(PIPEMONITORACK* pbuf);
	static void WINAPI receiveClientServiceCallBack(PIPEMONITORACK* pbuf);
	void	createPipe();
	void	deletePipe();
	void	SendServicePipe(DWORD command, int nStation);
	void	messagePipe(UINT message, WPARAM wParam, LPARAM lParam);

	void	CreateMessageThread();
	void	RemoveMessageThread();

	/* ------------------------------------------------------------------------------------ */

// ����
public:

// ����
public:

// �I�[�o�[���C�h
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ����
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �R���g���[�� �o�[�p�����o�[
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CMFCCaptionBarEx m_wndCaption;

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnWindowTileHorz();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnMsPlanning();
	afx_msg void OnTimeoffsetSet();
	afx_msg void OnOpStarttimeChg();
	afx_msg void OnOpEndtimeChg();
	afx_msg void OnForcedOpStop();
	afx_msg void OnMpPlanlist();
	afx_msg void OnMpForecastlist();
	afx_msg void OnMcCalibdiagns();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnViewPassstatus();
	afx_msg void OnUpdateViewPassstatus(CCmdUI *pCmdUI);
	afx_msg void OnViewEquipment();
	afx_msg void OnUpdateViewEquipment(CCmdUI *pCmdUI);
	afx_msg void OnViewEvent();
	afx_msg void OnUpdateViewEvent(CCmdUI *pCmdUI);
	afx_msg void OnViewLimiterror();
	afx_msg void OnUpdateViewLimiterror(CCmdUI *pCmdUI);
	afx_msg void OnHcopyNormal();
	afx_msg void OnUpdateHcopyNormal(CCmdUI *pCmdUI);
	afx_msg void OnHcopyReverse();
	afx_msg void OnUpdateHcopyReverse(CCmdUI *pCmdUI);
	afx_msg void OnMhManual();
	afx_msg void OnClose();
#ifdef MON_UT
	afx_msg void OnMonUt();
	afx_msg void OnUpdateMonUt(CCmdUI *pCmdUI);
#endif
};


