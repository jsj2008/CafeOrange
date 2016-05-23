#pragma once
#include <map>
#include <string>
#include "ShaderStrings.h"
using namespace std;

class bgShaderProperty
{
public:
	bgShaderProperty();
	~bgShaderProperty();

	typedef map<std::string, glm::mat4> _MatrixPropertyMap;
	typedef map<std::string, glm::vec4>	_Vec4PropertyMap;
	typedef map<std::string, glm::vec3>	_Vec3PropertyMap;
	typedef map<std::string, glm::vec2>  _Vec2PropertyMap;
	typedef map<std::string, float>			_FloatPropertyMap;
	typedef map<std::string, int>				_IntPropertyMap;

	_MatrixPropertyMap		MatrixPropertyMap;
	_Vec4PropertyMap		Vec4PropertyMap;
	_Vec3PropertyMap		Vec3PropertyMap;
	_Vec2PropertyMap		Vec2PropertyMap;
	_FloatPropertyMap		FloatPropertyMap;
	_IntPropertyMap			IntPropertyMap;

	virtual void initialize();
	virtual void bindProperties(map<string, GLuint>& propertyLocMap);

	bool setProperty(const std::string& name, glm::mat4& mat4);
	bool setProperty(const std::string& name, glm::vec4& vec4);
	bool setProperty(const std::string& name, glm::vec3& vec3);
	bool setProperty(const std::string& name, float value);
	bool setProperty(const std::string& name, int value);

	glm::mat4 getMatrixProperty(std::string& name);
	glm::vec4  getVec4Property(std::string& name);
	glm::vec3 getVec3Property(std::string& name);
	glm::vec2 getVec2Property(std::string& name);
	float getFloatProperty(std::string& name);
	int	 getIntProperty(std::string& name);
};

////////////////////////////////////////////////////////////////////////////////////////////////
/////bgAttributeShaderProperty
class bgAttributeShaderProperty : public bgShaderProperty
{
public:
	bgAttributeShaderProperty();
	virtual ~bgAttributeShaderProperty();

	virtual void initialize();
	virtual void bindProperties(map<string, GLuint>& propertyLocMap);
};


////////////////////////////////////////////////////////////////////////////////////////////////
/////bgUniformShaderProperty
class bgUniformShaderProperty : public bgShaderProperty
{
public:
	bgUniformShaderProperty();
	virtual ~bgUniformShaderProperty();

	virtual void initialize();
	virtual void bindProperties(map<string, GLuint>& propertyLocMap);
};
