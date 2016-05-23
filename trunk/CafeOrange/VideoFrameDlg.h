#pragma once
#include "afxwin.h"

class CVideoFrameDlg : public CDialogEx
{
	DECLARE_MESSAGE_MAP()
public:

	CSliderCtrl	FrameSlider;
	
	CButton		FirstBtn;
	CButton		LastBtn;
	CButton		PlayBtn;
	CButton		StopBtn;
	CButton		FrameGoBtn;
	
	CButton		SceneSaveBtn;
	CButton		SceneCaptureBtn;

	CEdit			TotalFrameEdit;
	CEdit			FrameIndexEdit;

	CButton		ApplyBtn;
	CButton		UpBtn;
	CButton		DownBtn;
	CButton		DeleteBtn;

	CListBox		EffectFactoryBox;
	CListBox		EffectChainBox;

	CVideoFrameDlg();
	virtual ~CVideoFrameDlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

public:
	virtual BOOL OnInitDialog();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCapture();
	afx_msg void OnPlay();
	afx_msg void OnStop();
	afx_msg void OnSave();
	afx_msg void OnApply();
	afx_msg void OnDelete();
	afx_msg void OnSelectMovieEffect();
};