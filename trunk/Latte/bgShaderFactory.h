#pragma once

#include "GlobalEnum.h"
#include <map>
using namespace std;

class bgShaderAlgorithm;

class bgShaderFactory
{
public:
	bgShaderFactory();
	~bgShaderFactory();

	typedef map<int, bgShaderAlgorithm*> _VertexShaderList;
	typedef map<int, bgShaderAlgorithm*> _TessellationShaderList;
	typedef map<int, bgShaderAlgorithm*> _GeometryShaderList;
	typedef map<int, bgShaderAlgorithm*> _FragmentShaderList;
	typedef map<int, bgShaderAlgorithm*> _ComputeShaderList;

	_VertexShaderList			VertexShaderList;
	_TessellationShaderList	TessellationShaderList;
	_GeometryShaderList	 GeometryShaderList;
	_FragmentShaderList		  FragmentShaderList;
	_ComputeShaderList		  ComputeShaderList;

	bgShaderAlgorithm*		getVertexShader(eShaderFactory type);
	bgShaderAlgorithm*		getTessellationShader(eShaderFactory type);
	bgShaderAlgorithm*		getGeometryShader(eShaderFactory type);
	bgShaderAlgorithm*		getFragmentShader(eShaderFactory type);
	bgShaderAlgorithm*		getComputeShader(eShaderFactory type);

	bgShaderAlgorithm*		requestVertexShader(eVertexType type);
	bgShaderAlgorithm*		requestFragmentShader(eVertexType type);



	void InitializeShaderFactory();

protected:



private:
};

