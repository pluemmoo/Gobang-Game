
// GobangDoc.cpp : implementation of the CGobangDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Gobang.h"
#endif

#include "GobangDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "OptionDialog.h"
#include "MainFrm.h"

// CGobangDoc

IMPLEMENT_DYNCREATE(CGobangDoc, CDocument)

BEGIN_MESSAGE_MAP(CGobangDoc, CDocument)
	ON_COMMAND(ID_FILE_LARGECHESSBOARD, &CGobangDoc::OnFileLargechessboard)
	ON_COMMAND(ID_FILE_SMALLCHESSBOARD, &CGobangDoc::OnFileSmallchessboard)
	ON_UPDATE_COMMAND_UI(ID_FILE_LARGECHESSBOARD, &CGobangDoc::OnUpdateFileLargechessboard)
	ON_UPDATE_COMMAND_UI(ID_FILE_SMALLCHESSBOARD, &CGobangDoc::OnUpdateFileSmallchessboard)
	ON_COMMAND(ID_FILE_OPTION, &CGobangDoc::OnFileOption)
	ON_COMMAND(ID_EDIT_UNDO32775, &CGobangDoc::OnEditUndo32775)
	ON_COMMAND(ID_EDIT_REDO32776, &CGobangDoc::OnEditRedo32776)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO32775, &CGobangDoc::OnUpdateEditUndo32775)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO32776, &CGobangDoc::OnUpdateEditRedo32776)
END_MESSAGE_MAP()


// CGobangDoc construction/destruction

CGobangDoc::CGobangDoc() noexcept
{
	// TODO: add one-time construction code here

}

CGobangDoc::~CGobangDoc()
{
}


BOOL CGobangDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	for (int i = 0; i < NUM_LINES; i++) {
		for (int j = 0; j < NUM_LINES; j++) {
			isPositionUsed[i][j] = 0;
		}
	}

	m_ellipses.clear();
	ellipseTemp.clear();
	gameOver = false;
	win = 0;
	playerturn = 2;
	colorNext = 0;

	return TRUE;
}


// CGobangDoc serialization

void CGobangDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << cellSize;
		ar << bgColor;

		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				ar << isPositionUsed[i][j];
			}
		}

		ar << m_ellipses.size();

		for (auto& ellipsedata : m_ellipses)
		{
			ellipsedata.Serialize(ar);
		}


		ar << gameOver;
		ar << chessSize;
		ar << playerturn;

		ar << win;
		ar << isDraw;
	}
	else
	{
		// TODO: add loading code here
		ar >> cellSize;
		ar >> bgColor;


		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				ar >> isPositionUsed[i][j];
			}
		}

		size_t count_ellipses;
		ar >> count_ellipses;
		m_ellipses.resize(count_ellipses);
		for (auto& ellipsedata : m_ellipses)
		{
			ellipsedata.Serialize(ar);
		}


		ar >> gameOver;
		ar >> chessSize;
		ar >> playerturn;

		ar >> win;
		ar >> isDraw;
	}
}

IMPLEMENT_SERIAL(EllipseData, CObject, 0)
void EllipseData::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar << color;
		ar << CorX_line;
		ar << CorY_line;
		ar << x; 
		ar << y;
	}
	else
	{
		// TODO: add loading code here
		ar >> color;
		ar >> CorX_line;
		ar >> CorY_line;
		ar >> x;
		ar >> y;
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGobangDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGobangDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGobangDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGobangDoc diagnostics

#ifdef _DEBUG
void CGobangDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGobangDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGobangDoc commands



void CGobangDoc::SetPosition(int player, int row, int col)
{
	isPositionUsed[row][col] = player;
}

bool CGobangDoc::GetPostion(int row, int col)
{
	if (isPositionUsed[row][col] == 1 || isPositionUsed[row][col] == 2) {
		return false;
	}
	else {
		return true;
	}
}

void CGobangDoc::SetPlayerTurn(int player)
{
	playerturn = player;
}

int CGobangDoc::GetPlayerTurn()
{
	return playerturn;
}

void CGobangDoc::Setm_ellipses(EllipseData& ellipseData)
{
	m_ellipses.push_back(ellipseData);
}

void CGobangDoc::IsWinningMove(int row, int col)
{
	// Check horinzontal
	int white_count = 0;
	int black_count = 0;
	for (int i = col - 4; i <= col + 4; i++)
	{
		if (i >= 0 && i < 15)
		{

			if (isPositionUsed[row][i] == white) {
				white_count++;
			}
			else {
				white_count = 0;
			}

			if (isPositionUsed[row][i] == black) {
				black_count++;
			}
			else {
				black_count = 0;
			}
		}

		if (white_count >= 5) {
			win = 1;
		}
		else if (black_count >= 5) {
			win = 2;
		}
	}

	//check vertical
	white_count = 0;
	black_count = 0;
	for (int i = row - 4; i <= row + 4; i++)
	{
		if (i >= 0 && i < 15)
		{

			if (isPositionUsed[i][col] == white)
				white_count++;
			else
				white_count = 0;

			if (isPositionUsed[i][col] == black)
				black_count++;
			else
				black_count = 0;

		}

		if (white_count >= 5) {
			win = 1;
		}
		else if (black_count >= 5) {
			win = 2;
		}
	}

	//Check diagonal (top-left to bottom-right)
	white_count = 0;
	black_count = 0;
	for (int i = -4; i <= 4; i++)
	{
		int r = row + i;
		int c = col + i;

		if (r >= 0 && r < 15 && c >= 0 && c < 15) {
			if (isPositionUsed[r][c] == white)
				white_count++;
			else
				white_count = 0;

			if (isPositionUsed[r][c] == black)
				black_count++;
			else
				black_count = 0;
		}

		if (white_count >= 5) {
			win = 1;
		}
		else if (black_count >= 5) {
			win = 2;
		}
	}

	//Check diagonal (top-right to bottom-left)
	white_count = 0;
	black_count = 0;
	for (int i = -4; i <= 4; i++)
	{
		int r = row + i;
		int c = col - i;

		if (r >= 0 && r < 15 && c >= 0 && c < 15) {
			if (isPositionUsed[r][c] == white)
				white_count++;
			else
				white_count = 0;

			if (isPositionUsed[r][c] == black)
				black_count++;
			else
				black_count = 0;
		}

		if (white_count >= 5) {
			win = 1;
		}
		else if (black_count >= 5) {
			win = 2;
		}
	}
}

void CGobangDoc::SetGameover(bool gameover)
{
	gameOver = gameover;
}

void CGobangDoc::SetRedo(bool status)
{
	redo = status;
}

void CGobangDoc::TempClear()
{
	ellipseTemp.clear();
}


void CGobangDoc::OnFileLargechessboard()
{
	// TODO: Add your command handler code here
	cellSize = 40;
	NUM_LINES = 15;
	chessSize = 15;

	for (int i = 0; i < m_ellipses.size(); i++) {
		m_ellipses[i].x = m_ellipses[i].CorX_line * cellSize + cellSize; // multiply cellSize to convert back to the actual pixel postion
		m_ellipses[i].y = m_ellipses[i].CorY_line * cellSize + cellSize; // then add 40 for obtainning the final result after removing the offset at first.

	}
	UpdateAllViews(NULL);
}


void CGobangDoc::OnFileSmallchessboard()
{
	// TODO: Add your command handler code here
	cellSize = 30;
	NUM_LINES = 15;
	chessSize = 12;

	if (cellSize == 30) {
		for (int i = 0; i < m_ellipses.size(); i++) {
			m_ellipses[i].x = m_ellipses[i].CorX_line * cellSize + cellSize; // multiply cellSize to convert back to the actual pixel postion
			m_ellipses[i].y = m_ellipses[i].CorY_line * cellSize + cellSize; // then add 40 for obtainning the final result after removing the offset at first.
		}
		UpdateAllViews(NULL);
	}
}


void CGobangDoc::OnUpdateFileLargechessboard(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(cellSize == 40);
}


void CGobangDoc::OnUpdateFileSmallchessboard(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(cellSize == 30);
}


void CGobangDoc::OnFileOption()
{
	// TODO: Add your command handler code here
	OptionDialog dlg;
	dlg.rd = colorNext;
	dlg.selectedColorIndex = currentbg;

	if (dlg.DoModal() == IDOK) {
		colorNext = dlg.rd;
		currentbg = dlg.selectedColorIndex;
		bgColor = dlg.selectedColor;
		dlg.selectedColor = bgColor;

		CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

		CStatusBar* pStatus = &pFrame->m_wndStatusBar;

		if (m_ellipses.empty()) {
			if (dlg.rd == 0) {
				playerturn = 2;

				pStatus->SetPaneText(2, _T("Next Color: BLACK"));
			}
			else {
				playerturn = 1;

				pStatus->SetPaneText(2, _T("Next Color: WHITE"));
			}
		}
		UpdateAllViews(NULL);
	}
}


void CGobangDoc::OnEditUndo32775()
{
	// TODO: Add your command handler code here
	ellipseTemp.push_back(m_ellipses.back());
	m_ellipses.pop_back();
	isPositionUsed[ellipseTemp.back().CorX_line][ellipseTemp.back().CorY_line] = NULL;
	if (playerturn == 1) {
		playerturn = 2;
	}
	else {
		playerturn = 1;
	}

	if (gameOver == true) {
		gameOver = false;
		win = 0;
	}

	redo = true;

	UpdateAllViews(NULL);
}


void CGobangDoc::OnEditRedo32776()
{
	// TODO: Add your command handler code here
	m_ellipses.push_back(ellipseTemp.back());
	if (playerturn == 2) {
		playerturn = 1;
		isPositionUsed[ellipseTemp.back().CorX_line][ellipseTemp.back().CorY_line] = 1;
	}
	else {
		playerturn = 2;
		isPositionUsed[ellipseTemp.back().CorX_line][ellipseTemp.back().CorY_line] = 2;
	}

	ellipseTemp.pop_back();

	UpdateAllViews(NULL);
}


void CGobangDoc::OnUpdateEditUndo32775(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_ellipses.size() != NULL);
}


void CGobangDoc::OnUpdateEditRedo32776(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(ellipseTemp.size() != NULL && redo == true);

}
