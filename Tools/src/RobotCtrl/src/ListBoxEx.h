#pragma once


// CListCtrlEx

class CListBoxEx : public CListBox
{
	DECLARE_DYNAMIC(CListBoxEx)

public:
	CListBoxEx();
	virtual ~CListBoxEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void		OnRButtonDown(UINT nFlags, CPoint point);
};


