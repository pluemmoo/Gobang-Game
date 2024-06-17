#pragma once



// GobangView view

class GobangView : public CScrollView
{
	DECLARE_DYNCREATE(GobangView)

protected:
	GobangView();           // protected constructor used by dynamic creation
	virtual ~GobangView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


