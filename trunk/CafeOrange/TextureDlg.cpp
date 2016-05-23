
#include "stdafx.h"
#include "CafeOrange.h"
#include "TextureDlg.h"
#include "afxdialogex.h"
#include "bgMaterial.h"
#include "Message.h"
#include "MainFrm.h"

const LPCTSTR ColumnHeader[] = 
{
	_T("Stage"), _T("File"), _T("Operation"), _T("Filter"),	_T("Address"), _T("Size")
};

const LPCTSTR TextureOperation[] = 
{
	_T("Modulate"), _T("Add"), _T("Darken"), _T("Lighten"), _T("Subtract"),_T("Difference") 
};

const LPCTSTR TextureFilter[] = 
{
	_T("Nearest"), _T("Linear"), _T("Linear MipMap"),	_T("Anisotropic")
};

const LPCTSTR TextureAddress[] = 
{
	_T("Repeat"), _T("Clamp"), _T("Mirrored Repeat"),	_T("Clamp Edge")
};

BEGIN_MESSAGE_MAP(CTextureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_OPERATION_COMBO, OnChangeOperation)
	ON_BN_CLICKED(IDC_TEX_ADD,		OnAddTexture)
	ON_BN_CLICKED(IDC_TEX_DELETE, OnDeleteTexture)
END_MESSAGE_MAP()


CTextureDlg::CTextureDlg()
{

}

CTextureDlg::~CTextureDlg()
{
}

void CTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TEXTURE_LIST,	TextureList);
	//DDX_Control(pDX, IDC_FILTER_COMBO, FilterComboBox);
	DDX_Control(pDX, IDC_ADDRESS_COMBO,		AddressComboBox);
	DDX_Control(pDX, IDC_OPERATION_COMBO,	OperationComboBox);
	DDX_Control(pDX, IDC_TEX_ADD,		AddBtn);
	DDX_Control(pDX, IDC_TEX_DELETE,  DeleteBtn);
}


BOOL CTextureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 255, 255));
	DefineListHeader();

//	for(int i=0; i < 4; i++)
//		FilterComboBox.AddString(TextureFilter[i]);

	for(int i=0; i < 6; i++)
		OperationComboBox.AddString(TextureOperation[i]);

	for(int i=0; i < 4; i++)
		AddressComboBox.AddString(TextureAddress[i]);
	
//	FilterComboBox.SetCurSel(0);
	OperationComboBox.SetCurSel(0);
	AddressComboBox.SetCurSel(0);

	TextureList.SetExtendedStyle(TextureList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

	return TRUE;  
}

void CTextureDlg::DefineListHeader()
{
	TextureList.InsertColumn((int)TEXTURE_LIST_INDEX,ColumnHeader[TEXTURE_LIST_INDEX],LVCFMT_CENTER,40);
	TextureList.InsertColumn((int)TEXTURE_LIST_FILE,ColumnHeader[TEXTURE_LIST_FILE],LVCFMT_CENTER,100);
	TextureList.InsertColumn((int)TEXTURE_LIST_OPERATION,ColumnHeader[TEXTURE_LIST_OPERATION],LVCFMT_CENTER,80);
	TextureList.InsertColumn((int)TEXTURE_LIST_ADDRESS,ColumnHeader[TEXTURE_LIST_ADDRESS],LVCFMT_CENTER,80);
}

void CTextureDlg::OnPaint()
{
	CPaintDC dc(this); 
}


void CTextureDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

}

int	CTextureDlg::GetSelectedItemIndex()
{
	POSITION pos = TextureList.GetFirstSelectedItemPosition();
	if (pos == NULL) return -1;

	return TextureList.GetNextSelectedItem(pos);
}

void CTextureDlg::AddItem(int Row, int Column, CString& Message)
{
	LVITEM lv;
	lv.iItem		= Row;
	lv.iSubItem = Column;
	lv.pszText = Message.GetBuffer(MAX_PATH);
	lv.mask = LVIF_TEXT;
	if (Column == 0)
		TextureList.InsertItem(&lv);
	else
		TextureList.SetItem(&lv);
}

void CTextureDlg::OnAddTexture()
{
	CFileDialog FileDialog(TRUE, NULL, NULL, OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST, STRING_IMPORT_IMG_FILTER);
	if (FileDialog.DoModal() != IDOK){
		return;
	}

	CString FileExt	 = FileDialog.GetFileExt();
	CString FilePath = FileDialog.GetPathName();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);		
	pMainFrm->AddTexture(FilePath);

	CString FileName = FileDialog.GetFileName();
	CString message;	
	int ItemCount = TextureList.GetItemCount();
	message.Format("%d", ItemCount+1);
	AddItem(ItemCount, (int)TEXTURE_LIST_INDEX, message);
	AddItem(ItemCount, (int)TEXTURE_LIST_FILE, FileName);
	AddItem(ItemCount, (int)TEXTURE_LIST_OPERATION, CString(TextureOperation[0]));
	AddItem(ItemCount, (int)TEXTURE_LIST_ADDRESS, CString(TextureAddress[1]));

}


void CTextureDlg::OnDeleteTexture()
{
	int selectedIndex = GetSelectedItemIndex();
	if (selectedIndex == -1) return;

	CString filename = TextureList.GetItemText(selectedIndex, (int)TEXTURE_LIST_FILE);
	CString stage = TextureList.GetItemText(selectedIndex, (int)TEXTURE_LIST_INDEX);
	int stageIndex = _ttoi(stage);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);	
	pMainFrm->DeleteTexture(stageIndex, filename);

	TextureList.DeleteItem(selectedIndex);	
}

void CTextureDlg::OnChangeOperation()
{
	int selectedIndex = GetSelectedItemIndex();
	if (selectedIndex == -1) return;
	CString stage = TextureList.GetItemText(selectedIndex, (int)TEXTURE_LIST_INDEX);
	int stageIndex = _ttoi(stage);

	int operation = OperationComboBox.GetCurSel();
	eTextureOperation  texOperation = eTextureOperation(operation);

	TextureList.SetItemText(selectedIndex, (int)TEXTURE_LIST_OPERATION, TextureOperation[operation]);
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);
	pMainFrm->ChangeTextureOpeation(stageIndex, texOperation);
	
}