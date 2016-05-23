#include "stdafx.h"
#include "CafeOrangeDoc.h"
#include "OpenCVView.h"
#include "MovieAction.h"
#include "Action.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(COpenCVView, CView)
BEGIN_MESSAGE_MAP(COpenCVView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()

	ON_MESSAGE(WM_TIMER_PLAY, RequestInformation)
	ON_MESSAGE(WM_TIMER_STOP, RequestInformation)
	ON_MESSAGE(WM_REQUEST_CHANGE_MOVIEFRAME, RequestInformation)
	ON_MESSAGE(WM_REQUEST_CAPTURE_MOVIEFRAME, RequestInformation)
	ON_MESSAGE(WM_REQUEST_ADD_MOVIE_EFFECT,		 RequestInformation)
	ON_MESSAGE(WM_REQUEST_DELETE_MOVIE_EFFECT,	RequestInformation)
	ON_MESSAGE(WM_REQUEST_CHANGE_MOVIE_EFFECT, RequestInformation)
	ON_MESSAGE(WM_REQUEST_VIDEO, RequestInformation)
END_MESSAGE_MAP()

COpenCVView::COpenCVView()
{
	pScene = 0;
}

COpenCVView::~COpenCVView()
{

}

BOOL COpenCVView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL COpenCVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

int  COpenCVView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1){
		return -1;
	}

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	bool bRes = Renderer.CreateDevice(GetSafeHwnd());
	ASSERT(bRes == true);

	Renderer.Initialize();
	return 0;
}

void COpenCVView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	Renderer.MakeCurrent();
	
	InitScene();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);
	currentFrame = 0;
	pMovie = new bgMovie();	
	pMainFrm->pMovieDlgAction->Insert(pMovie);
	
	pMainFrm->pDataPropertyAction->Insert(pMovie);
}

void COpenCVView::OnTimer(UINT nIDEvent)
{
	if (currentFrame > pMovie->GetTotalFrame())
		currentFrame = 0;

	UpdateFrame(currentFrame);
	currentFrame++;
}

cv::Mat*	COpenCVView::GetFrameMemory(int frame)
{
	pMovie->Run(frame);
	return pMovie->GetCurrentFrame();
}

void		COpenCVView::UpdateFrame(int frame)
{
	if (!pMovie->IsOpenVideo()) return;

	pMovie->Run(frame);
	bgTexture* pTexture = pMovie->GetCurrentTexture();
	pScene->SetMovieTexture(pTexture, Renderer);	

	Invalidate();
}

void		COpenCVView::PlayVideo()
{
	SetTimer(TIMER_MOVIE_PLAY, pMovie->GetFPS(), NULL);
}
void		COpenCVView::StopVideo()
{
	KillTimer(TIMER_MOVIE_PLAY);	
}
void		COpenCVView::GoFirstVideo()
{

}
void		COpenCVView::GoLastVideo()
{

}

void		COpenCVView::GoFrame(int frame)
{

}

bool		COpenCVView::OpenVideo(const std::string& filename)
{
	CloseVideo();
	std::string filePath = "../Media/Video/";
	filePath += filename;

	return pMovie->OpenVideo(filePath);
}

void		COpenCVView::CloseVideo()
{
	pMovie->CloseVideo();
}

void COpenCVView::InitScene()
{
	DefaultCamera.Init(cCamera::PROJECTION_ORTHOGONAL);

	if (pScene)
		pScene->Clear(Renderer);
	else
		pScene = new MovieScene(SCENE_TYPE_MOVIE);
	
	DefaultCamera.SetEyePos(glm::vec3(0.0f, 0.0f, 4.0f), true);
	pScene->DoInit(Renderer);

	pScene->InitializeActor(Renderer);

	Invalidate();
}

LRESULT COpenCVView::RequestInformation(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	bool  bInvalidated = false;
	int  Type = (int)wParam;
	switch (Type)
	{
		case WM_TIMER_PLAY:PlayVideo();	break;
		case WM_TIMER_STOP:StopVideo(); break;
		case WM_REQUEST_CHANGE_MOVIEFRAME:
		{
			int frame = (int)lParam;
			UpdateFrame(frame);
			bInvalidated = true;
		}
		break;
		case WM_REQUEST_CAPTURE_MOVIEFRAME:
		{
			int frame = (int)lParam;
			cv::Mat* cvFrame = GetFrameMemory(frame);
			pMainFrm->SetcvFrameToImageEditor(cvFrame);
		}
		break;
		case WM_REQUEST_ADD_MOVIE_EFFECT:
		{
			int effect = (int)lParam;
			eMovieEffect movieEffect = eMovieEffect(effect);
			pMovie->AddAlgorithm(movieEffect);
			UpdateFrame(pMovie->GetMovieFrame());
			bInvalidated = true;
		}
		break;
		case WM_REQUEST_DELETE_MOVIE_EFFECT:
		{
			int algorithmIndex = (int)lParam;
			pMovie->DeleteAlgorithm(algorithmIndex);
			UpdateFrame(pMovie->GetMovieFrame());
			bInvalidated = true;
		}
		break;
		case WM_REQUEST_CHANGE_MOVIE_EFFECT:
		{
			pMovie->SetNeedModify(true);
			UpdateFrame(pMovie->GetMovieFrame());
			bInvalidated = true;
		}
		break;
		case WM_REQUEST_VIDEO:
		{
			std::string filename = reinterpret_cast<char*>(lParam);
			OpenVideo(filename);
			pMainFrm->pMovieDlgAction->Initialize();
			bInvalidated = true;
		}
		break;
	}

	if (bInvalidated) Invalidate();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void COpenCVView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pScene == 0)
		return;

	SetCapture();

	UserMode = USER_MOUSE_LDOWN;
	DragStartPoint = point;

	Invalidate();
	CView::OnLButtonDown(nFlags, point);
}
void COpenCVView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (UserMode == USER_MOUSE_LDOWN){
		UserMode = USER_MOUSE_DRAG;
	}


	CView::OnMouseMove(nFlags, point);
}
void COpenCVView::OnLButtonUp(UINT nFlags, CPoint point)
{
	UserMode = USER_MOUSE_LUP;
	DragStartPoint.SetPoint(0, 0);
	DragEndPoint.SetPoint(0, 0);

	if (GetCapture() == this)
		ReleaseCapture();
	
	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}
BOOL COpenCVView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	glm::vec3 Target = DefaultCamera.GetLookAtPos();
	glm::vec3 Eye = DefaultCamera.GetEyePos();
	glm::vec3 Direction = Target - Eye;
	const float CAMERA_SPEED_RATE = 0.05f;
	Direction *= zDelta / WHEEL_DELTA * CAMERA_SPEED_RATE;
	Eye += Direction;

	glm::mat4 ModelView = glm::lookAt(Eye, Target, glm::vec3(0.0f, 1.0f, 0.0f));
	DefaultCamera.SetWorldViewMatrix(ModelView);

	DefaultCamera.SetEyePos(Eye);

	Invalidate(FALSE);
	return TRUE;
}

void COpenCVView::OnDraw(CDC* /*pDC*/)
{
	CCafeOrangeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!pDoc)  return;
	if (pScene == 0) return;

	Renderer.SetViewport(WindowSize.x, WindowSize.y);
	DefaultCamera.SetView();

	Renderer.MakeCurrent();
	Renderer.Clear();

	pScene->PreRender(Renderer, &DefaultCamera);
		pScene->Render(Renderer, &DefaultCamera);
	pScene->PostRender(Renderer, &DefaultCamera);

	Renderer.Present();
}
void COpenCVView::OnSize(UINT nType, int cx, int cy)
{
	if (cx == 0 || cy == 0 || pScene == 0)
		return;

	CView::OnSize(nType, cx, cy);

	Renderer.MakeCurrent();
	DefaultCamera.Reset(Renderer, cx, cy);

	WindowSize.x = cx;
	WindowSize.y = cy;

	pScene->ChangeSize(Renderer, cx, cy);
	Invalidate();
}

#ifdef _DEBUG
void COpenCVView::AssertValid() const
{
	CView::AssertValid();
}
void COpenCVView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CCafeOrangeDoc* COpenCVView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCafeOrangeDoc)));
	return (CCafeOrangeDoc*)m_pDocument;
}
#endif //_DEBUG