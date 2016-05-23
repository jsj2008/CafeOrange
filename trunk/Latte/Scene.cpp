#include "StdAfx.h"

#include "Renderer.h"
#include "ShaderFactory.h"
#include "bgObject.h"
#include "Scene.h"
#include "bgMaterial.h"
#include "bgSceneObserver.h"
const int PickingBufferSize = 256;

//////////////////////////////////////////////////////////////////////////////
////Background
cBackground::cBackground()
{
	UseGrid	= true;
	GridSize = 50.0f;
	GridStep = 5.0f;

	BackType = TYPE_BACK_COLOR;
	BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_LEFTTOP] = glm::vec3(0.0f, 0.0f, 0.0f);
	BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_RIGHTTOP] = glm::vec3(0.0f, 0.0f, 0.0f);
	BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_RIGHTBOTTOM] = glm::vec3(0.8f, 0.8f, 0.8f);
	BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_LEFTBOTTOM] = glm::vec3(0.8f, 0.8f, 0.8f);

}

void cBackground::SetGrid(bool bUse)
{
	UseGrid = bUse;
}

void cBackground::DrawGrid(float Size,float Step)
{
	glPushMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
	glBegin(GL_LINES);

	glColor3f(0.3f, 0.3f, 0.3f);
	for(float i=Step; i <= Size; i+= Step)
	{
		glVertex3f(-Size, 0,  i);  
		glVertex3f( Size, 0,  i);
		glVertex3f(-Size, 0, -i);  
		glVertex3f( Size, 0, -i);

		glVertex3f( i, 0, -Size);  
		glVertex3f( i, 0,  Size);
		glVertex3f(-i, 0, -Size);  
		glVertex3f(-i, 0,  Size);
	}

	// x-axis
	glColor3f(0.5f, 0, 0);
	glVertex3f(-Size, 0, 0);
	glVertex3f( Size, 0, 0);

	// z-axis
	glColor3f(0,0,0.5f);
	glVertex3f(0, 0, -Size);
	glVertex3f(0, 0,  Size);

	glEnd();

	glPopMatrix();
}

void cBackground::Render(cRenderer& Renderer,cCamera* pCamera)
{
	switch(BackType)
	{
	case TYPE_BACK_COLOR:
		RenderGradient(Renderer);
		break;
	case TYPE_BACK_IMAGE:
		RenderTexture(Renderer);
		break;
	case TYPE_BACK_ANIMATION:
		RenderAnimation(Renderer);
		break;
	}

	if(UseGrid)
		RenderGrid(Renderer,*pCamera);	
}

void cBackground::RenderGrid(cRenderer& Renderer,cCamera& Camera)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glm::vec3 Eye = Camera.GetInitEyePos();
		glm::vec3 LookAt = Camera.GetLookAtPos();
		gluLookAt(Eye.x, Eye.y, Eye.z, LookAt.x, LookAt.y, LookAt.z, 0.0f,  1.0f , 0.0f);

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		if(UseGrid){
			DrawGrid(GridSize, GridStep);
		}

		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);

	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void cBackground::RenderGradient(cRenderer& Renderer)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		glm::vec3 Color;

		glBegin(GL_QUADS);		
			Color = BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_LEFTTOP];
			glColor3f(Color.r, Color.g, Color.b);		
			glVertex2f(-1.0f,1.0f);

			Color = BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_RIGHTTOP];
			glColor3f(Color.r, Color.g, Color.b);		
			glVertex2f(1.0f, 1.0f);
		
			Color = BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_RIGHTBOTTOM];
			glColor3f(Color.r, Color.g, Color.b);		
			glVertex2f(1.0, -1.0);

			Color = BackgroundInfo.Color[(int)cBackgroundInfo::POSITION_LEFTBOTTOM];
			glColor3f(Color.r, Color.g, Color.b);		
			glVertex2f(-1.0, -1.0);
		glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}
void cBackground::RenderTexture(cRenderer& Renderer)
{

}
void cBackground::RenderAnimation(cRenderer& Renderer)
{

}
void cBackground::RenderActor(cRenderer& Renderer, cCamera& Camera, bgActor& BackgroundActor)
{

	glm::vec2  windowSize = Camera.GetWindowSize();
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, windowSize.x, 0.0, windowSize.y, -1.0, 1.0);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
			glLoadIdentity();

			bgShader* pShader = BackgroundActor.getShader();
			bgMaterial* pMaterial = BackgroundActor.getMaterial();
			glm::mat4 IdentityMat;

			if (pShader) Renderer.BindShader(pShader, pMaterial, IdentityMat, IdentityMat);
			if (pMaterial) Renderer.BindMaterial(pMaterial);
				Renderer.RenderActor(&BackgroundActor);
			if (pShader) Renderer.UnbindShader(pShader);
			if (pMaterial) Renderer.UnBindMaterial(pMaterial);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////////////////////////////////
////Scene
cScene::cScene(eSceneType	_SceneType)
{	
	pSelectedActor = 0;
	HasSelectedActor = false;
	SceneType = _SceneType;

	pImageActor = NULL;
	pImageData = NULL;
	pImageShader = NULL;
	pImageMaterial = NULL;
}
cScene::~cScene()
{
//	delete pSelectedData;
//	delete pSelectedMapper;

	Release();	
}

 void cScene::ChangeSize(cRenderer& Renderer,int width, int height)
 {
	WindowSize.x = width;
	WindowSize.y = height;
 }

 void cScene::InitializePostProcess()
 {
	 PostProcess.Initialize(ShaderFactory);
 }

void cScene::Initialize()
{
	fillMode = FILLMODE_SOLID;
	shadingModel = SHADING_MODEL_SMOOTH;

	pSelectedActor = 0;
	
	_LightList::iterator iLight = LightList.begin();
	for( ; iLight != LightList.end() ; iLight++)
	{
		cLight* pLight = *iLight;
		delete pLight;
	}
	LightList.clear();

	cDirectionalLight* pDirLight = new cDirectionalLight(LightList.size());
	pDirLight->SetPosition(glm::vec3(0.0f, 20.0f, 20.0f));
	pDirLight->Enable(true);
	pDirLight->Build();
	LightList.push_back(pDirLight);

	Build();
	ShaderFactory.InitializeShaderFactory();
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	pImageActor = new bgCanvasActor();
	pImageData = new bgData();
	pImageShader = new bgShader();
	pImageMaterial =  new bgMaterial();
	
	pImageActor->setShader(pImageShader);
	bgMapper* pMapper = new bgMapper();
	pMapper->setInput(pImageData);
	pImageActor->setInput(pMapper);
	pImageActor->setMaterial(pImageMaterial);

	pImageActor->name = std::string("CavasActor");

	ActorList.push_back(pImageActor);
	////////////////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

void cScene::Release()
{
	_ObjectList::iterator iObj = ObjectList.begin();
	for( ; iObj != ObjectList.end() ; iObj++){
		cObject* pObj = *iObj;
		delete pObj;
	}
	ObjectList.clear();

	_LightList::iterator iLight = LightList.begin();
	for( ; iLight != LightList.end() ; iLight++)
	{
		cLight* pLight = *iLight;
		delete pLight;
	}
	LightList.clear();

}

void cScene::Update()
{

}

void cScene::Build()
{
	_LightList::iterator iLight = LightList.begin();
	for( ; iLight != LightList.end() ; iLight++)
	{
		cLight* pLight = *iLight;
		pLight->Build();
	}
}

void cScene::Build(cRenderer& Renderer)
{
	_ActorList::iterator iActor = ActorList.begin();
	for (; iActor != ActorList.end(); iActor++)
	{
		bgActor* pActor = *iActor;
		pActor->build(Renderer);
	}
}

void cScene::PreRender(cRenderer& Renderer,cCamera* pCamera)
{
	Renderer.SetFillMode(FILLMODE_SOLID);
	Renderer.SetShadingModel(SHADING_MODEL_SMOOTH);	
		Background.Render(Renderer,pCamera);
	Renderer.SetFillMode(fillMode);
	Renderer.SetShadingModel(shadingModel);
}

void cScene::Render(cRenderer& Renderer,cCamera* pCamera)
{	
	if (pCamera == 0)
		return;

	Build(Renderer);
	//bool bHasPostEffect = PhotoEffectList.size() > 0 ? true : false;
	
	pCamera->SetView();
	glm::mat4  cameraProjectMat4 = pCamera->GetProjectionMatrix() * pCamera->GetWorldViewMatrix();

//	glEnable(GL_LIGHTING);
//	glEnable(GL_DEPTH_TEST);	
	
	if (PostProcess.IsEnable())
		PostProcess.BindScreenFBO(WindowSize);

	_ActorList::iterator iActor = ActorList.begin();
	for (; iActor != ActorList.end(); iActor++)
	{
		bgActor* pActor = *iActor;
		bgShader* pShader = pActor->getShader();
		bgMaterial* pMaterial = pActor->getMaterial();
		
		if (pShader) Renderer.BindShader(pShader, pMaterial, cameraProjectMat4, pActor->GetTransformMat4());
		if (pMaterial) Renderer.BindMaterial(pMaterial);
				Renderer.RenderActor(pActor);	
		if (pShader) Renderer.UnbindShader(pShader);
		if (pMaterial) Renderer.UnBindMaterial(pMaterial);
	}

	if (PostProcess.IsEnable()){
		PostProcess.UnBindScreenFBO();
		PostProcess.Render(Renderer, *pCamera);
	}

//	glDisable(GL_LIGHTING);
//	glDisable(GL_DEPTH_TEST);
}

void cScene::PostRender(cRenderer& Renderer,cCamera* pCamera)
{

}

void cScene::DrawLight(cRenderer& Renderer)
{
	glPushMatrix();
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	_LightList::iterator iLight = LightList.begin();
	for(; iLight != LightList.end() ; iLight++){
		cLight* pLight = *iLight;
		glm::vec3 Position =pLight->Position;
		glPushMatrix( );
			
			glTranslatef(Position.x,Position.y,Position.z);
			glScalef(0.2f, 0.2f, 0.2f);
//Picking
//			glLoadName((*iLight).PickingID);

			Renderer.DrawCircle();
			Renderer.DrawLight(*pLight);
		glPopMatrix( );
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

bgActor* cScene::FindActorByName(std::string& name)
{	
	_ActorList::iterator iActor = ActorList.begin();
	for (; iActor != ActorList.end(); iActor++)
	{
		if ( (*iActor)->name.compare(name) == 0)
			return *iActor;
	}

	return 0;
}

void cScene::InitializeActor(cRenderer& Renderer)
{
	pImageMaterial->LoadTexture(std::string("../Media/Texture/ghost02.jpg"));

	bgShaderAlgorithm*  pVertexShaderAlgorithm = new bgShaderAlgorithm(SHADER_TYPE_VERTEX, SHADER_VERTEX_SIMPLE_PTC);
	pVertexShaderAlgorithm->LoadFromFile(SHADER_TYPE_VERTEX, "../Media/Shader/simplePTC_vert.glsl");
	bgShaderAlgorithm*  pFragmentShaderAlgorithm = new bgShaderFragmentAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_SIMPLE_PTC);
	pFragmentShaderAlgorithm->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/simplePTC_frag.glsl");
	
	ASSERT(pVertexShaderAlgorithm != 0);
	ASSERT(pFragmentShaderAlgorithm != 0);

	pImageShader->AddShader(pVertexShaderAlgorithm);
	pImageShader->AddShader(pFragmentShaderAlgorithm);
	pImageData->generate(CANVAS_TYPE_FULL);

	pImageActor->setTranslation(glm::vec3(.0f, 0.f, 0.f));
	pImageActor->initialize();
	pImageActor->build(Renderer);	
}

void	cScene::ChangeCanvasSize(eCanvasSize size,cRenderer& Renderer)
{
	if (pImageActor == NULL) return;
	pImageActor->resize(size);
	pImageActor->build(Renderer);
}

void cScene::SetCVFrameToTexture(cv::Mat* cvFrame,cRenderer& Renderer)
{
	Renderer.MakeCurrent();
	bgTexture* Texture = new bgTexture();

	Texture->ConvertOpenCVMat(*cvFrame);
	pImageMaterial->ChangeTexture(0, Texture);
	pImageActor->build(Renderer);
//	imwrite("TestMe.jpg", *cvFrame);

}

void cScene::AddPhotoEffect(ePhotoEffect effect)
{	
	if (pImageActor == NULL) return;

	PostProcess.AddPostEffect(ShaderFactory, effect, pImageActor);
}
void cScene::DeletePhotoEffect(int  algorithmIndex)
{
	if (pImageActor == NULL) return;

	PostProcess.DeletePostEffect( algorithmIndex);
}
bgPhotoEffectAlgorithm*	cScene::GetPhotoEffect(int algorithmIndex)
{
	return PostProcess.GetPostEffect(algorithmIndex);
}

void cScene::GetTextureMemory()
{
	//bgActor* pActor = FindActorByName(std::string("CavasActor"));
	//bgMaterial* pMaterial = pActor->getMaterial();

	//bgTexture* Texture = pMaterial->TextureList[0];
	//GLuint TextureID = Texture->TextureID;
	//GLubyte* TextureMemory = new GLubyte[Texture->TextureWidth * Texture->TextureHeight * 3];

	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, TextureID);
	//	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureMemory);
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glDisable(GL_TEXTURE_2D);

	//CString message;
	//for (int col = 0; col < Texture->TextureHeight; col++)
	//{
	//	for (int row = 0; row < Texture->TextureWidth; row++)
	//	{
	//		int R = TextureMemory[col * Texture->TextureWidth  + row*3];
	//		int G = TextureMemory[col * Texture->TextureWidth + row*3 + 1];
	//		int B = TextureMemory[col * Texture->TextureWidth  + row*3 + 2];

	//	/*	message.Format("(R,G,B:%d,%d,%d)", R, G, B);
	//		OutputDebugString(message);
	//		OutputDebugString(" ");*/
	//	}

	//	//OutputDebugString("\n");
	//}
}


void	cScene::ChangeTexture(const std::string& filename)
{
	if (pImageActor == NULL) return;

	bgMaterial* pMaterial = new bgMaterial();
	std::string filePath = "../Media/Texture/";
	filePath += filename;
	pMaterial->LoadTexture(filePath);

	pImageActor->setMaterial(pMaterial);

}
void	cScene::ChangeTexture(bgTexture* pTexture,cRenderer& Renderer)
{
	pImageMaterial->ChangeTexture(0, pTexture);
	pImageActor->build(Renderer);
}
void	cScene::AddTexture(const std::string& filename,cRenderer& Renderer)
{
	Renderer.MakeCurrent();
	pImageMaterial->LoadTexture(filename);
	pImageActor->getShader()->SetTextureProperty(pImageMaterial);
}
void	cScene::DeleteTexture(int index,const std::string& filename,cRenderer& Renderer)
{
	if (pImageActor == NULL) return;
	std::string texturePath = pImageMaterial->TextureList[index]->FilePath;
	std::string textureName = FindFileName(texturePath);
	if (textureName.compare(filename) != 0) return;

	pImageMaterial->DeleteTexture(filename);
	pImageActor->getShader()->SetTextureProperty(pImageMaterial);
}
void	cScene::ChangeTextureOperation(int stageIndex, eTextureOperation operation)
{
	if (pImageActor == NULL) return;
	pImageShader->ChangeTextureOperation(stageIndex, operation);
}
void	cScene::SetSelectedActor(bgActor* _pActor)
{
	if (_pActor)
	{
		HasSelectedActor = true;
		pSelectedActor = _pActor;
	}
	else
	{
		HasSelectedActor = false;
		pSelectedActor = NULL;
	}
}

bool	cScene::IsIntersectedOn2D(eSourceType type, bgRay& ray, int primitiveCount, glm::mat4& transformedMat, bgData* pbgData)
{
	bool  IsIntersected = false;
	glm::vec3 IntersectedPoint;
	
	const  std::vector<GLushort>& IndexList = pbgData->GetIndexList();

	std::vector<glm::vec3> VectorList;
	MakeGlmVec3List(pbgData->GetPositionList(), VectorList);

	for (int index = 0; index < IndexList.size(); index += 3 )
	{
		std::vector<glm::vec3> triangle;
		triangle.push_back(VectorList[IndexList[index]]);
		triangle.push_back(VectorList[IndexList[index+1]]);
		triangle.push_back(VectorList[IndexList[index+2]]);

		switch (type)
		{
		case SOURCE_2D_TRIANGLE_PC:
		case SOURCE_2D_TRIANGLE_PTC:
		case SOURCE_2D_RECTANGLE_PC:
		case SOURCE_2D_CIRCLE_PC:
		case SOURCE_2D_RECTANGLE_PTC:
		case SOURCE_2D_CIRCLE_PTC:
		case SOURCE_2D_MOVIE_RECTANGLE_PTC:
			IsIntersected = ray.CalculateRayOnTriangle(triangle, transformedMat, IntersectedPoint); break;			
		}

		if (IsIntersected)
			return true;
	}

	return false;
}
bool cScene::IsIntersectedOn3D(eSourceType type, bgRay& ray, int primitiveCount, glm::mat4& transformedMat, bgData* pbgData)
{
	bool  IsIntersected = false;
	glm::vec3 IntersectedPoint;
	switch (type)
	{
		case SOURCE_3D_SPHERE_PC:
		case SOURCE_3D_SPHERE_PNC:
		{
			const bgSphereShapeInfo * pInfo = dynamic_cast<bgSphereShapeInfo*>(pbgData->GetShapeInfo());
			if (pInfo != 0)
				IsIntersected = ray.CalculateRayOnSphere(pInfo, transformedMat, IntersectedPoint); 
			else{
				bgSphereShapeInfo Info;
				IsIntersected = ray.CalculateRayOnSphere(&Info, transformedMat, IntersectedPoint);
			}

		}
		break;
	}

	return false;
}

bgActor* cScene::TestIntersction(bgRay& ray)
{
	_ActorList::iterator iActor = ActorList.begin();
	for (; iActor != ActorList.end(); iActor++)
	{
		bool IsIntersected = false;
		bgActor* pActor = *iActor;
		if (!pActor->IsPickable()) continue;

		bgData* pData = pActor->getInput()->getInput();
		glm::mat4 transformedMat = pActor->GetTransformMat4();

		eSourceType type = pData->GetSourceType();
		int primitiveCount = pActor->getPrimitiveCount();

		if (type > SOURCE_2D_BEGIN && type < SOURCE_2D_END)
		{
			IsIntersected = IsIntersectedOn2D(type, ray, primitiveCount, transformedMat, pData);
		}
		else if (type > SOURCE_3D_BEGIN && type < SOURCE_3D_END)
		{
			IsIntersected = IsIntersectedOn3D(type, ray, primitiveCount, transformedMat, pData);
		}

		if (IsIntersected)
			return pActor;
	}

	return 0;
}

//http://antongerdelan.net/opengl/raycasting.html
bgActor* cScene::SelectModel(cRenderer& Renderer, cCamera& Camera, int x, int y, bool drawHelper)
{
	glm::vec2 WindowSize = Camera.GetWindowSize(); 	
	if (x < 0 || x >= WindowSize.x || y < 0 || y >= WindowSize.y)
		return 0;

	glm::vec4 origin = glm::vec4(
															(2.0f*x)/WindowSize.x - 1.0f,
															1 - (2.0f*y)/WindowSize.y,
															-1, 1 
														);


	glm::mat4 InverseMVP = glm::inverse(Camera.GetWorldViewMatrix())* glm::inverse(Camera.GetProjectionMatrix());
	origin = InverseMVP *origin;
	glm::vec3 RayOrigin = glm::vec3(origin.x, origin.y, origin.z);

	bgRay ray(RayOrigin, Camera.GetEyePos());
	bgActor * pSelectedActor = TestIntersction(ray);

	//////////////////////////////////////////////////////////////////////////////////
	//Drawing Ray - Helper
	if (drawHelper)
	{
		bgRayActor* pRayActor = new bgRayActor(false);
		glm::vec4  direction = InverseMVP  * glm::vec4(ray.direction, 0);
		pRayActor->initializeRay(&ShaderFactory, RayOrigin, glm::vec3(direction.x, direction.y, direction.z));
		pRayActor->build(Renderer);
		ActorList.push_back(pRayActor);
	}
	//////////////////////////////////////////////////////////////////////////////////

	return pSelectedActor;
}


