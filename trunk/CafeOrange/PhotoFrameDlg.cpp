#include "stdafx.h"
#include "PhotoFrameDlg.h"
#include "MainFrm.h"

BEGIN_MESSAGE_MAP(CPhotoFrameDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PHOTO_APPLY_BTN,	OnApplyEffect)
	ON_BN_CLICKED(IDC_PHOTO_DELETE_BTN,	OnDeleteEffect)
	ON_LBN_SELCHANGE(IDC_PHOTO_EFFECT_CHAIN_LIST, OnSelectPhotoEffect)
END_MESSAGE_MAP()


CPhotoFrameDlg::CPhotoFrameDlg()
{

}

CPhotoFrameDlg::~CPhotoFrameDlg()
{

}

void CPhotoFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PHOTO_EFFECT_FACTORY_LIST,		EffectFactoryBox);
	DDX_Control(pDX, IDC_PHOTO_EFFECT_CHAIN_LIST,			EffectChainBox);
	DDX_Control(pDX, IDC_PHOTO_SPECIAL_EFFECT_LIST,		SpecialEffectFactoryBox);

	DDX_Control(pDX, IDC_PHOTO_APPLY_BTN, EffectApplyBtn);
	DDX_Control(pDX, IDC_PHOTO_UP_BTN,		EffectUpBtn);
	DDX_Control(pDX, IDC_PHOTO_DOWN_BTN, EffectDownBtn);
	DDX_Control(pDX, IDC_PHOTO_DELETE_BTN,EffectDeleteBtn);

	DDX_Control(pDX, IDC_PHOTO_SPECIAL_EFFECT_APPLY, SpecialEffectApplyBtn);
	DDX_Control(pDX, IDC_PHOTO_SPECIAL_EFFECT_REVERT, SpecialEffectRevertBtn);

}

BOOL CPhotoFrameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetBackgroundColor(RGB(255, 255, 255));
		
	for (int index = 0; index < (int)PHOTO_EFFECT_NUM; index++)
	{
		EffectFactoryBox.AddString(STRING_PHOTO_EFFECT[index]);
	}

	for (int index = 0; index < (int)PHOTO_SPECIAL_EFFECT_NUM; index++)
	{
		SpecialEffectFactoryBox.AddString(STRING_PHOTO_SPECIAL_EFFECT[index]);
	}


	return TRUE;
}

void CPhotoFrameDlg::OnApplyEffect()
{
	int photoEffect = EffectFactoryBox.GetCurSel();
	ePhotoEffect effect = ePhotoEffect(photoEffect);

	int selectedSel = EffectChainBox.AddString(STRING_PHOTO_EFFECT[effect]);
	EffectChainBox.SetCurSel(selectedSel);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	pMainFrm->AddPhotoEffect(effect);
}
void CPhotoFrameDlg::OnDeleteEffect()
{
	int currentSel = EffectChainBox.GetCurSel();
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	pMainFrm->DeletePhotoEffect(currentSel);

	EffectChainBox.DeleteString(currentSel);
}
void CPhotoFrameDlg::OnSelectPhotoEffect()
{
	int currentSel = EffectChainBox.GetCurSel();
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);
	pMainFrm->SetPhotoEffect(currentSel);
}
