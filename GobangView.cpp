// GobangView.cpp : implementation file
//

#include "pch.h"
#include "Gobang.h"
#include "GobangView.h"
#include "GobangDoc.h"
#include "MainFrm.h"


// GobangView

IMPLEMENT_DYNCREATE(GobangView, CScrollView)

GobangView::GobangView()
{

}

GobangView::~GobangView()
{
}


BEGIN_MESSAGE_MAP(GobangView, CScrollView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// GobangView drawing

void GobangView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 640;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

int NUM_LINES = 15;

void GobangView::OnDraw(CDC* pDC)
{
	CGobangDoc* pDoc = (CGobangDoc*)GetDocument();
	// TODO: add draw code here


	int cellSize = pDoc->cellSize;
	COLORREF bgColor = pDoc->bgColor;
	int chessSize = pDoc->chessSize;

	CRect rect;
	int maxWidth = GetSystemMetrics(SM_CXMAXTRACK);
	int maxHeight = GetSystemMetrics(SM_CYMAXTRACK);
	rect.SetRect(CPoint(0, 0), CPoint(maxWidth, maxHeight));
	pDC->FillSolidRect(&rect, bgColor); // Set background color


	// Draw the grid lines
	// NUM_LINES is the number of line in each row/column
	for (int i = 0; i < NUM_LINES; i++)
	{
		// Vertical lines
		pDC->MoveTo(i * cellSize + cellSize, cellSize);
		pDC->LineTo(i * cellSize + cellSize, cellSize * (NUM_LINES - 1) + cellSize);

		// Horizontal lines

		pDC->MoveTo(cellSize, i * cellSize + cellSize);
		pDC->LineTo(cellSize * (NUM_LINES - 1) + cellSize, i * cellSize + cellSize);
	}

	for (EllipseData& ellipseData : pDoc->m_ellipses)
	{
		CBrush brush(ellipseData.color);
		pDC->SelectObject(brush);

    		pDC->Ellipse(ellipseData.x - chessSize, ellipseData.y - chessSize,
			ellipseData.x + chessSize, ellipseData.y + chessSize);
	}
}


// GobangView diagnostics

#ifdef _DEBUG
void GobangView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void GobangView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// GobangView message handlers


void GobangView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	EllipseData ellipseData;

	CGobangDoc* pDoc = (CGobangDoc*)GetDocument();

	int cellSize = pDoc->cellSize;
	bool gameOver = pDoc->gameOver;
	int chessSize = pDoc->chessSize;


	ellipseData.CorX_line = (point.x - cellSize + cellSize / 2) / cellSize; //clickedPoint - offset + halfCellSize to adjust the position to the nearest grid line.
	ellipseData.CorY_line = (point.y - cellSize + cellSize / 2) / cellSize; // divided by cellSize  represents how many cellSize unit away from the grid origin which is 40.

	ellipseData.x = ellipseData.CorX_line * cellSize + cellSize; // multiply cellSize to convert back to the actual pixel postion
	ellipseData.y = ellipseData.CorY_line * cellSize + cellSize; // then add 40 for obtainning the final result after removing the offset at first.


	if (ellipseData.x >= cellSize && ellipseData.x <= cellSize * NUM_LINES && ellipseData.y >= cellSize
		&& ellipseData.y <= cellSize * NUM_LINES && gameOver == false) {
		if (pDoc->GetPostion(ellipseData.CorX_line, ellipseData.CorY_line)) {
			
			CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

			CStatusBar* pStatus = &pFrame->m_wndStatusBar;

			if (pDoc->GetPlayerTurn() == 2) {
				ellipseData.color = RGB(0, 0, 0); //black
				pDoc->SetPlayerTurn(1);
				pDoc->SetPosition(2, ellipseData.CorX_line, ellipseData.CorY_line);

				pStatus->SetPaneText(2, _T("Next Color: WHITE"));
			}
			else
			{
				ellipseData.color = RGB(255, 255, 255); //white
				pDoc->SetPlayerTurn(2);
				pDoc->SetPosition(1, ellipseData.CorX_line, ellipseData.CorY_line);

				pStatus->SetPaneText(2, _T("Next Color: BLACK"));
			}

			pDoc->Setm_ellipses(ellipseData);


			bool redo = pDoc->redo;

			if (redo == true) {
				pDoc->SetRedo(false);
				pDoc->TempClear();
			}
			Invalidate();
		}

	}

	pDoc->IsWinningMove(ellipseData.CorX_line, ellipseData.CorY_line);

	int win = pDoc->win;

	if (win == 2) {
		pDoc->SetGameover(true);
		MessageBox(_T("Black player wins!"), _T("Game Over"), MB_OK | MB_ICONINFORMATION);
	}
	else if (win == 1) {
		pDoc->SetGameover(true);
		MessageBox(_T("White player wins!"), _T("Game Over"), MB_OK | MB_ICONINFORMATION);
	}

	bool isDraw = pDoc->isDraw;
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (pDoc->GetPostion(i, j) == true) {
				isDraw = false;
				break;
			}
		}
		if (!isDraw)
			break;
	}

	if (isDraw) {
		pDoc->SetGameover(true);
		MessageBox(_T("The game ended in a draw!"), _T("Game Over"), MB_OK | MB_ICONINFORMATION);
	}
	CScrollView::OnLButtonDown(nFlags, point);
}
