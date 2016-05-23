#include "StdAfx.h"
#include "Message.h"

LPCTSTR  STRING_PROGRAM_NAME = _T("sung jae fly v0.0.0.1");
LPCTSTR	STRING_MODEL_GROUP  = _T("Models");
LPCTSTR	STRING_LIGHT_GROUP  = _T("Lights");
LPCTSTR	STRING_CAMERA_GROUP = _T("Cameras");
LPCTSTR	STRING_OBJECT 		= _T("object");
LPCTSTR	STRING_MODEL  		= _T("model");
LPCTSTR	STRING_LIGHT  		= _T("light");
LPCTSTR	STRING_CAMERA 		= _T("camera");

LPCTSTR STRING_NAME			= _T("Name");
LPCTSTR STRING_POSITION		= _T("Position");
LPCTSTR STRING_ROTATION		= _T("Rotation");
LPCTSTR STRING_SCALE		= _T("Scale");

LPCTSTR STRING_OBJ_EXTENSION 		= _T("OBJ");
LPCTSTR STRING_3DS_EXTENSION 		= _T("3DS");
LPCTSTR STRING_MD2_EXTENSION 		= _T("MD2");
LPCTSTR STRING_MD3_EXTENSION 		= _T("MD3");
LPCTSTR STRING_COLLADA_EXTENSION	= _T("DAE");
LPCTSTR STRING_STL_EXTENSION		= _T("STL");

LPCTSTR	STRING_BMP_EXTENSION		= _T("BMP");

LPCTSTR STRING_TERRAIN_RAW_FORMAT	= _T("RAW");
LPCTSTR STRING_IMPORT_3D_FILTER = _T("3D Files(*.OBJ;*.3DS;*.DAE;*.MD2;*.MD3;*.STL)|*.OBJ;*.3DS;*.DAE;*.MD2;*.MD3;*.STL|Collada file(*.DAE)|*.DAE|OBJ file(*.OBJ)|*.OBJ|3DS file(*.3DS)|*.3DS|MD2 files(*.MD2)|*.MD2|MD3 files(*.MD3)|*.MD3| STL file(*.STL)|*.STL");
LPCTSTR STRING_IMPORT_IMG_FILTER= _T("Image Files(*.bmp;*.png;*.jpg;*.jpeg)|*.bmp;*.png;*.jpg;*.jpeg|bmp file(*.bmp)|*.bmp|PNG file(*.png)|*.png|JPG file(*.jpg)|*.jpg|JPEG file(*.jpeg)|*.jpeg");

LPCTSTR STRING_SHADING_MODEL[] = { _T("Flat"), _T("Smooth") };
LPCTSTR STRING_FILL_MODE[] = {_T("Wire"), _T("Solid") };
LPCTSTR	STRING_PICKING_MODE[] = { _T("Model"), _T("Face"), _T("Neighboring Face") };

LPCTSTR STRING_OBJECT_TRANS				= _T("Transform");
LPCTSTR STRING_OBJECT_TRANSLATION	= _T("Translation");
LPCTSTR STRING_OBJECT_ROTATION		= _T("Rotation");
LPCTSTR STRING_OBJECT_SCALE				= _T("Scale");

LPCTSTR STRING_MATERIAL_PROPERTY = _T("Material Properties");
LPCTSTR STRING_MATERIAL_COLOR		= _T("Color");
LPCTSTR STRING_MATERIAL_DIFFUSE	= _T("Diffuse");
LPCTSTR STRING_MATERIAL_SPECULAR = _T("Specular");
LPCTSTR STRING_MATERIAL_AMBIENT	= _T("Ambient");
LPCTSTR STRING_MATERIAL_EMISSIVE	= _T("Emissive");
LPCTSTR STRING_MATERIAL_ALPHA		= _T("Alpha");

LPCTSTR  STRING_LIGHT_POSITON					= _T("Light Position");
LPCTSTR	 STRING_SHADOW_POLYGON_OFFSET	= _T("Polygon Offset");
LPCTSTR	 STRING_SHADOW_MAP_SIZE				= _T("Depth Map Size");

LPCTSTR	 STRING_SHADER_PROPERTY  = _T("Shader Properties");
LPCTSTR  STRING_SHADOW_LIGHTNESS = _T("Lightness");
LPCTSTR  STRING_SHADOW_ACCESSIBILITY  = _T("Accessibility");
LPCTSTR  STRING_TEXTURE_NAME = _T("Texture");

LPCTSTR STRING_SCENE_RENDERER						= _T("Renderer");
LPCTSTR STRING_SCENE_RENDERER_FILL			=_T("Fill Mode");
LPCTSTR STRING_SCENE_RENDERER_SHADING = _T("Shading Mode");

LPCTSTR STRING_CANVAS_NAME= _T("Canvas");
LPCTSTR STRING_CANVAS_SIZE[] = { _T("Full"), _T("0.9x"), _T("0.8x"), _T("0.7x"), _T("0.6x"), _T("0.5x"), _T("0.4x"), _T("0.3x") };

LPCTSTR STRING_CANVAS_EFFECT = _T("Shader Effect");
LPCTSTR STRING_CANVAS_MORPH = _T("Morph&Warp");

LPCTSTR STRING_MOVIE_EFFECT[] = { _T("Blur"), _T("Erode") };
LPCTSTR STRING_MOVIE_EFFECT_NAME = _T("Movie Effect");

LPCTSTR STRING_PHOTO_EFFECT[] = { _T("Alpha"), _T("Bright and Contrast"), _T("Gray Scale") };
LPCTSTR STRING_PHOTO_EFFECT_NAME = _T("Photo Effect");
LPCTSTR STRING_PHOTO_EFFECT_GRAY_NAME = _T("Gray Scale Mode");
LPCTSTR STRING_PHOTO_EFFECT_GRAY[] = { _T("Average"), _T("NTSC"), _T("HSV"), _T("Desaturation"), _T("Max Decomposition"), _T("Min Decomposition") };


LPCTSTR STRING_PHOTO_SPECIAL_EFFECT[] = { _T("Matryoshka Album"), _T("2D to 3D Conversion-01") };
LPCTSTR STRING_PHOTO_SPECIAL_EFFECT_NAME = _T("Special Effect");