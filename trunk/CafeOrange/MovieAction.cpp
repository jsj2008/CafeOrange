#include "stdafx.h"
#include "VideoFrameDlg.h"
#include "bgMovie.h"
#include "MovieAction.h"


////////////////////////////////////////////////////////////////////////////////////////////
//cMovieAction
cMovieAction::cMovieAction()
{
	pMovie = 0;
	pScene = 0;
}

cMovieAction::~cMovieAction()
{

}

////////////////////////////////////////////////////////////////////////////////////////////
//cMovieDlgAction

cMovieDlgAction::cMovieDlgAction(CVideoFrameDlg* _pVideoDlg)
{
	pVideoDlg = _pVideoDlg;
}

cMovieDlgAction::~cMovieDlgAction()
{

}

void cMovieDlgAction::Initialize()
{
	pVideoDlg->FrameSlider.SetRange(0, pMovie->GetTotalFrame());
	pVideoDlg->FrameSlider.SetPos(0);
	pVideoDlg->FrameSlider.SetLineSize(1);
	pVideoDlg->FrameSlider.SetPageSize(100);

	CString message;
	message.Format("%d", (int)pMovie->GetTotalFrame());
	pVideoDlg->TotalFrameEdit.SetWindowText(message);
	message.Format("%d", 0);
	pVideoDlg->FrameIndexEdit.SetWindowText(message);

	pVideoDlg->UpdateWindow();
}