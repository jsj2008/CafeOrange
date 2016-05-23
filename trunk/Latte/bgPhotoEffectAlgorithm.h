#pragma once 

#include "GlobalEnum.h"
#include "bgTexture.h"
#include "bgShader.h"

class bgActor;
class cCamera;
class bgShaderFactory;
/////////////////////////////////////////////////////////////////////////////////////////////
////bgPhotoEffectAlgorithm
class bgPhotoEffectAlgorithm
{
public:

	virtual bool CreateShader(bgShaderFactory& ShaderFactory) { return false;  }
	virtual GLuint RenderActor(cRenderer& Renderer, cCamera& Camera, bgActor& Actor);
	
	ePhotoEffect GetEffectType() { return effectType;  }

	bgPhotoEffectAlgorithm();
	virtual ~bgPhotoEffectAlgorithm();

protected:	
	ePhotoEffect	 effectType;
	bgShader*		 pShader;
	
private:

};

/////////////////////////////////////////////////////////////////////////////////////////////
////bgAlphaPhotoEffectAlgorithm
class bgAlphaPhotoEffectAlgorithm : public bgPhotoEffectAlgorithm
{
public:
	virtual bool CreateShader(bgShaderFactory& ShaderFactory);

	float GetAlpha(){ return Alpha;  }
	void SetAlpha(float value) { Alpha = value; }

	bgAlphaPhotoEffectAlgorithm();
	virtual ~bgAlphaPhotoEffectAlgorithm();
	
protected:


private:
	float Alpha;

};


/////////////////////////////////////////////////////////////////////////////////////////////
////bgBrightContrastPhotoEffectAlgorithm
class bgBrightContrastPhotoEffectAlgorithm : public bgPhotoEffectAlgorithm
{
public:
	virtual bool CreateShader(bgShaderFactory& ShaderFactory);

	bgBrightContrastPhotoEffectAlgorithm();
	virtual ~bgBrightContrastPhotoEffectAlgorithm();

	float GetBrightness() { return Brightness; }
	void SetBrightness(float value)  { Brightness = value;  }
	float GetContrast() { return Contrast;  }
	void SetContrast(float value) { Contrast = value;  }

protected:
	float Brightness;
	float Contrast;

};

/////////////////////////////////////////////////////////////////////////////////////////////
////bgGrayPhotoEffectAlgorithm
class bgGrayPhotoEffectAlgorithm : public bgPhotoEffectAlgorithm
{
public:
	virtual bool CreateShader(bgShaderFactory& ShaderFactory);

	bgGrayPhotoEffectAlgorithm();
	virtual ~bgGrayPhotoEffectAlgorithm();

	int GetBlendWay(){ return BlendWay; }
	void SetBlendWay(int value){ BlendWay = value; }

protected:
	int	BlendWay;

};
