#include "stdafx.h"
#include "ShaderStrings.h"
#include "bgMaterial.h"
#include "Renderer.h"
#include "bgShaderFactory.h"
#include "bgActor.h"

bgActor::bgActor(bool _pickable)
{
	pMapper = 0;
	pMaterial = 0;
	pShader = 0;
	//opengl buffer object
	VAO = VBO = IBO = -1;

	primitvieCount = 0;	
	indexCount = 0;

	selected = false;
	pickable = _pickable;
}

bgActor::~bgActor()
{

}

bool bgActor::initialize()
{
	ASSERT(pShader != 0);
	if (pMapper)
		name = pMapper->GetName();

	if (VAO != -1)
		glDeleteVertexArrays(1, &VAO);

	if (VBO != -1)
		glDeleteBuffers(1, &VBO);

	if (IBO != -1)
		glDeleteBuffers(1, &IBO);

	pShader->CreateAndLinkProgram();

	pShader->Use();		
		/*pShader->AddUniform(world_matrix);		
		pShader->AddUniform(model_matrix);
		if (pMaterial != 0){
			pShader->AddUniform(texsize);
			pShader->AddUniform(tex0);
		}*/
		pShader->InitializeProperties();
	pShader->UnUse();

	primitvieCount = 0;


	return true;
}

bool bgActor::setInput(bgMapper* _pMapper)
{
	pMapper = _pMapper;
	return true;
}

bool bgActor::setShader(bgShader* _pShader)
{
	ASSERT(_pShader != 0);

	pShader = _pShader;
	return true;
}

bool bgActor::setMaterial(bgMaterial* _pMaterial)
{
	pMaterial = _pMaterial;
	return true;
}

void bgActor::setTranslation(glm::vec3& translate)
{	
	transformMat4 = glm::translate(glm::mat4(), translate);	
}

void bgActor::setScale(glm::vec3& scale)
{
	transformMat4 = glm::scale(transformMat4, scale);
}

void	 bgActor::setPosition(glm::vec3& position)
{
	
}

void		bgActor::intializeTransformed()
{
	transformMat4 = glm::mat4();
}

bool bgActor::update()
{

	return true;
}

bool bgActor::build(cRenderer& Renderer)
{
//	ASSERT(pShader != 0);

	if (pMapper == 0)
		return false;
	
	bgData* pData = pMapper->getInput();
	if (pData == 0)
		return false;

	drawType = pData->GetDrawElementType();
	setPrimitiveCount(pData->NumPosition());
	indexCount = pData->NumIndex();
	Renderer.GenerateGLBuffer(pData, VAO, VBO, IBO);
	
	return true;
}

void bgActor::setPrimitiveCount(int positionCount)
{
	switch (drawType)
	{
	case ELEMENT_TYPE_TRIANGLES:
		primitvieCount = (int)(positionCount / 3); break;
	case ELEMENT_TYPE_QUADS: 
		primitvieCount = (int)(positionCount / 3); break;
	case ELEMENT_TYPE_TRIANGLE_STRIP:
		primitvieCount = (int)(positionCount / 3); break;
	case ELEMENT_TYPE_LINES:
		primitvieCount = (int)(positionCount / 2); break;
	case ELEMENT_TYPE_POINTS:
		primitvieCount = (int)positionCount; break;
	}

}

void bgActor::setSelectionMapper(bgMapper* _pMapper)
{
	if (pMapper == 0 || _pMapper == 0)
		return;

	//bgData* pOriginData = pMapper->getInput();
	//bgData* pSelectedData = _pMapper->getInput();
	//pOriginData->DeepCopy(*pSelectedData);
	//pOriginData->GenerateFromData(SOURCE_2D_LINE_PC, pSelectedData);

	//std::vector<float>& ColorList = pOriginData->GetColorList();
	//for (int index = 0; index < ColorList.size(); index += 3)
	//{
	//	ColorList[index] = 0.f;		//r
	//	ColorList[index +1] = 1.f; //g
	//	ColorList[index + 2] = 0.f;//b
	//}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bgCanvasActor
bgCanvasActor::bgCanvasActor(bool _pickable) : bgActor(_pickable)
{

}

bgCanvasActor::~bgCanvasActor()
{

}

void bgCanvasActor::setMovie(bgMovie* _pMovie)
{
	pMovie = _pMovie;
}

void bgCanvasActor::resize(eCanvasSize size)
{
	bgData* bgData = getInput()->getInput();
	bgData->generate(size);
}

bool bgCanvasActor::initialize()
{	
	canvasSize		= CANVAS_TYPE_FULL;
	canvasEffect	= CANVAS_EFFECT_RESERVED;
	canvasMorph = CANVAS_MORPH_RESERVED;

	return bgActor::initialize();
}

bool bgCanvasActor::build(cRenderer& Renderer)
{
	//bgMapper* pMapper = getInput();
	//if (pMapper->getAlgorithmCount() > 0){
	//	
	//}
	return bgActor::build(Renderer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//bgRayActor
bgRayActor::bgRayActor(bool _pickable) : bgActor(_pickable)
{
	pShader = new bgShader();
	bgData* pData = new bgData();
	pMapper = new bgMapper();
	pMapper->setInput(pData);
}

bgRayActor::~bgRayActor()
{
	if (pMapper) delete pMapper;

}

bool bgRayActor::initialize()
{
	return bgActor::initialize();
}

bool bgRayActor::initializeRay(bgShaderFactory* pShaderFactory, glm::vec3& origin, glm::vec3& direction)
{
	bgData* pData = pMapper->getInput();
	ASSERT(pData != 0);
	pData->generateRay(origin, direction);

	if (pShaderFactory)
	{
		bgShaderAlgorithm* pVertexShaderAlgorithm = pShaderFactory->requestVertexShader(VERTEX_XYZ_COLOR);
		bgShaderAlgorithm* pFragmentShaderAlgorithm = pShaderFactory->requestFragmentShader(VERTEX_XYZ_COLOR);

		pShader->AddShader(pVertexShaderAlgorithm);
		pShader->AddShader(pFragmentShaderAlgorithm);
	}

	
	return initialize();
}

glm::vec3 bgRayActor::GetOrigin()
{

	return glm::vec3();
}

glm::vec3  bgRayActor::GetDirection()
{
	return glm::vec3();
}

void  bgRayActor::SetOrigin(glm::vec3& origin)
{

}

void  bgRayActor::SetDirection(glm::vec3& direction)
{

}








