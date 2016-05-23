#include "stdafx.h"
#include "Renderer.h"
#include "bgTexture.h"


bgTexture::bgTexture()
{
	TextureID = 0;
	Type = TYPE_2D;
	TextureWidth = 0 ;
	TextureHeight = 0;
}

bgTexture::~bgTexture()
{
}

void	bgTexture::Set(size_t StageID, cRenderer* pRenderer)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GLenum(GL_TEXTURE0 + StageID));
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//glEnable(GL_TEXTURE_2D);
}

void	bgTexture::Reset(size_t StageID, cRenderer* pRenderer)
{
	glActiveTexture(GLenum(GL_TEXTURE0 + StageID));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

void bgTexture::UnSet(cRenderer* pRenderer)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

bool bgTexture::ConvertOpenCVMat(cv::Mat& Frame)
{
	GLenum inputFormat = GL_BGR;
	if (Frame.channels() == 1)
		inputFormat = GL_LUMINANCE;

	TextureWidth = Frame.cols;
	TextureHeight = Frame.rows;

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	
	glTexImage2D(GL_TEXTURE_2D,     
		0,							
		GL_RGB,            
		Frame.cols,        
		Frame.rows,      
		0,                
		inputFormat, 
		GL_UNSIGNED_BYTE,  
		Frame.ptr());       

	return true;
}

///////////////////////////////////////////////////////////////////////////////
/// bgFBOTexture
bgFBOTexture::bgFBOTexture()
{
	FrameBufferID = -1;
	RenderBufferID = -1;
}

bgFBOTexture::~bgFBOTexture()
{
	ReleaseFrameBuffer();
}

bool bgFBOTexture::CreateFrameBuffer(eType _Type, int Width, int Height)
{
	Type = _Type;
	TextureWidth = Width;
	TextureHeight = Height;
	///////////////////////////////////////////////////////////////////////////
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (Type)
	{
	case FBO_TYPE_COLOR:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		break;
	case FBO_TYPE_DEPTH:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	///////////////////////////////////////////////////////////////////////////

	glGenFramebuffersEXT(1, &FrameBufferID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FrameBufferID);

	glGenRenderbuffersEXT(1, &RenderBufferID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, RenderBufferID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, Width, Height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	switch (Type)
	{
	case FBO_TYPE_COLOR:
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, TextureID, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, RenderBufferID);
		break;
	case FBO_TYPE_DEPTH:
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, TextureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		break;
	}

	bool bStatus = CheckFrameBufferStatus();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	return bStatus;
}
bool bgFBOTexture::ReleaseFrameBuffer()
{
	glDeleteTextures(1, &TextureID);

	glDeleteFramebuffersEXT(1, &FrameBufferID);
	glDeleteRenderbuffersEXT(1, &RenderBufferID);

	TextureID = -1;
	FrameBufferID = -1;
	RenderBufferID = -1;

	return true;
}

bool bgFBOTexture::BindFrameBuffer()
{
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, FrameBufferID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, RenderBufferID);

	switch (Type)
	{
	case FBO_TYPE_COLOR:
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, TextureID, 0);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, RenderBufferID);
		break;
	case FBO_TYPE_DEPTH:
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, TextureID, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		break;
	}

	return CheckFrameBufferStatus();
}
bool bgFBOTexture::UnBindFrameBuffer()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	return true;
}

bool bgFBOTexture::CheckFrameBufferStatus()
{
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	CString Message;

	switch (status)
	{
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		OutputDebugString("[ERROR] Framebuffer incomplete: Attachment is NOT complete.\r\n");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		OutputDebugString("[ERROR] Framebuffer incomplete: No image is attached to FBO.\r\n");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		OutputDebugString("[ERROR] Framebuffer incomplete: Attached images have different dimensions.\r\n");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		OutputDebugString("[ERROR] Framebuffer incomplete: Color attached images have different internal formats.\r\n");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		OutputDebugString("[ERROR] Framebuffer incomplete: Draw buffer.\r\n");
		return false;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		OutputDebugString("[ERROR] Framebuffer incomplete: Read buffer. \r\n");
		return false;

	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		OutputDebugString("[ERROR] Unsupported by FBO implementation. \r\n");
		return false;

	default:
		OutputDebugString("[ERROR] Unknow error. \r\n");
		return false;
	}
}

void bgFBOTexture::SaveFrameBufferToFile(const std::string& fileName, int width, int height)
{
	GLubyte *buffer = new GLubyte[width* height * 3];
	memset(buffer, 0, width* height * 3);
	if (BindFrameBuffer()){
		glReadBuffer((GLenum)GL_COLOR_ATTACHMENT0_EXT);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)buffer);
		UnBindFrameBuffer();
	}

	SOIL_save_image(fileName.c_str(), SOIL_SAVE_TYPE_BMP, width, height, 3, buffer);

	//delete[] buffer;
}