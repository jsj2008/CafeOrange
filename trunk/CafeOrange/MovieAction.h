#pragma once
////////////////////////////////////////////////////////////////////////////////////////////
//cMovieAction
class bgMovie;
class MovieScene;
class CVideoFrameDlg;

class cMovieAction
{
public:
	bgMovie *pMovie;
	MovieScene *pScene;

	virtual void Insert(bgMovie* _pMovie) { pMovie = _pMovie; }
	virtual void Insert(MovieScene* _pScene) { pScene = _pScene; }
	virtual void Initialize(){ };

	cMovieAction();
	virtual ~cMovieAction();

protected:

};

////////////////////////////////////////////////////////////////////////////////////////////
//cMovieDlgAction
class cMovieDlgAction : public cMovieAction
{
public:
	CVideoFrameDlg* pVideoDlg;

	virtual void Initialize();

	cMovieDlgAction(CVideoFrameDlg* _pVideoDlg);
	virtual ~cMovieDlgAction();

};

