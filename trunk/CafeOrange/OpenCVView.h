#pragma once
#include "afxwin.h"
#include "Renderer.h"
#include "MovieScene.h"
#include "Camera.h"
#include "bgMovie.h"

class COpenCVView : public CView
{
	DECLARE_DYNCREATE(COpenCVView)
	DECLARE_MESSAGE_MAP()

public:
	enum eUserMode{
		USER_NONE,
		USER_MOUSE_LDOWN,
		USER_MOUSE_LUP,
		USER_MOUSE_DRAG,
	};

	enum eUserAction{
		USER_ACTION_NONE,
		USER_ACTION_MOVE,
		USER_ACTION_ROTATE,
		USER_ACTION_ZOOM,
		USER_ACTION_MODEL_TRANSLATE,
		USER_ACTION_MODEL_ROTATE
	};

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void		UpdateFrame(int frame);
	void		PlayVideo();
	void		StopVideo();
	void		GoFirstVideo();
	void		GoLastVideo();
	void		GoFrame(int frame);
	void		CloseVideo();

	cCamera			DefaultCamera;
	cRenderer		Renderer;

	void		InitScene();
	cv::Mat*	GetFrameMemory(int frame);
	CCafeOrangeDoc* GetDocument() const;

	COpenCVView();
	virtual ~COpenCVView();

protected:
	//Mouse Event
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	//User Message
	afx_msg LRESULT  RequestInformation(WPARAM wParam, LPARAM lParam);

	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void		OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL   OnEraseBkgnd(CDC* pDC);

	afx_msg void OnTimer(UINT nIDEvent);

	bool		OpenVideo(const std::string& filename);

private:
	MovieScene*			pScene;
	glm::vec2		WindowSize;
	//Just Test
	bgMovie*	pMovie;
	//Frame Index
	int currentFrame;
	
	//User Interaction
	bool			bMouseDrag;
	CPoint		DragStartPoint;
	CPoint		DragEndPoint;
	eUserMode	   UserMode;
	eUserAction	UserAction;
	///////////////////////////////////////////////////


public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#ifndef _DEBUG  // debug version in COpenCVView.cpp
inline CCafeOrangeDoc* COpenCVView::GetDocument() const
{
	return reinterpret_cast<CCafeOrangeDoc*>(m_pDocument);
}
#endif