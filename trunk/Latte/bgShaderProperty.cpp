#include "stdafx.h"
#include "bgShaderProperty.h"


bgShaderProperty::bgShaderProperty()
{
}


bgShaderProperty::~bgShaderProperty()
{

}


void bgShaderProperty::initialize()
{
	MatrixPropertyMap.clear();
	Vec4PropertyMap.clear();
	Vec3PropertyMap.clear();
	Vec2PropertyMap.clear();
	FloatPropertyMap.clear();
}

void bgShaderProperty::bindProperties(map<string, GLuint>& propertyLocMap)
{

}


bool bgShaderProperty::setProperty(const std::string& name, glm::mat4& mat4)
{
	_MatrixPropertyMap::iterator iMatrix = MatrixPropertyMap.find(name);
	if (iMatrix == MatrixPropertyMap.end())
		MatrixPropertyMap.insert(std::make_pair(name, mat4));
	else
		MatrixPropertyMap[name] = mat4;
	return true;
}

bool  bgShaderProperty::setProperty(const std::string& name, glm::vec4& vec4)
{
	_Vec4PropertyMap::iterator iVec4= Vec4PropertyMap.find(name);
	if (iVec4 == Vec4PropertyMap.end())
		Vec4PropertyMap.insert(std::make_pair(name, vec4));
	else
		Vec4PropertyMap[name] = vec4;

	return true;
}

bool  bgShaderProperty::setProperty(const std::string& name, glm::vec3& vec3)
{
	_Vec3PropertyMap::iterator iVec3 = Vec3PropertyMap.find(name);
	if (iVec3 == Vec3PropertyMap.end())
		Vec3PropertyMap.insert(std::make_pair(name, vec3));
	else
		Vec3PropertyMap[name] = vec3;

	return true;
}

bool  bgShaderProperty::setProperty(const  std::string& name, float value)
{
	_FloatPropertyMap::iterator iFloat = FloatPropertyMap.find(name);
	if (iFloat == FloatPropertyMap.end())
		FloatPropertyMap.insert(std::make_pair(name, value));
	else
		FloatPropertyMap[name] = value;

	return true;
}

bool bgShaderProperty::setProperty(const std::string& name, int value)
{
	_IntPropertyMap::iterator iInt = IntPropertyMap.find(name);
	if (iInt == IntPropertyMap.end())
		IntPropertyMap.insert(std::make_pair(name, value));
	else
		IntPropertyMap[name] = value;

	return true;
}

glm::mat4  bgShaderProperty::getMatrixProperty(std::string& name)
{	
	_MatrixPropertyMap::iterator iMatrix = MatrixPropertyMap.find(name);
	if (iMatrix != MatrixPropertyMap.end())
		return iMatrix->second; 

	ASSERT(0);
	return glm::mat4();
}

glm::vec4  bgShaderProperty::getVec4Property(std::string& name)
{
	_Vec4PropertyMap::iterator iVec4 = Vec4PropertyMap.find(name);
	if (iVec4 != Vec4PropertyMap.end())
		return iVec4->second;
	 
	ASSERT(0);
	 return glm::vec4();
}

glm::vec3 bgShaderProperty::getVec3Property(std::string& name)
{
	_Vec3PropertyMap::iterator iVec3 = Vec3PropertyMap.find(name);
	if (iVec3 != Vec3PropertyMap.end())
		return iVec3->second;

	ASSERT(0);
	 return glm::vec3();
}

glm::vec2 bgShaderProperty::getVec2Property(std::string& name)
{
	_Vec2PropertyMap::iterator iVec2 = Vec2PropertyMap.find(name);
	if (iVec2 != Vec2PropertyMap.end())
		return iVec2->second;
	
	ASSERT(0);
	return glm::vec2();
}

float bgShaderProperty::getFloatProperty(std::string& name)
{
	_FloatPropertyMap::iterator iFloat = FloatPropertyMap.find(name);
	if (iFloat != FloatPropertyMap.end())
		return iFloat->second;

	ASSERT(0);
	return 0.0f;
}

int bgShaderProperty::getIntProperty(std::string& name)
{
	_IntPropertyMap::iterator iInt=IntPropertyMap.find(name);
	if (iInt != IntPropertyMap.end())
		return iInt->second;

	ASSERT(0);
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////
/////bgAttributeShaderProperty
bgAttributeShaderProperty::bgAttributeShaderProperty()
{

}

bgAttributeShaderProperty::~bgAttributeShaderProperty()
{

}

void bgAttributeShaderProperty::initialize()
{
	bgShaderProperty::initialize();
}

void bgAttributeShaderProperty::bindProperties(map<string, GLuint>& propertyLocMap)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
/////bgUniformShaderProperty
bgUniformShaderProperty::bgUniformShaderProperty()
{

}

bgUniformShaderProperty::~bgUniformShaderProperty()
{

}

void bgUniformShaderProperty::initialize()
{
	bgShaderProperty::initialize();
}

void bgUniformShaderProperty::bindProperties(map<string, GLuint>& propertyLocMap)
{
	map<string, GLuint>::iterator iLoc = propertyLocMap.begin();
	_MatrixPropertyMap::iterator iMatrix = MatrixPropertyMap.begin();
	for (; iMatrix != MatrixPropertyMap.end(); iMatrix++)
	{
			iLoc =  propertyLocMap.find(iMatrix->first);
			if (iLoc != propertyLocMap.end())
			{
				int loc = iLoc->second;
				glm::mat4 mat4 = iMatrix->second;
				glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
			}
	}

	_Vec4PropertyMap::iterator iVec4 = Vec4PropertyMap.begin();
	for (; iVec4 != Vec4PropertyMap.end(); iVec4++)
	{
		iLoc = propertyLocMap.find(iVec4->first);
		if (iLoc != propertyLocMap.end())
		{
			int loc = iLoc->second;
			glm::vec4 vec4 = iVec4->second;
			glUniform4fv(loc, 1, glm::value_ptr(vec4));
		}
	}

	_Vec3PropertyMap::iterator iVec3 = Vec3PropertyMap.begin();
	for (; iVec3 != Vec3PropertyMap.end(); iVec3++)
	{
		iLoc = propertyLocMap.find(iVec3->first);
		if (iLoc != propertyLocMap.end())
		{
			int loc = iLoc->second;
			glm::vec3 vec3 = iVec3->second;
			glUniform3fv(loc, 1, glm::value_ptr(vec3));
		}
	}

	_Vec2PropertyMap::iterator iVec2 = Vec2PropertyMap.begin();
	for (; iVec2 != Vec2PropertyMap.end(); iVec2++)
	{
		iLoc = propertyLocMap.find(iVec2->first);
		if (iLoc != propertyLocMap.end())
		{
			int loc = iLoc->second;
			glm::vec2 vec2 = iVec2->second;
			glUniform2fv(loc, 1, glm::value_ptr(vec2));
		}
	}

	_FloatPropertyMap::iterator iFloat = FloatPropertyMap.begin();
	for (; iFloat != FloatPropertyMap.end(); iFloat++)
	{
		iLoc = propertyLocMap.find(iFloat->first);
		if (iLoc != propertyLocMap.end())
		{
			int loc = iLoc->second;
			float  value = iFloat->second;
			glUniform1fv(loc, 1, &value);
		}
	}

	_IntPropertyMap::iterator iInt = IntPropertyMap.begin();
	for (; iInt != IntPropertyMap.end(); iInt++)
	{
		iLoc = propertyLocMap.find(iInt->first);
		if (iLoc != propertyLocMap.end())
		{
			std::string name = iLoc->first;
			int loc = iLoc->second;
			GLint  value = iInt->second;
			glUniform1i(loc, value);
		}
	}


}



