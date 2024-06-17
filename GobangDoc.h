
// GobangDoc.h : interface of the CGobangDoc class
//


#pragma once
#include <vector>
class EllipseData;

class CGobangDoc : public CDocument
{
protected: // create from serialization only
	CGobangDoc() noexcept;
	DECLARE_DYNCREATE(CGobangDoc)

// Attributes
public:
	COLORREF bgColor = RGB(50, 205, 50);
	std::vector<EllipseData> m_ellipses;
	std::vector<EllipseData> ellipseTemp;
	int isPositionUsed[15][15];
	const int white = 1; // 1 = white's turn, 2 = black's turn
	const int black = 2;
	int playerturn = 2; //First player

	bool gameOver = false;
	int win = 0;
	bool isDraw = true;

	int cellSize = 40;
	int NUM_LINES = 15;
	int chessSize = 15;


	bool redo = true;

	int colorNext = 0;
	int currentbg = 2;
// Operations
public:
	void SetPosition(int player, int row, int col);
	bool GetPostion(int row, int col);
	void SetPlayerTurn(int player);
	int GetPlayerTurn();
	void Setm_ellipses(EllipseData& ellipseData);
	void IsWinningMove(int row, int col);
	void SetGameover(bool gameover);
	void SetRedo(bool status);

	void TempClear();
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGobangDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileLargechessboard();
	afx_msg void OnFileSmallchessboard();
	afx_msg void OnUpdateFileLargechessboard(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSmallchessboard(CCmdUI* pCmdUI);
	afx_msg void OnFileOption();
	afx_msg void OnEditUndo32775();
	afx_msg void OnEditRedo32776();
	afx_msg void OnUpdateEditUndo32775(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo32776(CCmdUI* pCmdUI);
};


class EllipseData : CObject {
	DECLARE_SERIAL(EllipseData)
public:
	COLORREF color = 0;
	int CorX_line = -1;
	int CorY_line = -1;
	int x = 0;
	int y = 0;
	EllipseData() {}

	EllipseData(const EllipseData& other)
	{
		color = other.color;
		CorX_line = other.CorX_line;
		CorY_line = other.CorY_line;
		x = other.x;
		y = other.y;
	}

	virtual void Serialize(CArchive& ar);
};

