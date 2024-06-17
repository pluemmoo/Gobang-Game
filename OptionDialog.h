#pragma once
#include "afxdialogex.h"


// OptionDialog dialog

class OptionDialog : public CDialog
{
	DECLARE_DYNAMIC(OptionDialog)

public:
	OptionDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~OptionDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox COLORLIST;
	int rd;
	int selectedColorIndex;
	COLORREF selectedColor;


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
