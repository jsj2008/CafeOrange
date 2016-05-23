#pragma once
#include "../glm-0.9.6.3/glm/glm/glm.hpp"

class cRenderer;
class cCamera
{
public:
	enum eCameraPosition
	{
		CP_USER_POSITION=0,
		CP_FRONT,
		CP_BACK,
		CP_TOP,
		CP_BOTTOM,
		CP_LEFT,
		CP_RIGHT,	
	};

	enum eProjection
	{
		PROJECTION_PERSPECTIVE,
		PROJECTION_ORTHOGONAL,
	};

	void  Init(eProjection projection);
	void  SetView(); 	
	void  Reset(cRenderer& Renderer,int Width=0, int Height=0);

	void	SetGizmoView(glm::vec3 EyePosition);

	void	SetEyePos(glm::vec3 Position,bool bInit = false);
	void	RotateOnXYAxis(float X_Diff,float Y_Diff);

	glm::vec3 GetEyePos() { return Eye; }
	glm::vec3 GetLookAtPos() { return LookAt; }
	glm::vec3 GetInitEyePos() { return InitEye; }
	glm::vec2  GetWindowSize() { return WindowSize;  }

	glm::vec3 GetPickingDirection() { return glm::normalize(LookAt - Eye); };

	glm::vec4 GetViewPort() { return Viewport; }

	const glm::mat4& GetProjectionMatrix() { return projectionMat4; }
	const glm::mat4& GetWorldViewMatrix() { return worldMat4;  }
	const glm::mat4& GetMVP(){ return projectionMat4*worldMat4; }
	void SetWorldViewMatrix(const glm::mat4& worldMat) { worldMat4 = worldMat;  }

	eProjection GetProjection(){ return Projection; }
	void SetProjection(eProjection projection){ Projection = projection; }

	cCamera();
	virtual ~cCamera();

protected:
	float		FovY;
	float		Aspect;
	float		NearPlane;
	float		FarPlane;
	float		FocalLength;
	//int		Viewport[4];

	glm::vec3 Eye;
	glm::vec3 LookAt;
	glm::vec3 Up;

	glm::vec3 InitEye;


	glm::vec2  WindowSize;

	glm::mat4 projectionMat4;
	glm::mat4 worldMat4;
	glm::ivec4 Viewport;

	//bool		bInit;
	eCameraPosition	 CameraPosition;
	eProjection		 Projection;

};

