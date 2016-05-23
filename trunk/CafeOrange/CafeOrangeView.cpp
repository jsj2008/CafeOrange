
#include "stdafx.h"

// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
	#include "CafeOrange.h"
#endif

#include "ObjLoader.h"
#include "ShaderFactory.h"
#include "CafeOrangeDoc.h"
#include "CafeOrangeView.h"
#include "Action.h"
#include "bgSceneObserver.h"
#include "bgGizmo.h"
#include "bgSpecialEffectAlgorithm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CCafeOrangeView, CView)
BEGIN_MESSAGE_MAP(CCafeOrangeView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	
	ON_WM_ERASEBKGND()
	///////////////////////////////////
	//MouseEvent
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	///////////////////////////////////

	ON_COMMAND(ID_IMPORT_OBJ, OnImportObj)
	
	///////////////////////////////////

	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCafeOrangeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()

	ON_MESSAGE(WM_CHANGE_SHADER_TYPE, ChangeScene)
	ON_MESSAGE(WM_REQUEST_SCENE_INFO,						RequestInformation)
	ON_MESSAGE(WM_REQUEST_BACKGROUND_INFO,		RequestInformation)
	ON_MESSAGE(WM_REQUEST_GIZMO_INFO,						RequestInformation)
	ON_MESSAGE(WM_REQUEST_CHANGE_TEXTURE,			RequestInformation)
	ON_MESSAGE(WM_REQUEST_ADD_TEXTURE,					RequestInformation)
	ON_MESSAGE(WM_REQUEST_DELETE_TEXTURE,				RequestInformation)
	ON_MESSAGE(WM_REQUEST_TEXTURE_OPERATION,		RequestInformation)
	ON_MESSAGE(WM_REQUEST_SET_CAPTURE,					RequestInformation)
	ON_MESSAGE(WM_REQUEST_CANVAS_INFO,					RequestInformation)
	ON_MESSAGE(WM_REQUEST_ADD_PHOTO_EFFECT,		RequestInformation)
	ON_MESSAGE(WM_REQUEST_DELETE_PHOTO_EFFECT, RequestInformation)
	
	ON_MESSAGE(SCENE_RENDERER_FILL,							ChangeRendererMode)
	ON_MESSAGE(SCENE_RENDERER_SHADING,				ChangeRendererMode)
	ON_MESSAGE(GIZMO_CANVAS_ENABLE_MARK,			ChangeRendererMode)
	ON_MESSAGE(GIZMO_CANVAS_ENABLE_GUIDELINE, ChangeRendererMode)

	ON_MESSAGE(WM_REQUEST_CANVAS_SIZE,				ChangeCanvasMode)


END_MESSAGE_MAP()

CCafeOrangeView::CCafeOrangeView()
{	
	pScene = 0;
	bMouseDrag = false;

	pGizmo =0;
	pTranslateGizmo = 0;
}
CCafeOrangeView::~CCafeOrangeView()
{
	if(pGizmo) delete pGizmo;
	if (pTranslateGizmo) delete pTranslateGizmo;
}

BOOL CCafeOrangeView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
BOOL CCafeOrangeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}
int  CCafeOrangeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1){
		return -1;
	}

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);
	bgSceneObserver::GetInstance().setActiveView(pMainFrm);

	bool bRes = Renderer.CreateDevice(GetSafeHwnd());
	ASSERT(bRes == true);

	Renderer.Initialize();
	return 0;
}
void CCafeOrangeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	Renderer.MakeCurrent();	
	
	pControlPointsGizmo = new bgControlPointsGizmo();
	pBezierControlNetGizmo = new bgBezierPatchControlPointsGizmo();

//	pTranslateGizmo = new cTranslateGizmo(); 
//////////////////////////////////////////////////////////////////////////

	InitScene(SCENE_TYPE_PROTOTYPE);
}
void CCafeOrangeView::OnDraw(CDC* /*pDC*/)
{
	CCafeOrangeDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)  return;
	if (pScene == 0) return;

	Renderer.SetViewport(WindowSize.x, WindowSize.y);
	DefaultCamera.SetView();

	Renderer.MakeCurrent();
	Renderer.Clear();

	pScene->PreRender(Renderer,&DefaultCamera);				
		pScene->Render(Renderer, &DefaultCamera);
//		pControlPointsGizmo->Draw(Renderer, &DefaultCamera, pScene->GetSelectedActor(), pScene->GetSelectedShader());
//		pBezierControlNetGizmo->Draw(Renderer, &DefaultCamera, pScene->GetSelectedActor(), pScene->GetSelectedShader());
	pScene->PostRender(Renderer, &DefaultCamera);

	Renderer.Present();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCafeOrangeView::InitScene(eSceneType SceneType)
{
	DefaultCamera.Init(cCamera::PROJECTION_ORTHOGONAL);
	OrthogonalCamera.Init(cCamera::PROJECTION_ORTHOGONAL);
	
	if(pScene)
		pScene->Clear(Renderer);

	pScene = GetScene(SceneType);
	ASSERT(pScene);
	
	pScene->DoInit(Renderer);
	pScene->InitializePostProcess();

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);
	bgSceneObserver::GetInstance().setActiveScene(pScene);
	bgSceneObserver::GetInstance().setActiveView(pMainFrm);

	ASSERT(pMainFrm && pMainFrm->pDataPropertyAction);
	pMainFrm->pDataPropertyAction->Insert(pScene);
	
	//DefaultCamera.Reset(Renderer, WindowSize.x , WindowSize.y);		
	//pScene->ChangeSize(Renderer, WindowSize.x , WindowSize.y);
	
	pScene->InitializeActor(Renderer);
//	bgActor* actor = pScene->FindActorByName(std::string("Test_Bspline"));
//	pControlPointsGizmo->Initialize(actor, pScene->GetShaderFactory(), &Renderer);

//	actor = pScene->FindActorByName(std::string("CavasActor"));
//	pBezierControlNetGizmo->Initialize(actor, pScene->GetShaderFactory(), &Renderer);

	pSpecialEffect = 0; // new bg2Dto3DSpecialEffectAlgorithm();


	Invalidate();
}
bool CCafeOrangeView::InsertModel(cObject* pModel)
{
	if(pModel == 0 || pScene == 0)
		return false;

	//if(cGeometryObject* pGeoObj = dynamic_cast<cGeometryObject*>(pModel))
	//	pGeoObj->SetBoundingBox();

	//pScene->ObjectList.push_back(pModel);

	//CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	//ASSERT(pMainFrm);	
	//pMainFrm->AddModel(pModel);

	return true;
}
LRESULT CCafeOrangeView::ChangeScene(WPARAM wParam, LPARAM lParam)
{
	int  Type = (int)wParam;
	eSceneType type= eSceneType(Type);
	InitScene(type);
	return 0;
}
LRESULT CCafeOrangeView::RequestInformation(WPARAM wParam, LPARAM lParam)
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

	int  Type = (int)wParam;
	switch (Type)
	{
		case WM_REQUEST_SCENE_INFO:
		{
			bgProperty property;
			bgSceneObserver::GetInstance().getInformation(pScene, property);
			pMainFrm->ResponseInformation(INFO_TARGET_PROPERTY_RENDERER, property);
		}
		break;
		case WM_REQUEST_BACKGROUND_INFO:
		{
			bgActor* actor = pScene->GetCanvasActor();
		}
		break;
		case WM_REQUEST_GIZMO_INFO:
		{
			pMainFrm->ResponseInformation(INFO_TARGET_GIZMO_ADD, *pBezierControlNetGizmo);
		}
		break;
		case WM_REQUEST_CANVAS_INFO:
		{			
			bgActor* actor = pScene->GetCanvasActor();
			pMainFrm->ResponseInformation(INFO_TARGET_CANVAS_INFO, *actor);
		}
		break;
		case WM_REQUEST_CHANGE_TEXTURE:
		{
			std::string filename = reinterpret_cast<char*>(lParam);			
			if (filename.empty()) break;
			pScene->ChangeTexture(filename);
			bgActor* actor = pScene->GetCanvasActor();
			pBezierControlNetGizmo->Initialize(actor, pScene->GetShaderFactory(), &Renderer);
			pBezierControlNetGizmo->MoveControlPoint(Renderer, glm::vec2(0, 0), actor);
			actor->build(Renderer);
			Invalidate();
		}
		break;
		case WM_REQUEST_ADD_TEXTURE:
		{
			std::string filename = reinterpret_cast<char*>(lParam);
			if (filename.empty()) break;
			pScene->AddTexture(filename,Renderer);
			Invalidate();
		}
		break;
		case WM_REQUEST_DELETE_TEXTURE:
		{
			IndexStringParam* param = reinterpret_cast<IndexStringParam*>(lParam);
			if (param->stringValue.empty()) break;
			pScene->DeleteTexture(param->intValue,param->stringValue, Renderer);
			Invalidate();
		}
		break;
		case WM_REQUEST_TEXTURE_OPERATION:
		{
			IndexIntParam* param = reinterpret_cast<IndexIntParam*>(lParam);
			pScene->ChangeTextureOperation(param->intValue1, eTextureOperation(param->intValue2));
			Invalidate();
		}
		break;
		case WM_REQUEST_SET_CAPTURE:
		{
			cv::Mat* cvFrame = reinterpret_cast<cv::Mat*>(lParam);
			pScene->SetCVFrameToTexture(cvFrame, Renderer);			
			///////////////////////////////////////////////////////////////////////////////////////////////////
			//Just For Test
			//if (pSpecialEffect == 0)
			//{
			//	pSpecialEffect = new bg2Dto3DSpecialEffectAlgorithm();
			//	pSpecialEffect->Initialize(WindowSize, Renderer, *pScene->GetShaderFactory());
			//}
			//else
			//{
			//	pSpecialEffect->Render(pScene->ActorList, Renderer, OrthogonalCamera);
			//	bgActor* pTargetActor = pScene->FindActorByName(std::string("CavasActor"));
			//	pSpecialEffect->Execute(Renderer, pTargetActor);
			//}
			///////////////////////////////////////////////////////////////////////////////////////////////////
			Invalidate();			
		}
		break;
		case WM_REQUEST_ADD_PHOTO_EFFECT:
		{
			ePhotoEffect effect = ePhotoEffect((int)lParam);
			pScene->AddPhotoEffect(effect);
			Invalidate();
		}
		break;
		case WM_REQUEST_DELETE_PHOTO_EFFECT:
		{
			pScene->DeletePhotoEffect((int)lParam);
			Invalidate();
		}
		break;
	}

	return 0;
}
LRESULT CCafeOrangeView::ChangeRendererMode(WPARAM wParam, LPARAM lParam)
{
	int  Type = (int)wParam;
	eEvent event = eEvent(Type);
	switch (event)
	{
		case  SCENE_RENDERER_FILL:
		{
			std::string mode  = reinterpret_cast<char*>(lParam);
			if (mode.compare("Wire") == 0) pScene->fillMode = FILLMODE_WIREFRAME;
			else if (mode.compare("Solid") == 0) pScene->fillMode = FILLMODE_SOLID;			 
		}
		break;
		case SCENE_RENDERER_SHADING:
		{
			std::string mode = reinterpret_cast<char*>(lParam);
			eShadingModel _mode;
			if (mode.compare("Flat") == 0) pScene->shadingModel = SHADING_MODEL_FLAT;
			else if (mode.compare("Smooth") == 0)pScene->shadingModel = SHADING_MODEL_SMOOTH;
		}
		break;
		case GIZMO_CANVAS_ENABLE_MARK:
		{
			std::string mode = reinterpret_cast<char*>(lParam);
			bool enabled = mode == "on" ? true : false;
			pBezierControlNetGizmo->enableMark = enabled;
		}
		break;
		case GIZMO_CANVAS_ENABLE_GUIDELINE:
		{
			std::string mode = reinterpret_cast<char*>(lParam);
			bool enabled = mode == "on" ? true : false;
			pBezierControlNetGizmo->enableGuideLine = enabled;
		}
		break;

	}

	return 0;
}
LRESULT CCafeOrangeView::ChangeCanvasMode(WPARAM wParam, LPARAM lParam)
{
	int  Type = (int)wParam;
	eEvent event = eEvent(Type);
	switch (event)
	{
	case WM_REQUEST_CANVAS_SIZE:
		{
			int targetIndex = 0;
			std::string size = reinterpret_cast<char*>(lParam);
			for (int index = 0; index < (int)CANVAS_TYPE_NUM; index++){
				if (size.compare(STRING_CANVAS_SIZE[index]) == 0){
					targetIndex = index;
					break;
				}
			}
			
			pScene->ChangeCanvasSize(eCanvasSize(targetIndex), Renderer);
		}
		break;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Import 3D Model
void CCafeOrangeView::OnImportObj()
{
	ASSERT(pScene);

	CFileDialog FileDialog(TRUE,NULL,NULL,OFN_ALLOWMULTISELECT,STRING_IMPORT_3D_FILTER);
	if(FileDialog.DoModal() != IDOK){
		return;
	}

	CString FileExt = FileDialog.GetFileExt();
	FileExt.MakeUpper();

	POSITION Pos = FileDialog.GetStartPosition();
	while(Pos != NULL)
	{
		CString FilePath = FileDialog.GetNextPathName(Pos);
		if(FileExt.CompareNoCase(STRING_COLLADA_EXTENSION) == 0){
		
		}
		else if(FileExt.CompareNoCase(STRING_OBJ_EXTENSION) == 0){
			ObjLoader ObjLoader;
			if(cObject *pModel = ObjLoader.LoadOBJFromFile(FilePath)){				
				InsertModel(pModel);
			}
		}
		else if(FileExt.CompareNoCase(STRING_3DS_EXTENSION) == 0){

		}
		else if(FileExt.CompareNoCase(STRING_MD2_EXTENSION) == 0){

		}
		else if(FileExt.CompareNoCase(STRING_MD3_EXTENSION) == 0){

		}
		else if(FileExt.CompareNoCase(STRING_STL_EXTENSION) == 0){
	
		}
	}

	Invalidate();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	CCafeOrangeView::Resize(int width, int height)
{
	/*if (width == 0 || height == 0 || pScene == 0)
		return;

	Renderer.MakeCurrent();
	DefaultCamera.Reset(Renderer, width, height);

	WindowSize.x = width;
	WindowSize.y = height;

	pScene->ChangeSize(Renderer, width, height);

	Invalidate();*/
}

void CCafeOrangeView::OnSize(UINT nType, int cx, int cy) 
{
	if(cx== 0 || cy == 0 || pScene == 0)
		return;

	CView::OnSize(nType, cx, cy);

	Renderer.MakeCurrent();
	DefaultCamera.Reset(Renderer, cx, cy);

	WindowSize.x = cx;
	WindowSize.y = cy;

	pScene->ChangeSize(Renderer, cx, cy);
	Invalidate();

	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCafeOrangeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(pScene == 0)
		return;

	switch(nChar)
	{
	case VK_ESCAPE:
		 pGizmo = 0;
		 Invalidate();
		 break;
	case 't':
	case 'T':
		//if(cObject* pObject = pScene->GetSelectedObject()){
		//	if(cGeometryObject* pGeoObject = dynamic_cast<cGeometryObject*>(pObject))
		//	{
		//		pGizmo = pTranslateGizmo;
		//		pGizmo->SetObject(pGeoObject);
		//		Invalidate();
		//	}
		//}
		break;


	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCafeOrangeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (pScene == 0)
		return;

	SetCapture();

	UserMode = USER_MOUSE_LDOWN;
	DragStartPoint = point;
	
	//bgActor* pSelectedActor= pScene->SelectModel(Renderer, DefaultCamera, DragStartPoint.x, DragStartPoint.y);
	//if (pSelectedActor == NULL){
	//	pSelectedActor = pControlPointsGizmo->SelectControlPoint(Renderer, DefaultCamera, DragStartPoint.x, DragStartPoint.y);
	//	if (pSelectedActor == NULL)
	//		pSelectedActor = pBezierControlNetGizmo->SelectControlPoint(Renderer, DefaultCamera, DragStartPoint.x, DragStartPoint.y);
	//}	
	//pScene->SetSelectedActor(pSelectedActor);

	Invalidate();
	CView::OnLButtonDown(nFlags, point);
}
void CCafeOrangeView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(UserMode == USER_MOUSE_LDOWN){
       UserMode = USER_MOUSE_DRAG;
	}

	bgActor* pSelectedActor = pScene->GetSelectedActor();
	if (pControlPointsGizmo == NULL || pBezierControlNetGizmo==NULL || pSelectedActor == NULL)
		return;

	bool bReRendering = false;
	if(UserMode == USER_MOUSE_DRAG)
	{
	   DragEndPoint = point;
	   
	   glm::vec2 Differential;
	   Differential.x = (float)DragEndPoint.x - (float)DragStartPoint.x;
	   Differential.y = (float)DragEndPoint.y - (float)DragStartPoint.y;
	   Differential.x *= 0.001f; Differential.y *= -0.001f;
			
	   pControlPointsGizmo->MoveControlPoint(Renderer,Differential, pSelectedActor);
	   if(bgActor* actor = pScene->FindActorByName(std::string("Test_Bspline")))
			actor->build(Renderer);
	   
	   pBezierControlNetGizmo->MoveControlPoint(Renderer, Differential, pSelectedActor);
	   if(bgActor* actor = pScene->FindActorByName(std::string("CavasActor")))
			actor->build(Renderer);

	   bReRendering = true;
	}

	

	if(bReRendering)
		Invalidate();

	CView::OnMouseMove(nFlags, point);
}
void CCafeOrangeView::OnLButtonUp(UINT nFlags, CPoint point)
{
	UserMode = USER_MOUSE_LUP;
	DragStartPoint.SetPoint(0, 0);
	DragEndPoint.SetPoint(0, 0);

	if (GetCapture()==this)
		ReleaseCapture();

	//OutputDebugString("-------------------------LButton Up is called-------------------------\n");
	pScene->SetSelectedActor(NULL);

	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}
BOOL CCafeOrangeView::OnMouseWheel(UINT nFlags,short zDelta, CPoint pt)
{	
	glm::vec3 Target	 = DefaultCamera.GetLookAtPos();
	glm::vec3 Eye		 = DefaultCamera.GetEyePos();
	glm::vec3 Direction =  Target -  Eye;
	const float CAMERA_SPEED_RATE = 0.05f;
	Direction *= zDelta / WHEEL_DELTA * CAMERA_SPEED_RATE;
	Eye += Direction;
	
	glm::mat4 ModelView = glm::lookAt(Eye, Target, glm::vec3(0.0f, 1.0f, 0.0f));
	DefaultCamera.SetWorldViewMatrix(ModelView);

	DefaultCamera.SetEyePos(Eye);

	Invalidate(FALSE);

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCafeOrangeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
void CCafeOrangeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CCafeOrangeView::UpdateUI(const cObject* pObject)
{
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	ASSERT(pMainFrm);

//	pMainFrm->pDataPropertyAction->Insert(const_cast<cObject*>(pObject));
//	pMainFrm->pMaterialPropertyAction->Insert(const_cast<cObject*>(pObject));

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
cScene* CCafeOrangeView::GetScene(eSceneType SceneType)
{
	CCafeOrangeDoc* pDoc = (CCafeOrangeDoc*)GetDocument();
	ASSERT(pDoc);
	switch (SceneType)
	{
	case SCENE_TYPE_PROTOTYPE:
		   DefaultCamera.SetEyePos(glm::vec3(0.0f, 0.0f, 1.0f), true);
		   OrthogonalCamera.SetEyePos(glm::vec3(0.0f, 0.0f, 1.0f), true);
		   break;
	}
	return pDoc->GetScene(SceneType);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
void CCafeOrangeView::AssertValid() const
{
	CView::AssertValid();
}
void CCafeOrangeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CCafeOrangeDoc* CCafeOrangeView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCafeOrangeDoc)));
	return (CCafeOrangeDoc*)m_pDocument;
}
#endif //_DEBUG




