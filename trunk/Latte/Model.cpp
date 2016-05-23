#include "StdAfx.h"
#include "Model.h"
#include "Renderer.h"
#include "ShaderAlgorithm.h"


cModel::cModel()
{
	pShaderEffect = 0;
	DrawMode = GL_TRIANGLES;
}


cModel::~cModel()
{

}

void cModel::Render(cRenderer& Renderer)
{
	
	glPushMatrix();

	/*Material.Set(Renderer);

	if(pShaderEffect)
		pShaderEffect->Render(Renderer, *this);

	glBegin(DrawMode); 
		
	int index =0;
	_VertexList::iterator iVertex = VertexList.begin();
	for(; iVertex != VertexList.end() ; iVertex++)
	{
		cVertex& Vertex = *iVertex;
		glTexCoord2f(Vertex.Texel.x, Vertex.Texel.y);
		glNormal3f(Vertex.Normal.x, Vertex.Normal.y, Vertex.Normal.z);
		glVertex3f(Vertex.Position.x, Vertex.Position.y, Vertex.Position.z);
		index++;
	}

	glEnd();

	Material.Reset(Renderer);

	if(pShaderEffect)
		pShaderEffect->SetFixedPipeline();*/

	glPopMatrix();
}

void cModel::GetVertexMinMax(glm::vec3& Min,glm::vec3& Max)
{
	_VertexList::iterator iVertex = VertexList.begin();
	for(; iVertex != VertexList.end() ; iVertex++)
	{
		cVertex& Vertex = *iVertex;
		glm::vec3 Position = Vertex.Position;
		if(Position.x < Min.x)
			Min.x = Position.x;
		if(Position.y < Min.y)
			Min.y = Position.y;
		if(Position.z < Min.z)
			Min.z = Position.z;

		if(Position.x > Max.x)
			Max.x = Position.x;
		if(Position.y > Max.y)
			Max.y = Position.y;
		if(Position.z > Max.z)
			Max.z = Position.z;
	}
}