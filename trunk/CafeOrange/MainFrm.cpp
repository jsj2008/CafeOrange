#include "stdafx.h"
#include "DefineEvent.h"

#include "Action.h"
#include "MovieAction.h"
#include "Shader.h"
#include "Object.h"

#include "CafeOrange.h"
#include "bgSceneObserver.h"

#include "bgGizmo.h"
#include "bgMovieAlgorithm.h"
#include "MainFrm.h"
#include <string>

#define OBJECT_TREE_ID  1200
#define SHADER_TREE_ID 1300

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)
BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()

	ON_NOTIFY(TVN_SELCHANGED, OBJECT_TREE_ID, OnChangeObject)
	ON_NOTIFY(TVN_SELCHANGED, SHADER_TREE_ID, OnChangeShader)

	ON_MESSAGE(WM_SCENE_INFORMATION, GetInformation)
	ON_MESSAGE(WM_ADD_ACTOR,				   GetInformation)
END_MESSAGE_MAP()

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//http://stackoverflow.com/questions/13725519/how-to-convert-bstr-to-stdstring-in-visual-studio-c-2010
// convert a BSTR to a std::string. 
 std::string& BstrToStdString(const BSTR bstr, std::string& dst, int cp = CP_UTF8)
{
	if (!bstr)
	{
		// define NULL functionality. I just clear the target.
		dst.clear();
		return dst;
	}

	// request content length in single-chars through a terminating
	//  nullchar in the BSTR. note: BSTR's support imbedded nullchars,
	//  so this will only convert through the first nullchar.
	int res = WideCharToMultiByte(cp, 0, bstr, -1, NULL, 0, NULL, NULL);
	if (res > 0)
	{
		dst.resize(res);
		WideCharToMultiByte(cp, 0, bstr, -1, &dst[0], res, NULL, NULL);
	}
	else
	{    // no content. clear target
		dst.clear();
	}
	return dst;
}

// conversion with temp.
std::string BstrToStdString(BSTR bstr, int cp = CP_UTF8)
{
	std::string str;
	BstrToStdString(bstr, str, cp);
	return str;
}

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);

	pObjectTreeAction =  0;
	pSceneTreeAction = 0;
	pDataPropertyAction		=0;
	pMaterialPropertyAction	=0;
	pGizmoTreeAction = 0;
	pMovieDlgAction = 0; 
	pChangeTextureAction = 0;
}
CMainFrame::~CMainFrame()
{
	delete pObjectTreeAction;
	delete pSceneTreeAction;
	delete pGizmoTreeAction;
	delete pMovieDlgAction;
	delete pChangeTextureAction;
}

void	CMainFrame::InitializeMDIView(CView* pOpenCVView, CView* pImgEditorView)
{
	MDITabNewGroup(FALSE);

	pMovieView		= pOpenCVView;
	pImageEditorView = pImgEditorView;

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Navigation pane will be created at left, so temporary disable docking at the left side:
	EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM | CBRS_ALIGN_RIGHT);

	// Create and setup "Outlook" navigation bar:
	if (!CreateOutlookBar(NavigationBar, ID_VIEW_NAVIGATION,ObjectTree, ShaderTree, 250))
	{
		TRACE0("Failed to create navigation pane\n");
		return -1;      // fail to create
	}

	// Outlook bar is created and docking on the left side should be allowed.
	//EnableDocking(CBRS_ALIGN_LEFT);
	//EnableAutoHidePanes(CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_RIGHT);
	EnableAutoHidePanes(CBRS_ALIGN_LEFT);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	DataPropertyGrid.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&DataPropertyGrid);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	pObjectTreeAction		= new cObjectTreeAction(&ObjectTree);
	pSceneTreeAction			= new cSceneTreeAction(&ShaderTree);
	pDataPropertyAction		= new cDataPropertyAction(&DataPropertyGrid); 	
	pMaterialPropertyAction	= new cMaterialPropertyAction(&MaterialPropertyGrid);
	pGizmoTreeAction			= new cGizmoTreeAction(&ObjectTree);
	pMovieDlgAction				= new cMovieDlgAction(&m_wndOutput.VideoFrameDlg);
	
	pSceneTreeAction->InitializeUI();
	pMaterialPropertyAction->InitializeUI();
	pObjectTreeAction->InitializeUI();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!DataPropertyGrid.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI)){
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	DataPropertyGrid.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

BOOL CMainFrame::CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID,CTreeCtrl& ObjectTree,CTreeCtrl& ShaderTree,int nInitialWidth)
{
	bar.SetMode2003();

	BOOL bNameValid;
	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_SHORTCUTS);
	ASSERT(bNameValid);
	if (!bar.Create(strTemp, this, CRect(0, 0, nInitialWidth, 32000), uiID, WS_CHILD | WS_VISIBLE | CBRS_RIGHT)){ //CBRS_LEFT
		return FALSE; // fail to create
	}

	CMFCOutlookBarTabCtrl* pOutlookBar = (CMFCOutlookBarTabCtrl*)bar.GetUnderlyingWindow();

	if (pOutlookBar == NULL){
		ASSERT(FALSE);
		return FALSE;
	}

	pOutlookBar->EnableInPlaceEdit(TRUE);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static UINT uiPageID = 1;
	// can float, can autohide, can resize, CAN NOT CLOSE
	DWORD dwStyle = AFX_CBRS_FLOAT | AFX_CBRS_AUTOHIDE | AFX_CBRS_RESIZE;
	CRect rectDummy(0, 0, 0, 0);
	const DWORD dwTreeStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |TVS_SHOWSELALWAYS;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ObjectTree.Create(dwTreeStyle, rectDummy, &bar, OBJECT_TREE_ID);
	pOutlookBar->AddControl(&ObjectTree, "Project", 1, TRUE, dwStyle);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	ShaderTree.Create(dwTreeStyle, rectDummy, &bar, SHADER_TREE_ID);
	//bNameValid = strTemp.LoadString(IDS_FOLDERS);
	//ASSERT(bNameValid);
	pOutlookBar->AddControl(&ShaderTree, "Scene", 2, TRUE, dwStyle);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!MaterialPropertyGrid.Create(STRING_MATERIAL_PROPERTY, &bar, rectDummy, TRUE, 
									 ID_MATERIAL_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}
	pOutlookBar->AddControl(&MaterialPropertyGrid, STRING_MATERIAL_PROPERTY, 3, TRUE, dwStyle);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TextureDlg.Create(IDD_TEXTURE_DIALOG, pOutlookBar);
	TextureDlg.ShowWindow(SW_SHOW);
	pOutlookBar->AddControl(&TextureDlg, STRING_TEXTURE_NAME, 3, TRUE);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bar.SetPaneStyle(bar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	pOutlookBar->SetImageList(theApp.m_bHiColorIcons ? IDB_PAGES_HC : IDB_PAGES, 24);
	pOutlookBar->SetToolbarImageList(theApp.m_bHiColorIcons ? IDB_PAGES_SMALL_HC : IDB_PAGES_SMALL, 16);
	pOutlookBar->RecalcLayout();

	BOOL bAnimation = theApp.GetInt(_T("OutlookAnimation"), TRUE);
	CMFCOutlookBarTabCtrl::EnableAnimation(bAnimation);

	bar.SetButtonsFont(&afxGlobalData.fontBold);

	return TRUE;
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CMainFrame::SetCapture(int frame)
{	
	pMovieView->SendMessage(WM_REQUEST_CAPTURE_MOVIEFRAME, WM_REQUEST_CAPTURE_MOVIEFRAME, frame);
}
void	CMainFrame::AddMovieEffect(eMovieEffect effect)
{
	pMovieView->SendMessage(WM_REQUEST_ADD_MOVIE_EFFECT, WM_REQUEST_ADD_MOVIE_EFFECT, (int)effect);
}
void CMainFrame::DeleteMovieEffect(int algorithmIndex)
{
	pMovieView->SendMessage(WM_REQUEST_DELETE_MOVIE_EFFECT, WM_REQUEST_DELETE_MOVIE_EFFECT, algorithmIndex);
}
void CMainFrame::SetMovieEffect(int algorithmIndex)
{
	pDataPropertyAction->InsertMovieEffect(algorithmIndex);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////ImageEditorView
void	 CMainFrame::AddPhotoEffect(ePhotoEffect effect)
{
	pImageEditorView->SendMessage(WM_REQUEST_ADD_PHOTO_EFFECT, WM_REQUEST_ADD_PHOTO_EFFECT, (int)effect);
}
void	CMainFrame::DeletePhotoEffect(int  algorithmIndex)
{
	pImageEditorView->SendMessage(WM_REQUEST_DELETE_PHOTO_EFFECT, WM_REQUEST_DELETE_PHOTO_EFFECT, algorithmIndex);
}
void CMainFrame::SetPhotoEffect(int algorithmIndex)
{	
	pDataPropertyAction->InsertPhotoEffect(algorithmIndex);
}

void	CMainFrame::ChangeTexture(CString& filename)
{
	pImageEditorView->SendMessage(WM_REQUEST_CHANGE_TEXTURE, WM_REQUEST_CHANGE_TEXTURE, reinterpret_cast<LPARAM>(filename.GetBuffer(MAX_PATH)));
}
void	CMainFrame::AddTexture(CString& filename)
{
	pImageEditorView->SendMessage(WM_REQUEST_ADD_TEXTURE, WM_REQUEST_ADD_TEXTURE, reinterpret_cast<LPARAM>(filename.GetBuffer(MAX_PATH)));
}
void	CMainFrame::ChangeTextureOpeation(int stageIndex, eTextureOperation operation)
{
	IndexIntParam param;
	param.intValue1 = stageIndex;
	param.intValue2 = (int)operation;
	pImageEditorView->SendMessage(WM_REQUEST_TEXTURE_OPERATION, WM_REQUEST_TEXTURE_OPERATION, reinterpret_cast<LPARAM>(&param));
}
void	CMainFrame::DeleteTexture(int index, CString& filename)
{
	IndexStringParam param;
	param.intValue = index;
	param.stringValue = filename.GetBuffer(MAX_PATH);
	pImageEditorView->SendMessage(WM_REQUEST_DELETE_TEXTURE, WM_REQUEST_DELETE_TEXTURE, reinterpret_cast<LPARAM>(&param));
}


void	CMainFrame::PlayVideo()
{
	pMovieView->SendMessage(WM_TIMER_PLAY, WM_TIMER_PLAY, 0);
}

void	CMainFrame::StopVideo()
{
	pMovieView->SendMessage(WM_TIMER_STOP, WM_TIMER_STOP, 0);
}
void	CMainFrame::UpdateMovieFrame(int frame)
{
	pMovieView->SendMessage(WM_REQUEST_CHANGE_MOVIEFRAME, WM_REQUEST_CHANGE_MOVIEFRAME, frame);
}

void	CMainFrame::SetcvFrameToImageEditor(cv::Mat* cvFrame)
{
	pImageEditorView->SendMessage(WM_REQUEST_SET_CAPTURE, WM_REQUEST_SET_CAPTURE, reinterpret_cast<LPARAM>(cvFrame));	
	pImageEditorView->Invalidate();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnChangeObject(NMHDR* pNMHDR,LRESULT *pResult)
{
	//CView* pActiveView = GetActiveView();
	//ASSERT(pActiveView != 0);

	NM_TREEVIEW *pObjectTree = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pObjectTree->itemNew.hItem;
	HTREEITEM hParentItem = ObjectTree.GetParentItem(hItem);
	CString Name = ObjectTree.GetItemText(hItem);
	CString ParentName = ObjectTree.GetItemText(hParentItem);

	if (Name.CompareNoCase("background") == 0){		
		pImageEditorView->SendMessage(WM_REQUEST_BACKGROUND_INFO, WM_REQUEST_BACKGROUND_INFO);
	}
	else if (Name.CompareNoCase("renderer") == 0){
		pImageEditorView->SendMessage(WM_REQUEST_SCENE_INFO, WM_REQUEST_SCENE_INFO);
	}
	else if (Name.CompareNoCase("canvas") == 0){
		pImageEditorView->SendMessage(WM_REQUEST_CANVAS_INFO, WM_REQUEST_CANVAS_INFO);
		//pImageEditorView->SendMessage(WM_REQUEST_GIZMO_INFO, WM_REQUEST_GIZMO_INFO);
	}
	else if (ParentName.CompareNoCase("video") == 0){
		pMovieView->SendMessage(WM_REQUEST_VIDEO, WM_REQUEST_VIDEO, reinterpret_cast<LPARAM>(Name.GetBuffer(MAX_PATH)));
	}


	*pResult = 0;
}
void CMainFrame::OnChangeShader(NMHDR* pNMHDR,LRESULT *pResult)
{

	NM_TREEVIEW *pShaderTree = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hItem = pShaderTree->itemNew.hItem;
	CString Name = ShaderTree.GetItemText(hItem);
	/*
	if(Name.CompareNoCase(STRING_SHADER_ALGORITHM_NONE) == 0){
		ChangeScene((int)SHADER_TYPE_FIXED_PIPE);
	}else if(Name.CompareNoCase(STRING_SHADER_FIXED_PIPE_SHADOW_MAP) == 0){
		ChangeScene((int)SHADER_TYPE_FIXED_PIPE_SHADOW_MAP);
	}else if(Name.CompareNoCase(STRING_SHADER_PROJECTED_SHADOW) == 0){
		ChangeScene((int)SHADER_TYPE_PROJECTED_MAP);
	}
	*/

	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::UpdateProperty(CMFCPropertyGridProperty* pProperty)
{	
	//CView* pActiveView = GetActiveView();
	//ASSERT(pActiveView != 0);

	eEvent Event = eEvent((int)pProperty->GetData());
//	ASSERT( EVENT_BEGIN <= Event && Event <= EVENT_END);
	BSTR bstr = pProperty->GetValue().bstrVal;
	std::string option = BstrToStdString(bstr);
	if (Event > MOVIE_PROPERTY_EVENT_BEGIN &&  Event < MOVIE_PROPERTY_EVENT_END)
	{		
		bgMovieAlgorithm* pEffect = pDataPropertyAction->pMovieEffect;
		int value = std::stoi(option);
		if (Event == MOVIE_PROPERTY_BLUR_SIZE_X)
			pEffect->BlurSizeX = value;
		else if (Event == MOVIE_PROPERTY_BLUR_SIZE_Y)
			pEffect->BlurSizeY = value;

		pMovieView->SendMessage(WM_REQUEST_CHANGE_MOVIE_EFFECT, WM_REQUEST_CHANGE_MOVIE_EFFECT);
	}
	else if (Event > PHOTO_PROPERTY_EVENT_BEGIN && Event < PHOTO_PROPERTY_EVENT_END)
	{
		bgPhotoEffectAlgorithm* pEffect = pDataPropertyAction->pPhotoEffect;
		if (Event >PHOTO_PROPERTY_ALPAH_BEGIN && Event < PHOTO_PROPERTY_ALPAH_END)
		{
			float value = std::stof(option);
			dynamic_cast<bgAlphaPhotoEffectAlgorithm*>(pEffect)->SetAlpha(value);
			pImageEditorView->Invalidate();
		}
		else if (Event >PHOTO_PROPERTY_BRIGHT_CONTRAST_BEGIN && Event < PHOTO_PROPERTY_BRIGHT_CONTRAST_END)
		{
			float value = std::stof(option);
			if (Event == PHOTO_PROPERTY_BRIGHT_VALUE)
				dynamic_cast<bgBrightContrastPhotoEffectAlgorithm*>(pEffect)->SetBrightness(value);
			else if (Event == PHOTO_PROPERTY_CONTRAST_VALUE)
				dynamic_cast<bgBrightContrastPhotoEffectAlgorithm*>(pEffect)->SetContrast(value);
			pImageEditorView->Invalidate();
		}
		else if (Event >PHOTO_PROPERTY_GRAY_BEGIN && Event < PHOTO_PROPERTY_GRAY_END)
		{		
			int value = 0;
			CString blendMode = CString(option.c_str());
			for (int index = 0; index < 6; index++)
			{
				if (blendMode.CompareNoCase(STRING_PHOTO_EFFECT_GRAY[index]) == 0)
				{
					value = index;
					break;
				}
			}

			dynamic_cast<bgGrayPhotoEffectAlgorithm*>(pEffect)->SetBlendWay(value);
			pImageEditorView->Invalidate();
		}
	}
	else
		pImageEditorView->SendMessage(Event, Event, reinterpret_cast<LPARAM>(option.c_str()));

	//if(Event > SCENE_RENDERER_EVENT_BEGIN && Event < SCENE_RENDERER_EVENT_END)
	//{
	//	BSTR bstr = pProperty->GetValue().bstrVal;
	//	std::string option = BstrToStdString(bstr);
	//	pImageEditorView->SendMessage(Event, Event, reinterpret_cast<LPARAM>(option.c_str()));
	//}
	//else if(Event > GIZMO_CANVAS_EVENT_BEGIN && Event < GIZMO_CANVAS_EVENT_END)
	//{
	//	BSTR bstr = pProperty->GetValue().bstrVal;
	//	std::string option = BstrToStdString(bstr);
	//	pImageEditorView->SendMessage(Event, Event, reinterpret_cast<LPARAM>(option.c_str()));
	//}
	//else if (Event > CANVAS_SIZE_EVENT_BEGIN && Event < CANVAS_SIZE_EVENT_END)
	//{
	//	BSTR bstr = pProperty->GetValue().bstrVal;
	//	std::string option = BstrToStdString(bstr);		
	//	pImageEditorView->SendMessage(Event, Event, reinterpret_cast<LPARAM>(option.c_str()));
	//}
	//else if (Event > CANVAS_EFFECT_EVENT_BEGIN && Event < CANVAS_EFFECT_EVENT_END)
	//{

	//}
	//else if (Event > CANVAS_MORPH_EVENT_BEGIN && Event < CANVAS_MORPH_EVENT_END)
	//{

	//}

/*
	if(Event >= TRANFORM_PROPERTY_EVENT_BEGIN && Event <= TRANFORM_PROPERTY_EVENT_END)
	{
		cObject* pObject = pDataPropertyAction->pObject;
		ASSERT(pObject);
		cGeometryObject* pGeoObject = dynamic_cast<cGeometryObject*>(pObject);
		if(pGeoObject == 0) 
			return;
		if(Event >= TRANSFORM_TRANSLATION_BEGIN && Event <= TRANSFORM_TRANSLATION_END)
		{
			COleVariant rVariant = pProperty->GetValue();
			float  Value = (float)rVariant.fltVal;
			switch(Event)
			{
			case TRANSFORM_TRANSLATION_X :  pGeoObject->Translation.x = Value; break;
			case TRANSFORM_TRANSLATION_Y :  pGeoObject->Translation.y = Value; break;
			case TRANSFORM_TRANSLATION_Z :  pGeoObject->Translation.z = Value; break;					
			}
		}
		else if(Event >= TRANSFORM_ROTATION_BEGIN && Event <= TRANSFORM_ROTATION_END)
		{
			float  Value = (float)pProperty->GetValue().fltVal;
			switch(Event)
			{
			case TRANSFORM_ROTATION_X :  pGeoObject->Rotation.x = Value; break;
			case TRANSFORM_ROTATION_Y :  pGeoObject->Rotation.y = Value; break;
			case TRANSFORM_ROTATION_Z :  pGeoObject->Rotation.z = Value; break;					
			}
		}
		else if(Event >= TRANSFORM_SCALE_BEGIN && Event <= TRANSFORM_SCALE_END)
		{
			float  Value = (float)pProperty->GetValue().fltVal;
			switch(Event)
			{
			case TRANSFORM_SCALE_X :  pGeoObject->Scale.x = Value; break;
			case TRANSFORM_SCALE_Y :  pGeoObject->Scale.y = Value; break;
			case TRANSFORM_SCALE_Z :  pGeoObject->Scale.z = Value; break;					
			}
		}
	}
	*/
	
	UpdateView();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CMainFrame::UpdateView()
{
	CWinApp* pApp = AfxGetApp();
	POSITION PosTemplate = pApp->GetFirstDocTemplatePosition();
	while(PosTemplate)
	{
		CDocTemplate *pTemplate = pApp->GetNextDocTemplate(PosTemplate);
		POSITION PosDoc = pTemplate->GetFirstDocPosition();
		while(PosDoc)
		{
			CDocument* pDoc = pTemplate->GetNextDoc(PosDoc);
			POSITION PosView= pDoc->GetFirstViewPosition();
			while(PosView)
			{
				CView *pView = pDoc->GetNextView(PosView);
				pView->Invalidate();
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ResponseInformation(eInformationTarget target, bgProperty& property)
{
	switch (target)
	{
		case INFO_TARGET_PROPERTY_RENDERER: 
			pDataPropertyAction->Insert(&property);
			break;

	}
}
void CMainFrame::ResponseInformation(eInformationTarget target, bgGizmo& gizmo)
{
	pDataPropertyAction->Insert(&gizmo);
}
void	CMainFrame::ResponseInformation(eInformationTarget target, bgActor& actor)
{
	if (target == INFO_TARGET_CANVAS_INFO)
	{
		if (bgCanvasActor* pCanvasActor = dynamic_cast<bgCanvasActor*>(&actor)){
			pDataPropertyAction->Insert(pCanvasActor);
		}		
	}
}
CView* CMainFrame::GetActiveView()
{
	CWinApp* pApp = AfxGetApp();
	CMDIChildWnd * pChild = ((CMDIFrameWnd*)(pApp->m_pMainWnd))->MDIGetActive();
	if (pChild == NULL)
		return NULL;

	return pChild->GetActiveView();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CMainFrame::ChangeScene(int Type)
{
	eSceneType type = eSceneType(Type);
	CWinApp* pApp = AfxGetApp();
	POSITION PosTemplate = pApp->GetFirstDocTemplatePosition();
	while(PosTemplate)
	{
		CDocTemplate *pTemplate = pApp->GetNextDocTemplate(PosTemplate);
		POSITION PosDoc = pTemplate->GetFirstDocPosition();
		while(PosDoc)
		{
			CDocument* pDoc = pTemplate->GetNextDoc(PosDoc);
			POSITION PosView= pDoc->GetFirstViewPosition();
			while(PosView)
			{
				CView *pView = pDoc->GetNextView(PosView);				
				pView->SendMessage(WM_CHANGE_SHADER_TYPE, type);
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT  CMainFrame::GetInformation(WPARAM wParam, LPARAM lParam)
{
	int Target = (int)wParam;
	eInformationTarget uiTarget = eInformationTarget(Target);

	switch (uiTarget)
	{
		case INFO_TARGET_OUTWND_OUTPUT:
		{
			bgProperty* pProperty = reinterpret_cast<bgProperty*>(lParam);
			m_wndOutput.AddMessage(pProperty->messageMap);
		}
		break;
		case INFO_TARGET_OBJECT_ADD:
		{
			bgActor* pActor = reinterpret_cast<bgActor*>(lParam);
			pObjectTreeAction->Insert(pActor);
		}
		break;
		case INFO_TARGET_GIZMO_ADD:
		{
			bgBezierPatchControlPointsGizmo* pGizmo= reinterpret_cast<bgBezierPatchControlPointsGizmo*>(lParam);
			pGizmoTreeAction->Insert(pGizmo);
		}
		break;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}
void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif

