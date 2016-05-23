#include "StdAfx.h"
#include "Renderer.h"
#include "Camera.h"


cCamera::cCamera()
{
	
}


cCamera::~cCamera()
{

}

void cCamera::Init(eProjection projection)
{
	CameraPosition = CP_USER_POSITION;
	Projection = projection;

	if (projection == PROJECTION_PERSPECTIVE)
		Eye = glm::vec3(0.0f, 3.0f, 5.0f);
	else
		Eye = glm::vec3(0.0f, 0.0f, 5.0f);

	InitEye = Eye;
	LookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	Up		 = glm::vec3(0.0f, 1.0f, 0.0f);	
	FovY		 = 45.0f;

	if (projection == PROJECTION_PERSPECTIVE)
	{
		NearPlane = 1.0f;
		FarPlane = 1000.0f;
	}
	else
	{
		NearPlane = -100.0f;
		FarPlane = 100.0f;
	}

	//bInit = false;
}

void cCamera::SetView()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	worldMat4 = glm::lookAt(Eye, LookAt, Up);
	gluLookAt(Eye.x, Eye.y, Eye.z, LookAt.x, LookAt.y, LookAt.z, Up.x,  Up.y, Up.z);
}


void cCamera::SetGizmoView(glm::vec3 EyePosition)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(EyePosition.x, EyePosition.y, EyePosition.z, LookAt.x, LookAt.y, LookAt.z, Up.x,  Up.y, Up.z);
}

void cCamera::Reset(cRenderer& Renderer, int Width, int Height)
{
	if(Width || Height){
		//ScreenSize.Set(Width, Height);
	}

	if (Height == 0){
		Aspect = (float)Width;
	}else{
		Aspect = (float)Width/(float)Height;
	}

	WindowSize.x = Width; WindowSize.y = Height;
	Renderer.SetViewport(Width, Height);

	glGetIntegerv(GL_VIEWPORT, glm::value_ptr(Viewport));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (Projection == PROJECTION_ORTHOGONAL)
	{
		//GLdouble left = -(GLdouble)Width / 2;
		//GLdouble right = (GLdouble)Width / 2;
		//GLdouble bottom = -(GLdouble)Height / 2;
		//GLdouble top = (GLdouble)Height / 2;
		//glOrtho(left, right, bottom, top, NearPlane, FarPlane);
		glOrtho(-1, 1, -1, 1, NearPlane, FarPlane);
		projectionMat4 = glm::ortho(-1, 1, -1, 1);	
	}
	else
	{
		gluPerspective(FovY, Aspect, NearPlane, FarPlane);
		projectionMat4 = glm::perspective(FovY, Aspect, NearPlane, FarPlane);
	}
	//glOrtho(left, right, bottom, top, NearPlane, FarPlane);	
	//projectionMat4 = glm::ortho(-1, 1, -1, 1);

	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
}

void cCamera::SetEyePos(glm::vec3 Position,bool bInit)
{
	Eye = Position;

	if(bInit){
		InitEye = Eye;
		//bInit = true;
	}
	SetView();
}


void cCamera::RotateOnXYAxis(float X_Diff,float Y_Diff)
{
	glm::mat4x4 RotateMat;

	RotateMat = glm::rotate(RotateMat, X_Diff, glm::vec3(1.0f, 0.0f, 0.0f));
	RotateMat = glm::rotate(RotateMat, Y_Diff, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec4 EyePos = glm::vec4(Eye.x,Eye.y,Eye.z, 1.0f);
	EyePos = RotateMat* EyePos;

	Eye = glm::vec3(EyePos);

	SetView();
}
