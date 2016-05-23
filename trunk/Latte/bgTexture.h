#pragma once
#include <GL/glut.h>
#include <vector>
#include <string>
#include "opencv2/core.hpp"
using namespace std;

class cRenderer;


class bgTexture
{
public:
	enum eType{
		TYPE_1D,
		TYPE_2D,
		TYPE_3D,
	};

	enum eOperation{
		OP_DISABLE,
		OP_SELECTARG1,
		OP_SELECTARG2,
		OP_MODULATE,
		OP_ADD,
		OP_ADD_SIGNED,
		OP_INTERPOLATE,
		OP_SUBTRACT,
		OP_COUNT,
	};

	enum eTextureFilter{
		TEXTURE_FILTER_NEAREST,
		TEXTURE_FILTER_LINEAR,
		TEXTURE_FILTER_LINEAR_MIPMAP,
		TEXTURE_FILTER_ANISOTROPIC,
		TEXTURE_FILTER_COUNT,
	};

	enum eTextureAddress{
		TEXTURE_ADDRESS_REPEAT,
		TEXTURE_ADDRESS_CLAMP,
		TEXTURE_MIRRORED_REPEAT,
		TEXTURE_CLAMP_EDGE,
		TEXTURE_ADDRESS_COUNT,
	};

	bool		Enabled;
	eType						Type;
	eOperation				OperationType;
	eTextureFilter			TextureFilter;
	eTextureAddress	TextureAddress;
	std::string				FilePath;

	GLuint			TextureID;

	int TextureWidth;
	int TextureHeight;

	void	SetFileName(const std::string& _FilePath){ FilePath = std::string(_FilePath); }
	void	Set(size_t StageID, cRenderer* pRenderer);
	void UnSet(cRenderer* pRenderer);
	void	Reset(size_t StageID, cRenderer* pRenderer);

	bool ConvertOpenCVMat(cv::Mat& Frame);

	bgTexture();
	virtual ~bgTexture();
};

///////////////////////////////////////////////////////////////////////////////
/// bgFBOTexture
class bgFBOTexture : public bgTexture
{
public:
	enum eType{
		FBO_TYPE_COLOR,
		FBO_TYPE_DEPTH,
		FBO_TYPE_STENCIL,
	};

	GLuint	FrameBufferID;
	GLuint	RenderBufferID;

	eType Type;
	bool CreateFrameBuffer(eType _Type, int Width, int Height);
	bool ReleaseFrameBuffer();

	bool CheckFrameBufferStatus();

	virtual bool BindFrameBuffer();
	virtual bool UnBindFrameBuffer();

	GLuint	 GetTextureID(){ return TextureID; }
	const GLuint	 GetTextureID() const{ return TextureID; }

	void SaveFrameBufferToFile(const std::string& fileName, int width, int height);

	bgFBOTexture();
	virtual ~bgFBOTexture();


protected:
};