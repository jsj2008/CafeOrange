#include "stdafx.h"
#include "bgShaderAlgorithm.h"
#include "bgShaderFactory.h"


bgShaderFactory::bgShaderFactory()
{

}


bgShaderFactory::~bgShaderFactory()
{

}

void bgShaderFactory::InitializeShaderFactory()
{
	//SHADER_VERTEX
	bgShaderAlgorithm* pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_VERTEX, SHADER_VERTEX_SIMPLE_PC);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_VERTEX, "../Media/Shader/simplePC_vert.glsl");
	VertexShaderList.insert(std::make_pair((int)SHADER_VERTEX_SIMPLE_PC, pShaderAlgo));

	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_VERTEX, SHADER_VERTEX_SIMPLE_PTC);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_VERTEX, "../Media/Shader/simplePTC_vert.glsl");
	VertexShaderList.insert(std::make_pair((int)SHADER_VERTEX_SIMPLE_PTC, pShaderAlgo));

	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_VERTEX, SHADER_VERTEX_SIMPLE_PNC);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_VERTEX, "../Media/Shader/simplePNC_vert.glsl");
	VertexShaderList.insert(std::make_pair((int)SHADER_VERTEX_SIMPLE_PNC, pShaderAlgo));

	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_VERTEX, SHADER_VERTEX_SELECTED);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_VERTEX, "../Media/Shader/selected_vert.glsl");
	VertexShaderList.insert(std::make_pair((int)SHADER_VERTEX_SELECTED, pShaderAlgo));


	//SHADER_TESSELLATION
	//SHADER_GEOMETRY

	//SHADER_FRAGMENT
	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_SIMPLE_PC);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/simplePC_frag.glsl");
	FragmentShaderList.insert(std::make_pair((int)SHADER_FRAGMENT_SIMPLE_PC, pShaderAlgo));

	pShaderAlgo = new bgShaderFragmentAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_SIMPLE_PTC);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/simplePTC_frag.glsl");
	FragmentShaderList.insert(std::make_pair((int)SHADER_FRAGMENT_SIMPLE_PTC, pShaderAlgo));

	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_SIMPLE_PNC);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/simplePNC_frag.glsl");
	FragmentShaderList.insert(std::make_pair((int)SHADER_FRAGMENT_SIMPLE_PNC, pShaderAlgo));

	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_SELECTED);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/selected_frag.glsl");
	FragmentShaderList.insert(std::make_pair((int)SHADER_FRAGMENT_SELECTED, pShaderAlgo));

	//////////////////////////////////////////////////////////////////////////////////////
	////For Post Process
	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_PP_ALPHA);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/alphaPTC_frag.glsl");
	FragmentShaderList.insert(std::make_pair((int)SHADER_FRAGMENT_PP_ALPHA, pShaderAlgo));

	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_PP_CONTRAST_BRIGHT);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/brightcontrastPTC_frag.glsl");
	FragmentShaderList.insert(std::make_pair((int)SHADER_FRAGMENT_PP_CONTRAST_BRIGHT, pShaderAlgo));

	pShaderAlgo = new bgShaderAlgorithm(SHADER_TYPE_FRAGMENT, SHADER_FRAGMENT_PP_GRAY);
	pShaderAlgo->LoadFromFile(SHADER_TYPE_FRAGMENT, "../Media/Shader/grayPTC_frag.glsl");
	FragmentShaderList.insert(std::make_pair((int)SHADER_FRAGMENT_PP_GRAY, pShaderAlgo));
	//////////////////////////////////////////////////////////////////////////////////////

	//SHADER_COMPUTE

}

bgShaderAlgorithm*	 bgShaderFactory::requestVertexShader(eVertexType type)
{
	_VertexShaderList::iterator iVertex = VertexShaderList.end();
	switch (type)
	{
	case VERTEX_XYZ_COLOR:
		iVertex = VertexShaderList.find((int)SHADER_VERTEX_SIMPLE_PC);
		break;
	case VERTEX_XYZ_NORMAL_DIFFUSE:
		iVertex = VertexShaderList.find((int)SHADER_VERTEX_SIMPLE_PNC);
		break;
	case VERTEX_XYZ_TEX0:
		break;
	case VERTEX_XYZ_COLOR_TEX0:
		iVertex = VertexShaderList.find((int)SHADER_VERTEX_SIMPLE_PTC);
		break;
	case VERTEX_XYZ_NORMAL_TEX0:
		break;
	case VERTEX_SELECTED:
		iVertex = VertexShaderList.find((int)SHADER_VERTEX_SELECTED);
		break;
	}

	if (iVertex != VertexShaderList.end())
		return iVertex->second;

	return 0;
}

bgShaderAlgorithm*		bgShaderFactory::requestFragmentShader(eVertexType type)
{
	_FragmentShaderList::iterator iFragment = FragmentShaderList.end();
	switch (type)
	{
	case VERTEX_XYZ_COLOR:
		iFragment = FragmentShaderList.find((int)SHADER_FRAGMENT_SIMPLE_PC);
		break;
	case VERTEX_XYZ_NORMAL_DIFFUSE:
		iFragment = FragmentShaderList.find((int)SHADER_FRAGMENT_SIMPLE_PNC);
		break;
	case VERTEX_XYZ_TEX0:
		break;
	case VERTEX_XYZ_COLOR_TEX0:
		iFragment = FragmentShaderList.find((int)SHADER_FRAGMENT_SIMPLE_PTC);
		break;
	case VERTEX_XYZ_NORMAL_TEX0:
		break;
	case VERTEX_SELECTED:
		iFragment = FragmentShaderList.find((int)SHADER_FRAGMENT_SELECTED);
		break;	
	}

	if (iFragment != FragmentShaderList.end())
		return iFragment->second;

	return 0;
}


bgShaderAlgorithm*	bgShaderFactory::getVertexShader(eShaderFactory type)
{
	_VertexShaderList::iterator iVertex = VertexShaderList.find((int)type);
	if (iVertex != VertexShaderList.end())
		return iVertex->second;

	return 0;
}

bgShaderAlgorithm*	bgShaderFactory::getTessellationShader(eShaderFactory type)
{
	return 0;
}

bgShaderAlgorithm*	bgShaderFactory::getGeometryShader(eShaderFactory type)
{
	return 0;
}

bgShaderAlgorithm*	bgShaderFactory::getFragmentShader(eShaderFactory type)
{
	_FragmentShaderList::iterator iFragment = FragmentShaderList.find((int)type);
	if (iFragment != FragmentShaderList.end())
		return iFragment->second;

	return 0;
}

bgShaderAlgorithm*	bgShaderFactory::getComputeShader(eShaderFactory type)
{
	return 0;
}
