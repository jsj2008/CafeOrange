#pragma once
#include "GlobalEnum.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "TextureDlg.h"
#include "Resource.h"
#include "opencv2\core\core.hpp"

class COutlookBar : public CMFCOutlookBar
{
	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); if (!bNameValid) strName.Empty(); }
};


class cObject;
class bgProperty;
class bgTexture;
class bgActor;
class cObjectTreeAction;
class cSceneTreeAction;
class cDataPropertyAction;
class cMaterialPropertyAction;
class cGizmoTreeAction;
class bgGizmo;
class CMFCPropertyGridProperty;
class cMovieDlgAction;
class COpenCVView;
class CCafeOrangeView;
class cChangeTextureAction;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
	DECLARE_MESSAGE_MAP()

public:
	cObjectTreeAction*			pObjectTreeAction;
	cSceneTreeAction*			pSceneTreeAction;
	cDataPropertyAction*		pDataPropertyAction;
	cMaterialPropertyAction*  pMaterialPropertyAction;
	cGizmoTreeAction*			  pGizmoTreeAction;
	cMovieDlgAction*			  pMovieDlgAction;
	cChangeTextureAction*	  pChangeTextureAction;

	void UpdateProperty(CMFCPropertyGridProperty* pProperty);

	COutputWnd*	GetOutputWnd() { return &m_wndOutput; }
	void		ResponseInformation(eInformationTarget target, bgProperty& property);
	void		ResponseInformation(eInformationTarget target, bgGizmo& gizmo);
	void		ResponseInformation(eInformationTarget target, bgActor& actor);
	void		ChangeTexture(CString& filename);
	/////////////////////////////////////////////////////////////////////////////
	void		PlayVideo();
	void		StopVideo();
	/////////////////////////////////////////////////////////////////////////////
	void		UpdateMovieFrame(int frame);
	void		InitializeMDIView(CView* pOpenCVView, CView* pImageEditorView);
	void		SetCapture(int frame);
	/////////////////////////////////////////////////////////////////////////////
	//Movie Effect
	void		AddMovieEffect(eMovieEffect effect);
	void		DeleteMovieEffect(int algorithmIndex);
	void		SetMovieEffect(int algorithmIndex);
	void		SetcvFrameToImageEditor(cv::Mat* cvFrame);
	//Photo Effect
	void		AddPhotoEffect(ePhotoEffect effect);
	void		DeletePhotoEffect(int  algorithmIndex);
	void		SetPhotoEffect(int algorithmIndex);
	/////////////////////////////////////////////////////////////////////////////

	//Texture
	void		AddTexture(CString& filename);
	void		ChangeTextureOpeation(int stageIndex, eTextureOperation operation);
	void		DeleteTexture(int index, CString& filename);
	/////////////////////////////////////////////////////////////

	CMainFrame();
	virtual ~CMainFrame();

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);


protected:  
	CMFCMenuBar			m_wndMenuBar;
	CMFCToolBar				m_wndToolBar;
	CMFCStatusBar			m_wndStatusBar;
	CMFCToolBarImages	m_UserImages;
	COutputWnd					m_wndOutput;

	CPropertiesWnd		DataPropertyGrid;
	CPropertiesWnd		MaterialPropertyGrid;

	COutlookBar			NavigationBar;
	CTreeCtrl					ObjectTree;
	CTreeCtrl					ShaderTree;
	CTextureDlg			TextureDlg;


	CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	CMFCOutlookBarPane*    m_pCurrOutlookPage;
	CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);

	void			UpdateView();
	CView*	GetActiveView();

	void		ChangeScene(int Type);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	
	BOOL	CreateDockingWindows();
	void	SetDockingWindowIcons(BOOL bHiColorIcons);
	BOOL	CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID,CTreeCtrl& ObjectTree,CTreeCtrl& ShaderTree,int nInitialWidth);
	int		FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);

	afx_msg void OnChangeObject(NMHDR* pNMHDR,LRESULT *pResult);
	afx_msg void OnChangeShader(NMHDR* pNMHDR,LRESULT *pResult);
	//afx_msg LRESULT OnPropertyChanged(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT  GetInformation(WPARAM wParam, LPARAM lParam);

private:
	CView*	pMovieView;
	CView*  pImageEditorView;


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};


