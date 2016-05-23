#pragma once
#include "afxdialogex.h"

class CPhotoFrameDlg : public CDialogEx
{
	DECLARE_MESSAGE_MAP()

public:

	CListBox		EffectFactoryBox;
	CListBox		EffectChainBox;
	CListBox		SpecialEffectFactoryBox;

	CButton		EffectApplyBtn;
	CButton		EffectUpBtn;
	CButton		EffectDownBtn;
	CButton		EffectDeleteBtn;

	CButton		SpecialEffectApplyBtn;
	CButton		SpecialEffectRevertBtn;


	CPhotoFrameDlg();
	virtual ~CPhotoFrameDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnApplyEffect();
	afx_msg void OnDeleteEffect();
	afx_msg void OnSelectPhotoEffect();

};

