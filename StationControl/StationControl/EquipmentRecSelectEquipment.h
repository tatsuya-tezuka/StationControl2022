#pragma once
#include "afxwin.h"
#include "EquipmentRecDumpOutput.h"

// CEquipmentRecSelectEquipment �_�C�A���O


#define COMMENT_DEV			_T("/* �_���v�Ώۑ��u�� */")
#define COMMENTSTART_TIME   _T("/* �_���v�J�n���� */")
#define COMMENTSTOP_TIME    _T("/* �_���v�I������ */")
#define COMMENTWATCH        _T("/* �_���v�ΏۊĎ����� */")

class CEquipmentRecSelectEquipment : public CDialogBase
{
	DECLARE_DYNAMIC(CEquipmentRecSelectEquipment)

public:
	CEquipmentRecSelectEquipment(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CEquipmentRecSelectEquipment();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_DLG_EQIPREC_SELEQIP };

	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	vector<CString> m_EquipNameList;
	map<CString, vector<CString>> m_EquipMonNameList;

protected:
	CWnd* m_pParent;



	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:

protected:
	CString GetTotalDay(CTime time);
	CString GetMonName(CString mon);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_EquipList;
	afx_msg void OnBnClickedEqiprecSeleqipOutputBtn();
};
