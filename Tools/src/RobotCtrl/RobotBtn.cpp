// RobotButton.cpp : 实现文件
//

#include "stdafx.h"  

#include "RobotBtn.h"


// CRobotButton

IMPLEMENT_DYNAMIC(CRobotBtn, CButton)

CRobotBtn::CRobotBtn(Robot * pRobot)
	: m_pRobot(pRobot)
	, m_btnColor(RGB(0,255,0))
	, m_nRobotIndex(-1)
{ 
}

CRobotBtn::~CRobotBtn()
{
}


BEGIN_MESSAGE_MAP(CRobotBtn, CButton) 
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_CONTROL_REFLECT(BN_CLICKED, &CRobotBtn::OnBnClicked)
END_MESSAGE_MAP()



// CRobotButton 消息处理程序
  
void CRobotBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
}

BOOL CRobotBtn::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

//	return TRUE;
	return CButton::OnEraseBkgnd(pDC);
}

void CRobotBtn::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CButton::OnPaint()

	CRect rect;
	GetClientRect(rect);

	CBrush brush(m_btnColor); 
	dc.FillRect(&rect , &brush);

	CFont * pFont = new CFont;
	pFont->CreatePointFont(70 , _T("微软雅黑"));
	dc.SelectObject(pFont);

	dc.SetBkMode(TRANSPARENT);
 
	CBrush brushPen(RGB(0,0,0)); 
	dc.SelectObject(&brushPen);

	CString str;
	if (m_pRobot)
	{
		str.Format(L"%s", m_pRobot->GetName());
	}
	else
	{
		str = L"None";
	}
	dc.TextOut(0,0, str);


	SAFE_DELETE(pFont);

}

HBRUSH CRobotBtn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CRobotBtn::OnBnClicked()
{ 

// 	CollectionRobotInforT::iterator iter = g_RobotData.m_mapRobotInforMgr.find(m_nRobotIndex);
// 
// 	if (iter != g_RobotData.m_mapRobotInforMgr.end())
// 	{
// 		CString str;
// 		str.Format("BaseMapID: %lld \nGameMapID:%lld \n", (iter->second).info.nBaseMapID, (iter->second).info.nGameMapID);
// 		MessageBox(str);

		CString str;
		str.Format(L"BaseMapID:  \nGameMapID: \n");
		MessageBox(str);
//	} 

	// TODO: 在此添加控件通知处理程序代码
}
