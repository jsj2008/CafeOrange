#pragma once
#include "glm.hpp"
#include "gtc\\matrix_transform.hpp"
#include "Model.h"

//#ifdef _DEBUG
#define ASSERT_GL_CALL()	{ do { int GLError = glGetError(); if(GLError!=0){ ATLTRACE(_T("GLError:%08X: %s \n"), GLError,gluErrorString(GLError));ASSERT(0); }}while(false); }
//#endif

class bgActor;
class bgData;
class bgShader;
class cLight;
class cBoundingBox;
class bgPhotoEffectAlgorithm;


class cRenderer
{
public:
	enum eVertex
	{
		VERTEX_XYZ			= 1 << 0,
		VERTEX_NORMAL	= 1 << 1,
		VERTEX_DIFFUSE		= 1 << 2,
		VERTEX_TEX0		= 1 << 3,

		VERTEX_XYZ_DIFFUSE				= VERTEX_XYZ | VERTEX_DIFFUSE,
		VERTEX_XYZ_NORMAL_DIFFUSE	= VERTEX_XYZ | VERTEX_NORMAL | VERTEX_DIFFUSE,
		VERTEX_XYZ_TEX0					= VERTEX_XYZ | VERTEX_TEX0,
		VERTEX_XYZ_NORMAL_TEX0		= VERTEX_XYZ | VERTEX_NORMAL | VERTEX_TEX0,
	};

	enum eIndexFormat
	{
		INDEX_FORMAT_I16,
		INDEX_FORMAT_I32,
	};

	enum ePrimitive
	{
		PRIMITIVE_POINTS,
		PRIMITIVE_LINES,
		PRIMITIVE_LINE_STRIP,
		PRIMITIVE_TRIANGLES,
		PRIMITIVE_TRIANGLE_STRIP,
		PRIMITIVE_TRIANGLE_FAN,
		PRIMITIVE_QUADS,
		PRIMITIVE_QUADS_STRIP,
	};


	struct  _VBO
	{
		GLuint  vboVB;
		GLsizei size;			 
	};

	struct _IBO
	{
		GLuint vboIB;

		uint nIndices;
		uint size;
	};

	std::vector<cCVertices>	Gizmos;

	bool  	CreateDevice(HWND hWnd);
	void	Initialize();
	bool  	MakeCurrent();
	void  	Clear();
	void  	Present();
	void  	Destory();




	void		SetViewport(int width, int height);
	//Material
	void		SetMaterial(glm::vec4& Ambient, glm::vec4& Diffuse, glm::vec4& Specular,glm::vec4& Emissive,float Power);
	//Render Bounding Box
	void		DrawBoudingBox(cBoundingBox& BoundingBox,glm::vec3& Position,glm::vec3& Rotation,glm::vec3& Scale);
	//Render Depth Texture
	void		RenderDepthTexture();

	//Generate opengl buffer
	bool		GenerateGLBuffer(const bgData* pData, GLuint &VAO, GLuint &VBO, GLuint &IBO);

	//Shader Command
	bool		BindShader(bgShader* pShader, bgMaterial* pMaterial,glm::mat4& cameraViewProjection, glm::mat4& transform,bgPhotoEffectAlgorithm* pPhotoEffect = 0);
	bool		UnbindShader(bgShader* pShader);
	//Material Command
	bool		BindMaterial(bgMaterial* pMaterial);
	bool		UnBindMaterial(bgMaterial* pMaterial);
	//Render Option
	void		SetFillMode(eFillMode mode);
	void		SetShadingModel(eShadingModel model);

	//Render Command - deprecated
	void		DrawCircle();
	void		DrawLight(cLight& Light);
	void		DrawPrimitive(ePrimitive Primitive, size_t PrimitiveCount, const void* pBuffer, int Stride, eVertex Vertex);
	void		DrawIndexedPrimitive(ePrimitive Primitive, const void* pIndex, eIndexFormat IndexFormat, size_t PrimitiveCount, 
														const void* pBuffer, int Stride, eVertex Vertex);

	//Render Command - modern
	void		RenderActor(bgActor* actor);

	//FBO Test
	void		RenderFBOTest(bgFBOTexture& FrameBuffer);

	cRenderer();
	virtual ~cRenderer();

protected:
	void	BuildGizmos();
	void	BuildCircleXGizmo(cCVertices& Vertices);
	void	BuildCircleYGizmo(cCVertices& Vertices);
	void	BuildCircleZGizmo(cCVertices& Vertices);

	int		SetWidowPixelFormat(HDC hDC);
	bool		InitMultiSample(HWND hWnd);
	bool		WGLisExtensionSupported(const char *extension);

private:
	HDC		 hRenderingDC;
	HGLRC	 hRenderingRC;
	HWND	 OwnerHwnd;
	bool			bMultisampleSupported;
	int			MultisampleFormat;
};

