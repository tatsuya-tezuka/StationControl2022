#pragma once


// COperationPlanGraph

class COperationPlanGraph : public CStatic
{
	DECLARE_DYNAMIC(COperationPlanGraph)

public:
	COperationPlanGraph();
	virtual ~COperationPlanGraph();


	/* ------------------------------------------------------------------------------------ */
	/* ��`                                                                                 */
	/* ------------------------------------------------------------------------------------ */
public:
protected:

	/* ------------------------------------------------------------------------------------ */
	/* �����o�ϐ�                                                                           */
	/* ------------------------------------------------------------------------------------ */
public:
	CTime m_sTime;
	CTime m_eTime;

protected:

	//-----------------------------------------------------------------------------
	//	�����o�֐�
	//-----------------------------------------------------------------------------
public:

protected:
	void TimeToCoordinate(CString strSTime, CString strETime, int &x, int &w);
	void TimeToCoordinate(CTime tS, CTime tE, int &x, int &w);


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


