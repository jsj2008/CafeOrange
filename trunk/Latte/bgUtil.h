#pragma once
#include "GlobalEnum.h"
#include "glm.hpp"
#include <string>
#include <vector>

using namespace std;

static int TypeFromTextureExtension(const std::string& ext)
{
	int type = 0;
	if (ext == "bmp")
		type = SOIL_IMAGE_READ_BMP;
	else if (ext == "jpg" || ext == "jpeg")
		type = SOIL_IMAGE_READ_JPG;
	else if (ext == "png")
		type = SOIL_IMAGE_READ_PNG;
	else if (ext == "tga")
		type = SOIL_IMAGE_READ_TGA;
	else if (ext == "dds")
		type = SOIL_IMAGE_READ_DDS;
	else if (ext == "psd")
		type = SOIL_IMAGE_READ_PSD;
	else if (ext == "hdr")
		type = SOIL_IMAGE_READ_HDR;
	else
		type = SOIL_IMAGE_READ_UNSUPPORTED;

	return type;
}

static std::string FindExtension(const std::string& name)
{
	int len = name.length(); //name.GetLength();
	int i;
	for (i = len - 1; i >= 0; i--){
		if (name[i] == '.'){
			return   name.substr(i + 1, len); // name.Mid(i + 1);
		}
	}
	return std::string("");
}

static std::string FindFileName(const std::string& name)
{
	int len = name.length(); //name.GetLength();
	int i;
	for (i = len - 1; i >= 0; i--)
	{
		if (name[i] == '\\' || name[i] == '/')
		{
			return   name.substr(i + 1, len); // name.Mid(i + 1);
		}
	}

	return std::string("");
}

static  bool MakeGlmVec3List(std::vector<float>& InFloatList, std::vector<glm::vec3>& OutVec3List)
{
	if (InFloatList.size() % 3 != 0)
		return false;

	for (int i = 0; i < InFloatList.size(); i += 3)
	{
		glm::vec3 vector(InFloatList[i], InFloatList[i + 1], InFloatList[i + 2]);
		OutVec3List.push_back(vector);
	}

	return true;
}