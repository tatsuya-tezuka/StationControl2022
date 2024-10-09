#pragma once
#include "atltime.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "DateTimeCtrlNoToday.h"

// COperationPlanTimeSet �_�C�A���O

class COperationPlanTimeSet : public CDialogBase
{
	DECLARE_DYNAMIC(COperationPlanTimeSet)

public:
	COperationPlanTimeSet(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~COperationPlanTimeSet();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_OPERATIONPLAN_TIMESET };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
	enum {
		TYPE_UL,
		TYPE_RNG,
		TYPE_CMD,
		TYPE_TLMS,
		TYPE_TLMX,
		TYPE_TLMKa
	};

protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CTime m_sTime;
	CTime m_eTime;

	CTime m_Range1;
	CTime m_Range2;

	int m_OpeType;
	vector<CString> m_OpeTypeEle;

protected:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:
	void SetDefTime(CTime startTime, CTime endTime) { m_sTime = startTime; m_eTime = endTime; }
	void SetRangeTime(CTime range1, CTime range2) { m_Range1 = range1; m_Range2 = range2; }
	void SetParam(int opetype, vector<CString> ele, CString kind = _T("")) { m_OpeType = opetype; m_OpeTypeEle = ele; m_OpeTypeKind = kind; }

protected:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpeplnTimeRegBtn();
	// �J�n�����iDay�j
	CTime m_StartDay;
	// �J�n�����iTime�j
	CTime m_StartTime;
	// �I�������iDay�j
	CTime m_EndDay;
	// �I�������iTime�j
	CTime m_EndTime;
	virtual BOOL OnInitDialog();
	// �J�n�����R���g���[��
	CDateTimeCtrlNoToday m_StartDayCtrl;
	CDateTimeCtrl m_StartTimeCtrl;
	// �I�������R���g���[��
	CDateTimeCtrlNoToday m_EndDayCtrl;
	CDateTimeCtrl m_EndTimeCtrl;
	CStatic m_KindStaticCtrl;
	CComboBox m_KindCmbCtrl;
	CString m_OpeTypeKind;
};
