#pragma once
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"

class cGizmo;
class bgProperty;
class cTranslateGizmo;

class bgControlPointsGizmo;
class bgBezierPatchControlPointsGizmo;
class bgSpecialEffectAlgorithm;

class CCafeOrangeView : public CView
{
DECLARE_DYNCREATE(CCafeOrangeView)
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
			
	cCamera			DefaultCamera;
	cCamera			OrthogonalCamera;
	cRenderer		Renderer;

	cGizmo*			pGizmo;
	bgControlPointsGizmo*	pControlPointsGizmo;
	bgBezierPatchControlPointsGizmo*	pBezierControlNetGizmo;

	CCafeOrangeDoc* GetDocument() const;

	void	   InitScene(eSceneType SceneType);
	cScene* GetScene(eSceneType SceneType);
	cScene*	GetActiveScene() { return pScene;  }

	bool		InsertModel(cObject* pModel);
	void		Resize(int width, int height);

	CCafeOrangeView();
	virtual ~CCafeOrangeView();
	
public:
	virtual void OnDraw(CDC* pDC);  
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	void	UpdateUI(const cObject* pObject);

	afx_msg int	OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy) ;

	//Mouse Event
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta, CPoint pt);
	
	//User Message
	afx_msg LRESULT	ChangeScene(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  RequestInformation(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ChangeRendererMode(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ChangeCanvasMode(WPARAM wParam, LPARAM lParam);

protected:

	cTranslateGizmo* pTranslateGizmo;
	
	afx_msg void OnImportObj();
	afx_msg BOOL  OnEraseBkgnd(CDC* pDC);	
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

private:
	bool			bMouseDrag;
	CPoint		DragStartPoint;
	CPoint		DragEndPoint;

	eUserMode	   UserMode;	
	eUserAction	UserAction;

	cScene*			pScene;
	glm::vec2		WindowSize;

	//Just For Test
	bgSpecialEffectAlgorithm* pSpecialEffect;

public:	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

};

#ifndef _DEBUG  // debug version in CafeOrangeView.cpp
inline CCafeOrangeDoc* CCafeOrangeView::GetDocument() const
   { return reinterpret_cast<CCafeOrangeDoc*>(m_pDocument); }
#endif

