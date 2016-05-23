#include "StdAfx.h"
#include "Renderer.h"
#include "bgMaterial.h"


GLint GetGLTextureFilter(bgTexture::eTextureFilter Filter)
{
	switch(Filter)
	{
	case bgTexture::TEXTURE_FILTER_NEAREST:					return GL_NEAREST; break;
	case bgTexture::TEXTURE_FILTER_LINEAR:						return GL_LINEAR; break;
	case bgTexture::TEXTURE_FILTER_LINEAR_MIPMAP:		return GL_LINEAR_MIPMAP_LINEAR; break;
	}

	return GL_NEAREST;
}

GLint GetGLTextureAddressMode(bgTexture::eTextureAddress Address)
{
	switch(Address)
	{
	case bgTexture::TEXTURE_ADDRESS_REPEAT:				return GL_REPEAT; break;
	case bgTexture::TEXTURE_ADDRESS_CLAMP:				return GL_CLAMP; break;
	case bgTexture::TEXTURE_MIRRORED_REPEAT:			return GL_MIRRORED_REPEAT; break;
	case bgTexture::TEXTURE_CLAMP_EDGE:					return GL_CLAMP_TO_EDGE; break;
	}

	return GL_REPEAT;
}


///////////////////////////////////////////////////////////////////////////////////////////////
//cMaterial
bgMaterial::bgMaterial()
{	
	Diffuse   = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	Ambient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);	
	Specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Emissive = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Power	= 64.0f;
}

bgMaterial::~bgMaterial()
{

}

void bgMaterial::Set(cRenderer* pRenderer)
{
	//pRenderer->SetMaterial(Ambient, Diffuse, Specular, Emissive, Power);

	if(TextureList.size() >0)
		glEnable(GL_TEXTURE_2D);

	for(size_t iTexture = 0; iTexture < TextureList.size(); iTexture ++){
		bgTexture* pTexture = TextureList[iTexture];
		if(pTexture->Enabled){
			pTexture->Set(iTexture, pRenderer);
		}
	}
}

void bgMaterial::Reset(cRenderer* pRenderer)
{
	for(size_t iTexture = 0; iTexture < TextureList.size(); iTexture ++){
		bgTexture* pTexture = TextureList[iTexture];
		if(pTexture->Enabled){
			pTexture->Reset(iTexture, pRenderer);
		}
	}
	if(TextureList.size() > 0)
		glDisable(GL_TEXTURE_2D);
}

void bgMaterial::UnSet(cRenderer* pRenderer)
{
	
}

void bgMaterial::CreateTexture(bgTexture& Texture)
{
/*
	ASSERT(Texture.TextureID == 0 && Texture.pImage);
	CxImage* pImage = Texture.pImage;
	CBitmap*	pBitmap =  CBitmap::FromHandle(pImage->MakeBitmap());
	BITMAP	BmpInfo;	
	pBitmap->GetBitmap( &BmpInfo );
	
	int Width = BmpInfo.bmWidth;
	int Height = BmpInfo.bmHeight;

	const int BITMAPINFOHEADER_SIZE = sizeof(BITMAPINFOHEADER) ;
	BYTE* abBitmapInfo[BITMAPINFOHEADER_SIZE] ;
	BITMAPINFOHEADER* pBMIH = (BITMAPINFOHEADER*)abBitmapInfo;
	memset(pBMIH, 0, BITMAPINFOHEADER_SIZE);

	// fill in the header info 
	pBMIH->biSize 			= sizeof(BITMAPINFOHEADER);
	pBMIH->biWidth 		= Width;
	pBMIH->biHeight 		= Height;
	pBMIH->biPlanes 		= 1;
	pBMIH->biBitCount 	= 24 ; //32?? Use 32 bpp to avoid boundary alignment issues.
	pBMIH->biCompression= BI_RGB ; 
	
	// Create the new 32 bpp DIB section.
	CDC dc;
	dc.CreateCompatibleDC(NULL);
	BYTE* pBits ;
	HBITMAP hbmBuffer = CreateDIBSection(dc.GetSafeHdc(),
													(BITMAPINFO*)pBMIH,
													DIB_RGB_COLORS,
													(VOID **) &pBits,
													NULL,
													0);
	// Select DIB into DC.
	HBITMAP hbmOld = (HBITMAP)::SelectObject(dc.GetSafeHdc(), hbmBuffer);
	
	int ImageSize = Width * Height * sizeof(cRGBTable);
	Texture.pPicBits = malloc(ImageSize);
	
//	RGBQUAD* pSrc = (RGBQUAD*)pBits ;
	cRGBTable* pRGBTable = (cRGBTable*)Texture.pPicBits;	
	for (int i = 0 ; i < Height; i++)
	{
		for(int j = 0 ; j < Width; j++)
		{
			RGBQUAD rgb = pImage->GetPixelColor(j, i);
			pRGBTable->Red	= rgb.rgbRed;	//R;
			pRGBTable->Green = rgb.rgbGreen;	//G;
			pRGBTable->Blue	= rgb.rgbBlue;	//B;
			pRGBTable++ ;
		}
	}

	if (hbmOld)
		(HBITMAP)::SelectObject(dc.GetSafeHdc(), hbmOld);
	DeleteObject(hbmBuffer) ;
	
	glGenTextures(1, &Texture.TextureID);	
	Texture.Enabled = true;
	//glBindTexture(GL_TEXTURE_2D, Texture.TextureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture.pPicBits);	
	//glBindTexture(GL_TEXTURE_2D, 0);	

	*/
}

bool	 bgMaterial::ChangeTexture(int order, bgTexture* pTexture)
{
	if (TextureList.size() < 0 || TextureList.size() <= order) return false;
	TextureList[order] = pTexture;
}


bool	bgMaterial::LoadTexture(const std::string& FilePath)
{
	int ImageType = TypeFromTextureExtension(FindExtension(FilePath));	
	if (ImageType == SOIL_IMAGE_READ_UNSUPPORTED){
		ASSERT(0);		
		return false;
	}
	
	unsigned int TextureWidth, TextureHeight;
	TextureWidth = TextureHeight = 0;
	GLuint newTexId = SOIL_load_OGL_texture(FilePath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y, &TextureWidth, &TextureHeight);
	if (newTexId == 0 || TextureWidth == 0 || TextureHeight == 0){
		ASSERT(0);
		//observer message ...
		return false;
	}

	bgTexture* pTexture = new bgTexture();
	pTexture->SetFileName(FilePath);
	pTexture->TextureID = newTexId;
	pTexture->TextureWidth = TextureWidth;
	pTexture->TextureHeight = TextureHeight;

	TextureList.push_back(pTexture);

	return true;
}

bool		bgMaterial::DeleteTexture(const std::string& FilePath)
{
	bgTexture* pTexture = 0;
	_TextureList::iterator iTexture = TextureList.begin();
	for (; iTexture != TextureList.end(); iTexture++)
	{
		pTexture = *iTexture;
		std::string filename = FindFileName(pTexture->FilePath);
		if (filename.compare(FilePath) == 0)
			break;
	}

	if (iTexture == TextureList.end())
		return false;

	delete pTexture;
	TextureList.erase(iTexture);

	return true;
}