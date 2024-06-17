// OptionDialog.cpp : implementation file
//

#include "pch.h"
#include "Gobang.h"
#include "afxdialogex.h"
#include "OptionDialog.h"


// OptionDialog dialog

IMPLEMENT_DYNAMIC(OptionDialog, CDialog)

OptionDialog::OptionDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
	, rd(0)
{

}

OptionDialog::~OptionDialog()
{
}

void OptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, COLORLIST);
	DDX_Radio(pDX, IDC_RADIO1, rd);
}


BEGIN_MESSAGE_MAP(OptionDialog, CDialog)
	ON_BN_CLICKED(IDOK, &OptionDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// OptionDialog message handlers


BOOL OptionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int index = COLORLIST.AddString(_T("White"));
	COLORLIST.SetItemData(index, RGB(255, 255, 255));
	index = COLORLIST.AddString(_T("Gray"));
	COLORLIST.SetItemData(index, RGB(203, 203, 203));
	index = COLORLIST.AddString(_T("Red"));
	COLORLIST.SetItemData(index, RGB(255, 0, 0));
	index = COLORLIST.AddString(_T("Yellow"));
	COLORLIST.SetItemData(index, RGB(245, 255, 32));
	index = COLORLIST.AddString(_T("Blue"));
	COLORLIST.SetItemData(index, RGB(0, 17, 255));
	index = COLORLIST.AddString(_T("Green"));
	COLORLIST.SetItemData(index, RGB(50, 205, 50));
	if (selectedColorIndex >= 0 && selectedColorIndex < COLORLIST.GetCount())
	{
		COLORLIST.SetCurSel(selectedColorIndex);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void OptionDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	selectedColorIndex = COLORLIST.GetCurSel();

	if (selectedColorIndex != LB_ERR) {
		selectedColor = COLORLIST.GetItemData(selectedColorIndex);
	}

	CDialog::OnOK();
}
