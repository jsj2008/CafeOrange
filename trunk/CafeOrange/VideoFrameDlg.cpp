#include "stdafx.h"
#include "CafeOrange.h"
#include "VideoFrameDlg.h"
#include "MainFrm.h"

BEGIN_MESSAGE_MAP(CVideoFrameDlg, CDialogEx)	
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CAPTURE_BTN, OnCapture)
	ON_BN_CLICKED(IDC_SAVE_BTN,			OnSave)
	ON_BN_CLICKED(IDC_VIDEO_PLAY,		OnPlay)
	ON_BN_CLICKED(IDC_VIDEO_STOP,		OnStop)
	ON_BN_CLICKED(IDC_APPLY_BTN,		OnApply)
	ON_BN_CLICKED(IDC_DELETE_BTN,		OnDelete)
	ON_LBN_SELCHANGE(IDC_EFFECT_CHAIN_LIST, OnSelectMovieEffect)
END_MESSAGE_MAP()


CVideoFrameDlg::CVideoFrameDlg()
{

}

CVideoFrameDlg::~CVideoFrameDlg()
{

}

void CVideoFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_VIDEO_FRAME, FrameSlider);

	DDX_Control(pDX, IDC_VIDEO_FIRST, FirstBtn);
	DDX_Control(pDX, IDC_VIDEO_PLAY, PlayBtn);
	DDX_Control(pDX, IDC_VIDEO_STOP, StopBtn);
	DDX_Control(pDX, IDC_VIDEO_LAST, LastBtn);
	DDX_Control(pDX, IDC_VIDEO_GO,	  FrameGoBtn);

	DDX_Control(pDX, IDC_CAPTURE_BTN, SceneCaptureBtn);
	DDX_Control(pDX, IDC_SAVE_BTN, SceneSaveBtn);

	DDX_Control(pDX, IDC_APPLY_BTN, ApplyBtn);
	DDX_Control(pDX, IDC_UP_BTN, UpBtn);
	DDX_Control(pDX, IDC_DOWN_BTN, DownBtn);
	DDX_Control(pDX, IDC_DELETE_BTN, DeleteBtn);

	DDX_Control(pDX, IDC_EFFECT_FACTORY_LIST, EffectFactoryBox);
	DDX_Control(pDX, IDC_EFFECT_CHAIN_LIST, EffectChainBox);

	DDX_Control(pDX, IDC_TOTAL_FRAME, TotalFrameEdit); 
	DDX_Control(pDX, IDC_EDIT_FRAME,		FrameIndexEdit);

}

BOOL CVideoFrameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(255, 255, 255));

	for (int index = 0; index < (int)MOVIE_EFFECT_NUM; index++)
	{
		EffectFactoryBox.AddString(STRING_MOVIE_EFFECT[index]);
	}


	return TRUE;
}

void CVideoFrameDlg::OnCapture()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	pMainFrm->SetCapture(FrameSlider.GetPos());
}

void CVideoFrameDlg::OnSave()
{

}

void CVideoFrameDlg::OnPlay()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);
	
	pMainFrm->PlayVideo();
}

void CVideoFrameDlg::OnStop()
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	pMainFrm->StopVideo();
}

void CVideoFrameDlg::OnApply()
{
	int movieEffect = EffectFactoryBox.GetCurSel();
	eMovieEffect effect = eMovieEffect(movieEffect);

	int selectedSel =EffectChainBox.AddString(STRING_MOVIE_EFFECT[movieEffect]);
	EffectChainBox.SetCurSel(selectedSel);
	
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	pMainFrm->AddMovieEffect(effect);
	OnSelectMovieEffect();
}

void CVideoFrameDlg::OnDelete()
{
	int currentSel = EffectChainBox.GetCurSel();
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	pMainFrm->DeleteMovieEffect(currentSel);
	EffectChainBox.DeleteString(currentSel);	
}

void CVideoFrameDlg::OnSelectMovieEffect()
{
	int currentSel = EffectChainBox.GetCurSel();
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);
	pMainFrm->SetMovieEffect(currentSel);
}


void CVideoFrameDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

}

void CVideoFrameDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	if (CSliderCtrl* pFrameSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar))
	{
		CString frame;
		frame.Format("%d", pFrameSlider->GetPos());
		FrameIndexEdit.SetWindowText(frame);

		pMainFrm->UpdateMovieFrame(pFrameSlider->GetPos());
	}

}









