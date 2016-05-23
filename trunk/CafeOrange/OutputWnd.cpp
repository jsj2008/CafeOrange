
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "bgUtil.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// COutputBar
IMPLEMENT_DYNAMIC(COutputWnd, CDockablePane)
BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

void COutputWnd::DoDataExchange(CDataExchange* pDX)
{
	CDockablePane::DoDataExchange(pDX);
	DDX_Control(pDX, 3, TextureList);
}

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create tabs window:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("Failed to create output tab window\n");
		return -1;      // fail to create
	}

	// Create output panes:
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (
			!OutputList.Create(dwStyle, rectDummy, &m_wndTabs, OUTPUTWND_OUTPUT) ||
			!TextureList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_ICON, CRect(0, 0, 800, 500), &m_wndTabs, OUTPUTWND_TEXTURE) ||
			!m_wndOutputFind.Create(dwStyle, rectDummy, &m_wndTabs, 4) ||
			!PhtoFrameDlg.Create(IDD_PHOTO_DIALOG, &m_wndTabs) ||
			!VideoFrameDlg.Create(IDD_VIDEO_DIALOG, &m_wndTabs) 
		)
	{
		TRACE0("Failed to create output windows\n");
		return -1;      // fail to create
	}

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;
	BOOL bInitialized;

	// Attach list windows to tab:
	bNameValid = strTabName.LoadString(IDS_OUTPUT_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&OutputList, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_TEXTURES_TAB);
	ASSERT(bNameValid);
	bInitialized = TextureList.Initialize();
	ASSERT(bInitialized);
	m_wndTabs.AddTab(&TextureList, strTabName, (UINT)1);

	//photo
	m_wndTabs.AddTab(&PhtoFrameDlg, "photo", (UINT)2);
	//video 
	m_wndTabs.AddTab(&VideoFrameDlg, "video", (UINT)3);


	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// Tab control should cover the whole client area:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	
}


void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		//cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
		cxExtentMax = cxExtentMax > dc.GetTextExtent(strItem).cx ? cxExtentMax : dc.GetTextExtent(strItem).cx;
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::AddMessage(std::map<string, string>& messageMap)
{
	std::map<string, string>::iterator iMessage = messageMap.begin();
	for (; iMessage != messageMap.end(); iMessage++)
	{
		std::string outMessage = iMessage->first;
		outMessage += iMessage->second;

		FillBuildWindow(outMessage);
	}

}

void COutputWnd::FillBuildWindow(std::string& message)
{
	OutputList.AddString(message.c_str());	
}


void COutputWnd::UpdateFonts()
{
	OutputList.SetFont(&afxGlobalData.fontRegular);
	TextureList.SetFont(&afxGlobalData.fontRegular);
	m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList
BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()


COutputList::COutputList()
{

}

COutputList::~COutputList()
{

}

/////////////////////////////////////////////////////////////////////////////
// COutputList message handlers
void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("Copy output"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("Clear output"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();
	}
}

/////////////////////////////////////////////////////////////////////////////
// COutputList
BEGIN_MESSAGE_MAP(CTextureList, CListCtrl)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CTextureList::CTextureList()
{

}

CTextureList::~CTextureList()
{

}

bool CTextureList::Initialize()
{
	DeleteAllItems();
	LVCOLUMN column;
	column.mask = LVCF_WIDTH | LVCF_IMAGE;// | LVSCW_AUTOSIZE;
	column.cx = 200;
	column.pszText = "Image";
	column.iImage = 0;
	InsertColumn(0, &column);

	imgList.Create(100, 100, ILC_COLOR32, 8, 1);
	SetImageList(&imgList, LVSIL_NORMAL);
	
	CString TexturePath = CString("..\\Media\\Texture\\*.*");

	CFileFind textureFind;
	BOOL bFinded = textureFind.FindFile(TexturePath);	
	int index = 0;
	while (bFinded)
	{
		bFinded = textureFind.FindNextFile();
		if (textureFind.IsDirectory())
			continue;
		CString name = textureFind.GetFileName();
		if (name == _T(".") || name == _T(".."))
			continue;

		std::string extension = FindExtension(name.GetBuffer(0));
		if (SOIL_IMAGE_READ_UNSUPPORTED == TypeFromTextureExtension(extension))
			continue;

		CString title = textureFind.GetFileTitle();
		CString path = textureFind.GetFilePath();
		title.Append(".");
		title.Append(extension.c_str());
		
		CImage imageLoader;
		HRESULT res = imageLoader.Load(path);
		if (res != S_OK)
			continue;

		CDC *screenDC = GetDC();
		CDC *pMDC = new CDC;
		pMDC->CreateCompatibleDC(screenDC);

		CBitmap *pb = new CBitmap;
		pb->CreateCompatibleBitmap(screenDC, 100, 100);

		CBitmap *pob = pMDC->SelectObject(pb);
		pMDC->SetStretchBltMode(MAXSTRETCHBLTMODE); 
		imageLoader.StretchBlt(pMDC->m_hDC, 0, 0, 100, 100, 0, 0, imageLoader.GetWidth(), imageLoader.GetHeight(), SRCCOPY);
		pMDC->SelectObject(pob);

		CImage new_image;
		new_image.Attach((HBITMAP)(*pb));

		CBitmap* pBitmap = CBitmap::FromHandle(new_image);
		imgList.Add(pBitmap, RGB(0, 0, 0));
		
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE | LVIF_TEXT;		
		lvItem.iItem = index;
		lvItem.iSubItem = 0;
		lvItem.iImage = index; 
		lvItem.pszText = title.GetBuffer(MAX_PATH);

		InsertItem(&lvItem);
		
		index++;
		ReleaseDC(screenDC);
	
	}

	//Change Homer Simpson
	selectedItem = 0;
	//////////////////////////////////
	return true;
}

void CTextureList::OnLButtonDown(UINT nFlags, CPoint point)
{
	CListCtrl::OnLButtonDown(nFlags, point);
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos == NULL) return;

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	while (pos)
	{
			int nItem = GetNextSelectedItem(pos);
			if (nItem == selectedItem)
				return;
			selectedItem = nItem;
			CString filename = GetItemText(selectedItem, 0);
			pMainFrm->ChangeTexture(filename);

			//CString message("filename is ");
			//message += filename;
			//message += "\n";
			//OutputDebugString(message);
	}
}



