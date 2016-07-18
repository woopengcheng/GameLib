#pragma once
#include "Robot.h"

// CRobotBtn

class CRobotBtn : public CButton
{
	DECLARE_DYNAMIC(CRobotBtn)

public:
	CRobotBtn(Robot * pRobot);
	virtual ~CRobotBtn();

protected:
	virtual void				DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL				OnEraseBkgnd(CDC* pDC);
	afx_msg void				OnPaint();
	afx_msg HBRUSH				OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void				OnBnClicked();

	DECLARE_MESSAGE_MAP()
	
public: 

public:
	INT32						GetRobotIndex() const { return m_nRobotIndex; }
	void						SetRobotIndex(INT32 val) { m_nRobotIndex = val; }
	CRect						GetRectPos() const { return m_rectPos; }
	void						SetRectPos(CRect val) { m_rectPos = val; }
	void						SetBtnColor(COLORREF colorRef) { m_btnColor = colorRef; }

protected:
	CRect						m_rectPos;
	INT32						m_nRobotIndex;
	Robot				*		m_pRobot;
	COLORREF					m_btnColor;

};


