#include "StdAfx.h"
#include "Object.h"
#include "Light.h"
#include "bgActor.h"
#include "bgSceneObserver.h"
#include "Renderer.h"

const DWORD LIGHT_COLOR			= 0xFFFFFF00;
const DWORD LIGHT_START_COLOR = 0xFFFFFFFF;
const DWORD LIGHT_END_COLOR	= 0xFFFFFF00;

GLuint GetGLDrawType(eDrawElementType type)
{
	switch (type)
	{
	case ELEMENT_TYPE_TRIANGLES:			  return GL_TRIANGLES; break;
	case ELEMENT_TYPE_TRIANGLE_STRIP: return GL_TRIANGLE_STRIP; break;
	case ELEMENT_TYPE_QUADS:				  return GL_QUADS; break;
	case ELEMENT_TYPE_POINTS:				  return GL_POINT; break;
	case ELEMENT_TYPE_LINES:					  return GL_LINES; break;
	}

	return GL_TRIANGLES;
}

GLuint GetGLFillMode(eFillMode mode)
{
	switch (mode)
	{
	case FILLMODE_WIREFRAME:
		return GL_LINE;
		break;
	case FILLMODE_SOLID:
		return GL_FILL;
		break;
	}

	return GL_FILL;
}

GLuint GetGLShadingModel(eShadingModel model)
{
	switch (model)
	{
	case SHADING_MODEL_FLAT:
		return GL_FLAT;
		break;
	case SHADING_MODEL_SMOOTH:
		return GL_SMOOTH;
		break;
	}
	return GL_SMOOTH;
}

cRenderer::cRenderer()
{
	OwnerHwnd = 0;
	hRenderingDC = 0;
	hRenderingRC = 0;
	MultisampleFormat	  = -1;
	bMultisampleSupported = true;

	BuildGizmos();
}
cRenderer::~cRenderer()
{
	Destory();
}
void  cRenderer::Destory()
{
//	DepthTexture.Release();

	if(wglGetCurrentContext() != NULL){
		wglMakeCurrent(NULL,NULL);
	}

	if(hRenderingRC != NULL){
		wglDeleteContext(hRenderingRC);
		hRenderingRC = NULL;
	}

	if (hRenderingDC){
		::ReleaseDC(OwnerHwnd, hRenderingDC);
	}

}

bool  cRenderer::CreateDevice(HWND hWnd)
{
	OwnerHwnd	 = hWnd;
	hRenderingDC = ::GetDC(hWnd);

	if(!SetWidowPixelFormat(hRenderingDC)){
		return false;
	}
	
	hRenderingRC = wglCreateContext(hRenderingDC);
	if(hRenderingRC){
		MakeCurrent();		
	}
	else{
		return false;
	}

	if(!bMultisampleSupported)
	{
		if(InitMultiSample(hWnd))
		{
			wglMakeCurrent(hRenderingDC, 0);			
			wglDeleteContext(hRenderingRC);										
			ReleaseDC(hWnd, hRenderingDC);	
			hRenderingDC = 0;
			hRenderingRC = 0;
			return CreateDevice(hWnd);
		}
	}

	GLint error = glewInit();
	if(error != GLEW_OK){
		return false;		
	}
	

	glEnable(GL_NORMALIZE);

	bgProperty property;
	const GLubyte* pString1 = glGetString(GL_VENDOR);
	string message = reinterpret_cast<const char*>(pString1);
	property.messageMap.insert(make_pair(string("VENDOR:"), message));
	const GLubyte* pString2 = glGetString(GL_VERSION);
	message = reinterpret_cast<const char*>(pString2);
	property.messageMap.insert(make_pair(string("OPENGL VERSION:"), message));
	const GLubyte* pString3 = glGetString(GL_RENDERER);
	message = reinterpret_cast<const char*>(pString3);
	property.messageMap.insert(make_pair(string("RENDERER:"), message));
	const GLubyte* pString4 = glGetString(GL_SHADING_LANGUAGE_VERSION);
	message = reinterpret_cast<const char*>(pString4);
	property.messageMap.insert(make_pair(string("GLSL:"), message));

	bgSceneObserver::GetInstance().sendInformation(INFO_TARGET_OUTWND_OUTPUT, property);

	return true;
}
int    cRenderer::SetWidowPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,										   // version number
		PFD_DRAW_TO_WINDOW |			// support window
		PFD_SUPPORT_OPENGL |				// support OpenGL
		PFD_DOUBLEBUFFER,					// double buffered
		PFD_TYPE_RGBA,							// RGBA type
		24,								 // 24-bit color depth
		0, 0, 0, 0, 0, 0,				 // color bits ignored
		0,								 // no alpha buffer
		0,								 // shift bit ignored
		0,								 // no accumulation buffer
		0, 0, 0, 0,						 // accum bits ignored
		16,								 // 16-bit z-buffer
		0,								 // no stencil buffer
		0,								 // no auxiliary buffer
		PFD_MAIN_PLANE,			 // main layer
		0,								 // reserved
		0, 0, 0							 // layer masks ignored
	};

	int iPixelFormat  = ChoosePixelFormat(hDC, &pfd);

	if ( iPixelFormat == 0 ){
		return 0;
	}

	if (SetPixelFormat(hDC, iPixelFormat, &pfd) == FALSE){
		return 0;
	}    

	return iPixelFormat;
}
bool  cRenderer::InitMultiSample(HWND hWnd)
{	
	if(!WGLisExtensionSupported("WGL_ARB_multisample"))
	{		
		bMultisampleSupported=false;
		return false;
	}

	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");	
	if (!wglChoosePixelFormatARB) 
	{
		bMultisampleSupported=false;
		return false;
	}

	HDC hDC = GetDC(hWnd);

	int		pixelFormat;
	BOOL	valid;
	UINT	numFormats;
	float	fAttributes[] = {0,0};

	int iAttributes[] = 
	{ 
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,
		WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB,		24,
		WGL_ALPHA_BITS_ARB,		8,
		WGL_DEPTH_BITS_ARB,		16,
		WGL_STENCIL_BITS_ARB,	0,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB, 8,		// Check For 4x Multisampling
		0,0
	};
	
	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
	if (valid && numFormats >= 1)
	{
		bMultisampleSupported	= true;
		MultisampleFormat		= pixelFormat;	
		return bMultisampleSupported;
	}

	// Our Pixel Format With 4 Samples Failed, Test For 2 Samples
	iAttributes[19] = 4;
	valid = wglChoosePixelFormatARB(hDC,iAttributes,fAttributes,1,&pixelFormat,&numFormats);
	if (valid && numFormats >= 1)
	{
		bMultisampleSupported	= true;
		MultisampleFormat		= pixelFormat;	 
		return bMultisampleSupported;
	}
	
	return  bMultisampleSupported;
}

//Gizmo Setup
void cRenderer::DrawBoudingBox(cBoundingBox& BoundingBox,glm::vec3& Position,glm::vec3& Rotation,glm::vec3& Scale)
{
	glPushMatrix();

	glTranslatef(Position.x,Position.y,Position.z);
	glRotatef(Rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef(Rotation.z, 0.0f, 0.0f, 1.0f);
	glScalef(Scale.x, Scale.y, Scale.z);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	
	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(1);
	glBegin(GL_LINES);
		glVertex3f(BoundingBox.Min.x, BoundingBox.Max.y, BoundingBox.Min.z);
		glVertex3f(BoundingBox.Min.x, BoundingBox.Max.y, BoundingBox.Max.z);

		glVertex3f(BoundingBox.Min.x, BoundingBox.Max.y, BoundingBox.Max.z);
		glVertex3f(BoundingBox.Max.x, BoundingBox.Max.y, BoundingBox.Max.z);

		glVertex3f(BoundingBox.Max.x, BoundingBox.Max.y, BoundingBox.Max.z);
		glVertex3f(BoundingBox.Max.x, BoundingBox.Max.y, BoundingBox.Min.z);

		glVertex3f(BoundingBox.Max.x, BoundingBox.Max.y, BoundingBox.Min.z);
		glVertex3f(BoundingBox.Min.x, BoundingBox.Max.y, BoundingBox.Min.z);

		glVertex3f(BoundingBox.Min.x, BoundingBox.Min.y, BoundingBox.Min.z);
		glVertex3f(BoundingBox.Min.x, BoundingBox.Min.y, BoundingBox.Max.z);

		glVertex3f(BoundingBox.Min.x, BoundingBox.Min.y, BoundingBox.Max.z);
		glVertex3f(BoundingBox.Max.x, BoundingBox.Min.y, BoundingBox.Max.z);

		glVertex3f(BoundingBox.Max.x, BoundingBox.Min.y, BoundingBox.Max.z);
		glVertex3f(BoundingBox.Max.x, BoundingBox.Min.y, BoundingBox.Min.z);

		glVertex3f(BoundingBox.Max.x, BoundingBox.Min.y, BoundingBox.Min.z);
		glVertex3f(BoundingBox.Min.x, BoundingBox.Min.y, BoundingBox.Min.z);

		glVertex3f(BoundingBox.Min.x, BoundingBox.Max.y, BoundingBox.Min.z);
		glVertex3f(BoundingBox.Min.x, BoundingBox.Min.y, BoundingBox.Min.z);

		glVertex3f(BoundingBox.Min.x, BoundingBox.Max.y, BoundingBox.Max.z);
		glVertex3f(BoundingBox.Min.x, BoundingBox.Min.y, BoundingBox.Max.z);

		glVertex3f(BoundingBox.Max.x, BoundingBox.Max.y, BoundingBox.Max.z);
		glVertex3f(BoundingBox.Max.x, BoundingBox.Min.y, BoundingBox.Max.z);

		glVertex3f(BoundingBox.Max.x, BoundingBox.Max.y, BoundingBox.Min.z);
		glVertex3f(BoundingBox.Max.x, BoundingBox.Min.y, BoundingBox.Min.z);

	glEnd();

	
	glLineWidth(1);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
void cRenderer::BuildCircleXGizmo(cCVertices& Vertices)
{
	const int SegmentCount = 32*4;

	Vertices.resize(SegmentCount);

	Vertices[0].Set(glm::vec3(0, 0, 0), 0x77ffff00);
	for(int iSegment = 1; iSegment < SegmentCount; iSegment++){
		float Angle = 3.141592f * 2 * (float)iSegment / (float)SegmentCount;
		Vertices[iSegment].Set(glm::vec3(0 , cosf(Angle), sinf(Angle)), 0xFFFFFF00);
	}
}
void cRenderer::BuildCircleYGizmo(cCVertices& Vertices)
{
	const int SegmentCount = 32*4;

	Vertices.resize(SegmentCount) ;

	Vertices[0].Set(glm::vec3(0, 0, 0), 0x77ffff00);
	for(int iSegment = 1; iSegment < SegmentCount; iSegment++){
		float Angle = 3.141592f  * 2 * (float)iSegment / (float)SegmentCount;
		Vertices[iSegment].Set(glm::vec3(sinf(Angle), 0, cosf(Angle)), 0xFFFFFF00);
	}
}
void cRenderer::BuildCircleZGizmo(cCVertices& Vertices)
{
	const int SegmentCount = 32*4;

	Vertices.resize(SegmentCount);

	Vertices[0].Set(glm::vec3(0, 0, 0), 0x77ffff00);
	for(int iSegment = 1; iSegment < SegmentCount; iSegment++){
		float Angle = 3.141592f  * 2 * (float)iSegment / (float)SegmentCount;
		Vertices[iSegment].Set(glm::vec3(cosf(Angle), sinf(Angle), 0), 0xFFFFFF00);
	}
}
void cRenderer::BuildGizmos()
{
	Gizmos.resize(GIZMO_COUNT);
	BuildCircleXGizmo(Gizmos[GIZMO_ROTATION_X]);
	BuildCircleYGizmo(Gizmos[GIZMO_ROTATION_Y]);
	BuildCircleZGizmo(Gizmos[GIZMO_ROTATION_Z]);
}
void  cRenderer::DrawCircle()
{	
	glPushMatrix();

	const cCVertices& RotateX = Gizmos[GIZMO_ROTATION_X];
	DrawPrimitive(PRIMITIVE_LINE_STRIP, (UINT)RotateX.size() -2, 
				  &RotateX[1], sizeof(cCVertex), VERTEX_XYZ_DIFFUSE);

	const cCVertices& RotateY = Gizmos[GIZMO_ROTATION_Y];
	DrawPrimitive(PRIMITIVE_LINE_STRIP, (UINT)RotateY.size() -2, 
				  &RotateY[1], sizeof(cCVertex), VERTEX_XYZ_DIFFUSE);

	const cCVertices& RotateZ = Gizmos[GIZMO_ROTATION_Z];
	DrawPrimitive(PRIMITIVE_LINE_STRIP, (UINT)RotateZ.size()-2, 
				  &RotateZ[1], sizeof(cCVertex), VERTEX_XYZ_DIFFUSE);

	glPopMatrix();
}
void  cRenderer::DrawLight(cLight& Light)
{
	glm::vec3 LightPosition = Light.Position;
	glPushMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(LightPosition.x,LightPosition.y,LightPosition.z);

	switch(Light.Type)
	{
	case cLight::LIGHT_TYPE_DIRECTION:
		{		
			int AngleCount = 20; // 8개의 점으로 그림
			float RayLength = glm::length(LightPosition);
			float Radius = RayLength/4 ;/// 4.0f;
			
			glm::vec3 LightDirection = -LightPosition;
			LightDirection = glm::normalize(LightDirection);
			glm::vec3 YAxis(0, 1, 0);
			glm::vec3 XAxis = glm::cross(YAxis,LightDirection);
			XAxis = glm::normalize(XAxis);
			YAxis = glm::cross(LightDirection,XAxis);
			
			std::vector<cCVertex> Line(AngleCount * 2);
			// 중앙선
			Line[0].Set(LightPosition, 0xFFFFFFFF);

			glm::vec3 EndPoint = LightDirection * (RayLength*2) + LightPosition;
			Line[1].Set(EndPoint, 0xFFFF00FF);
			DrawPrimitive(PRIMITIVE_LINES, 1, &Line[0], sizeof(cCVertex), VERTEX_XYZ_DIFFUSE); //PRIMITIVE_LINES
						
			for(int iAngle = 0; iAngle < AngleCount; iAngle ++){
				float xOffset = Radius * cosf(2 * 3.141592f * iAngle / AngleCount);
				float yOffset = Radius * sinf(2 * 3.141592f* iAngle / AngleCount);

				glm::vec3 Offset = xOffset * XAxis + yOffset * YAxis;
				glm::vec3 StartPoint = LightPosition + Offset;
				
				Line[iAngle * 2 + 0].Set(StartPoint, LIGHT_START_COLOR);

				glm::vec3 EndPoint = LightDirection * (RayLength*2) + LightPosition + Offset;
				Line[iAngle * 2 + 1].Set(EndPoint, LIGHT_END_COLOR);
			}
			DrawPrimitive(PRIMITIVE_LINES, Line.size() / 2, &Line[0], sizeof(cCVertex), VERTEX_XYZ_DIFFUSE);
		}
		break;
	case cLight::LIGHT_TYPE_POINT:
		  break;
	case cLight::LIGHT_TYPE_SPOT:
		  break;
	}


	glPopMatrix();
}

//Init
void  cRenderer::Initialize()
{
	//EnableNormalize(true);
	//EnableDepthTest(true);
	//glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		
	//glClearDepth(1.0f);									
	//glDepthFunc(GL_LEQUAL);								
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
}

void  cRenderer::SetViewport(int width, int height)
{
	glViewport(0, 0, width, height); 
}

bool  cRenderer::MakeCurrent()
{
	if(hRenderingRC ==  0){
		return false;
	}

	return 	wglMakeCurrent(hRenderingDC,hRenderingRC) == TRUE ? true : false;
}
void  cRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glLoadIdentity();
}
void  cRenderer::Present()
{
	glFlush();
	glFinish();

	SwapBuffers(hRenderingDC);
}

void cRenderer::SetFillMode(eFillMode mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, GetGLFillMode(mode));
}

void cRenderer::SetShadingModel(eShadingModel model)
{
	glShadeModel(GetGLShadingModel(model));
}

void cRenderer::RenderDepthTexture()
{   
	/*
	GLint	Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport); 
	int LightDepthSize = DepthTexture.DepthSize;
	glActiveTexture(GL_TEXTURE0 + DepthTexture.DepthStage);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, DepthTexture.pPicBits);
    glActiveTexture(GL_TEXTURE0);
    glPixelZoom( (float)Viewport[2] / (float) LightDepthSize, (float)Viewport[3] / (float) LightDepthSize);
		glDrawPixels(LightDepthSize, LightDepthSize, GL_LUMINANCE, GL_FLOAT, DepthTexture.pPicBits);
    glPixelZoom(1.0, 1.0);
	ASSERT_GL_CALL();
	*/
}

//Generate opengl buffer
bool	 cRenderer::GenerateGLBuffer(const bgData* pData, GLuint &VAO, GLuint &VBO, GLuint &IBO)
{
	if (pData == 0)
		return false;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (pData->NumIndex()){
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, pData->NumIndex() * sizeof(GLushort), pData->GetIndexListPointer(), GL_STATIC_DRAW);
	}

	ASSERT_GL_CALL();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pData->NumTotalVertexAttrib() * sizeof(float), NULL, GL_STATIC_DRAW);
	
	int offset = 0;
	int attribPointerIndex = 0;

	//Position List
	glBufferSubData(GL_ARRAY_BUFFER, 0, pData->NumPosition()*sizeof(float), pData->GetPositionListPointer());
	glVertexAttribPointer(attribPointerIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(attribPointerIndex);

	ASSERT_GL_CALL();
	offset = pData->NumPosition()*sizeof(float);
	
	//Color List
	if (pData->NumColor()){
		attribPointerIndex++;
		glBufferSubData(GL_ARRAY_BUFFER, offset, pData->NumColor()*sizeof(float), pData->GetColorListPointer());
		glVertexAttribPointer(attribPointerIndex, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)offset);
		glEnableVertexAttribArray(attribPointerIndex);
		ASSERT_GL_CALL();
	}

	offset += pData->NumColor()*sizeof(float);
	
	//Texcel List
	if (pData->NumTexcel()){
		attribPointerIndex++;
		glBufferSubData(GL_ARRAY_BUFFER, offset, pData->NumTexcel()*sizeof(float), pData->GetTexcelListPointer());
		glVertexAttribPointer(attribPointerIndex, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)offset);
		glEnableVertexAttribArray(attribPointerIndex);
		ASSERT_GL_CALL();
	}
		
	offset += pData->NumTexcel() * sizeof(float);	
	//Normal List
	if (pData->NumNormal()){
		attribPointerIndex++;
		glBufferSubData(GL_ARRAY_BUFFER, offset, pData->NumNormal()*sizeof(float), pData->GetNormalListPointer());
		glVertexAttribPointer(attribPointerIndex, 3, GL_FLOAT, GL_TRUE, 0, (const GLvoid *)offset);
		glEnableVertexAttribArray(attribPointerIndex);
		ASSERT_GL_CALL();
	}


	return true;
}

//Shader Command
bool	 cRenderer::BindShader(bgShader* pShader, bgMaterial* pMaterial, glm::mat4& cameraViewProjection, glm::mat4& transform, bgPhotoEffectAlgorithm* pPhotoEffect)
{
	if (pShader == 0) return false;

	pShader->Use();

	if (bgShaderAlgorithm* pAlgo = pShader->GetShaderAlgorithms(SHADER_TYPE_VERTEX)){
		pAlgo->UniformProperties.setProperty(world_matrix, cameraViewProjection);
		pAlgo->UniformProperties.setProperty(model_matrix, transform);
	}
	pShader->SetTextureProperty(pMaterial, pPhotoEffect);
	pShader->BindProperties();

	ASSERT_GL_CALL();
	return true;
}

bool cRenderer::BindMaterial(bgMaterial* pMaterial)
{
	if (pMaterial != 0){
		pMaterial->Set(this);
		ASSERT_GL_CALL();
	}
	return true;
}

bool		cRenderer::UnBindMaterial(bgMaterial* pMaterial)
{
	if (pMaterial != 0){
		pMaterial->Reset(this);
		ASSERT_GL_CALL();
	}
	else{
		glDisable(GL_TEXTURE_2D);
	}

	return true;
}

void cRenderer::RenderActor(bgActor* actor)
{
	glBindVertexArray(actor->getVAO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, actor->getIBO());
	ASSERT_GL_CALL();	
	
	eDrawElementType drawType = actor->getDrawType();
	glDrawElements(GetGLDrawType(drawType), actor->getIndexCount(), GL_UNSIGNED_SHORT, NULL);
	//glDrawArrays(GetGLDrawType(drawType), actor->getPrimitiveCount(), GL_UNSIGNED_SHORT, NULL);
}

bool	 cRenderer::UnbindShader(bgShader* pShader)
{
	if (pShader == 0) return false;

	pShader->UnUse();
	
	ASSERT_GL_CALL();
	return true;
}

void  cRenderer::SetMaterial(glm::vec4& Ambient, glm::vec4& Diffuse, glm::vec4& Specular,glm::vec4& Emissive,float Power)
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,	&Ambient.r);	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,		&Diffuse.r);	
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,	&Specular.r);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,	&Emissive.r);
	_ASSERT(0 <= Power && Power <= 128);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Power);
}

void  cRenderer::DrawPrimitive(ePrimitive Primitive, size_t PrimitiveCount, const void* pBuffer, int Stride, eVertex Vertex){
		DrawIndexedPrimitive(Primitive, 0, INDEX_FORMAT_I16, PrimitiveCount, pBuffer, Stride, Vertex);
}

void  cRenderer::DrawIndexedPrimitive(ePrimitive Primitive, const void* pIndex, eIndexFormat IndexFormat, size_t PrimitiveCount, 
																	const void* pBuffer, int Stride, eVertex Vertex)
{
	int VertexStride = Stride;

	const BYTE* pComponent = (const BYTE*)pBuffer;
	
	if(Vertex & VERTEX_XYZ){
		glEnableClientState(GL_VERTEX_ARRAY);//	ASSERT_GL_CALL();
		glVertexPointer(3, GL_FLOAT, VertexStride, pComponent);	//ASSERT_GL_CALL();
		glm::vec3 dummy;
		pComponent += sizeof(dummy);
	}

	if(Vertex & VERTEX_NORMAL){
		glEnableClientState(GL_NORMAL_ARRAY);	
		ASSERT_GL_CALL();
		
		glNormalPointer(GL_FLOAT, VertexStride, pComponent);	
		ASSERT_GL_CALL();
		
		pComponent += sizeof(glm::vec3);
	}

	if(Vertex & VERTEX_DIFFUSE){
		glEnableClientState(GL_COLOR_ARRAY);	//ASSERT_GL_CALL();
		glColorPointer(4, GL_UNSIGNED_BYTE, VertexStride, pComponent);	//ASSERT_GL_CALL();
		pComponent += sizeof(DWORD);
	}

	glm::vec2 dummy;
	pComponent += sizeof(dummy);

	if(pIndex){
		GLenum GLIndexType = (IndexFormat == INDEX_FORMAT_I16)? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
		switch(Primitive){
		case PRIMITIVE_POINTS:				glDrawElements(GL_POINTS,				int(PrimitiveCount),			GLIndexType, pIndex);	break;
		case PRIMITIVE_LINES:					glDrawElements(GL_LINES,					int(PrimitiveCount * 2),		GLIndexType, pIndex);	break;
		case PRIMITIVE_LINE_STRIP:			glDrawElements(GL_LINE_STRIP,			int(PrimitiveCount + 1),		GLIndexType, pIndex);	break;
		case PRIMITIVE_TRIANGLES:			glDrawElements(GL_TRIANGLES,			int(PrimitiveCount * 3),		GLIndexType, pIndex);	break;
		case PRIMITIVE_TRIANGLE_STRIP:	glDrawElements(GL_TRIANGLE_STRIP,	int(PrimitiveCount + 2),		GLIndexType, pIndex);	break;
		case PRIMITIVE_TRIANGLE_FAN:		glDrawElements(GL_TRIANGLE_FAN,		int(PrimitiveCount + 2),		GLIndexType, pIndex);	break;
		case PRIMITIVE_QUADS:				glDrawElements(GL_QUADS,				int(PrimitiveCount/2),		GLIndexType, pIndex);   break;
		case PRIMITIVE_QUADS_STRIP:		glDrawElements(GL_QUADS,				int(PrimitiveCount/2),		GLIndexType, pIndex);	break;
		default:
			_ASSERT(FALSE);
			break;
		}
		ASSERT_GL_CALL();
	}else{
		switch(Primitive){
		case PRIMITIVE_POINTS:				glDrawArrays(GL_POINTS,			0, int(PrimitiveCount));		break;
		case PRIMITIVE_LINES:					glDrawArrays(GL_LINES,			0, int(PrimitiveCount * 2));	break;
		case PRIMITIVE_LINE_STRIP:			glDrawArrays(GL_LINE_STRIP,		0, int(PrimitiveCount+ 1));		break;
		case PRIMITIVE_TRIANGLES:			glDrawArrays(GL_TRIANGLES,		0, int(PrimitiveCount * 3));	break;
		case PRIMITIVE_TRIANGLE_STRIP:	glDrawArrays(GL_TRIANGLE_STRIP,	0, int(PrimitiveCount + 2));	break;
		case PRIMITIVE_TRIANGLE_FAN:		glDrawArrays(GL_TRIANGLE_FAN,	0, int(PrimitiveCount + 2));	break;
		case PRIMITIVE_QUADS:				glDrawArrays(GL_QUADS,			0, int(PrimitiveCount/2));		break;
		case PRIMITIVE_QUADS_STRIP:		glDrawArrays(GL_QUADS,			0, int(PrimitiveCount/2));		break;
		default:
			_ASSERT(FALSE);
			break;
		}
	//	ASSERT_GL_CALL();
	}

	
	if(Vertex & VERTEX_XYZ)
		glDisableClientState(GL_VERTEX_ARRAY);
	if(Vertex & VERTEX_NORMAL)	
		glDisableClientState(GL_NORMAL_ARRAY);
	if(Vertex & VERTEX_DIFFUSE)
		glDisableClientState(GL_COLOR_ARRAY);

	
//	ASSERT_GL_CALL();
}

bool  cRenderer::WGLisExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;

	PROC wglGetExtString = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtString)
		supported = ((char*(__stdcall*)(HDC))wglGetExtString)(wglGetCurrentDC());

	if (supported == NULL)
		supported = (char*)glGetString(GL_EXTENSIONS);

	if (supported == NULL)
		return false;

	for (const char* p = supported; ; p++)
	{
		p = strstr(p, extension);

		if (p == NULL)
			return false;		

		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' '))
			return true;		
	}
}

void  cRenderer::RenderFBOTest(bgFBOTexture& FrameBuffer)
{
	 glBindTexture(GL_TEXTURE_2D, FrameBuffer.TextureID);

	 FrameBuffer.Set(0, this);

	 glBegin(GL_QUADS);
       // glColor4f(1, 1, 1, 1);
		//cMaterial Material;
		//Material.Set(*this);

        // face v0-v1-v2-v3
        glNormal3f(0,0,1);
        glTexCoord2f(1, 1);  glVertex3f(10,10,10);
        glTexCoord2f(0, 1);  glVertex3f(-10,10,10);
        glTexCoord2f(0, 0);  glVertex3f(-10,-10,10);
        glTexCoord2f(1, 0);  glVertex3f(10,-10,10);

        // face v0-v3-v4-v5
        glNormal3f(1,0,0);
        glTexCoord2f(0, 1);  glVertex3f(10,10,10);
        glTexCoord2f(0, 0);  glVertex3f(10,-10,10);
        glTexCoord2f(1, 0);  glVertex3f(10,-10,-10);
        glTexCoord2f(1, 1);  glVertex3f(10,10,-10);

        // face v0-v5-v6-v1
        glNormal3f(0,1,0);
        glTexCoord2f(1, 0);  glVertex3f(10,10,10);
        glTexCoord2f(1, 1);  glVertex3f(10,10,-10);
        glTexCoord2f(0, 1);  glVertex3f(-10,10,-10);
        glTexCoord2f(0, 0);  glVertex3f(-10,10,10);

        // face  v1-v6-v7-v2
        glNormal3f(-1,0,0);
        glTexCoord2f(1, 1);  glVertex3f(-10,10,10);
        glTexCoord2f(0, 1);  glVertex3f(-10,10,-10);
        glTexCoord2f(0, 0);  glVertex3f(-10,-10,-10);
        glTexCoord2f(1, 0);  glVertex3f(-10,-10,10);

        // face v7-v4-v3-v2
        glNormal3f(0,-1,0);
        glTexCoord2f(0, 0);  glVertex3f(-10,-10,-10);
        glTexCoord2f(1, 0);  glVertex3f(10,-10,-10);
        glTexCoord2f(1, 1);  glVertex3f(10,-10,10);
        glTexCoord2f(0, 1);  glVertex3f(-10,-10,10);

        // face v4-v7-v6-v5
        glNormal3f(0,0,-1);
        glTexCoord2f(0, 0);  glVertex3f(10,-10,-10);
        glTexCoord2f(1, 0);  glVertex3f(-10,-10,-10);
        glTexCoord2f(1, 1);  glVertex3f(-10,10,-10);
        glTexCoord2f(0, 1);  glVertex3f(10,10,-10);
    glEnd();

	FrameBuffer.Reset(0, this);
	glBindTexture(GL_TEXTURE_2D, 0);
}
