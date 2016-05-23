#pragma once

#include "bgData.h"
#include "GlobalEnum.h"
#include "bgAlgorithm.h"
#include <string>
#include <map>

class bgMapper :  public bgObject
{
public:
	typedef std::map<int, bgAlgorithm*> _AlgorithmChainMap;

	bgMapper();
	virtual ~bgMapper();

	std::string GetName();

	virtual bool updateAlgorithm(float t);
	virtual bool execute();
	virtual void Reset() {}

	void	clearAlgorithm();

	void setVertexType(eVertexType _type);
	
	void	setDrawElementType(eDrawElementType _drawElementType);
	eDrawElementType getDrawElementType(){ return drawElementType; }
	
	bgData* getInput() { return pbgData;  }
	void setInput(bgData* pData) { pbgData = pData; }

	const bgData* getInput() const { return pbgData; }
	
	void	setAlgorithm(int index, bgAlgorithm* pAlgorithm);
	bgAlgorithm*	getAlgorithm(int index); 
	const bgAlgorithm*	getAlgorithm(int index) const;

	int getAlgorithmCount() { return AlgorithmChainMap.size();  }

protected:
	bgData*		pbgData;	
	_AlgorithmChainMap AlgorithmChainMap;

	eVertexType	 vertexType;
	eDrawElementType	drawElementType;

private:

};

////////////////////////////////////////////////////////////////////
///bgSplineMapper
class bgSplineMapper : public bgMapper
{
public:
	bgSplineMapper();
	virtual ~bgSplineMapper();

	virtual bool initialize();		
	virtual void Reset(){}

	void SetControlPoints(std::vector<glm::vec3>& _controlPoints);
	void	SetResolution(int _resolution);
	void TranslateControlPoint(int index, glm::vec2& Differential);
	std::vector<glm::vec3>	GetControlPoints() { return controlPoints;  }

protected:
	int	resolution;
	std::vector<glm::vec3> controlPoints;

private:

};


////////////////////////////////////////////////////////////////////
///bgBezierPatchMapper
class bgBezierPatchMapper : public bgMapper
{
public:
	typedef std::map<ControlPointIndex*, glm::vec3> _ControlPointList;
	bgBezierPatchMapper();
	virtual ~bgBezierPatchMapper();

	virtual bool initialize();
	virtual bool updateAlgorithm(float t);
	virtual void Reset();

	void SetControlPoints(float StepSize);
	void	SetResolution(int _resolution);
	void TranslateControlPoint(int u, int v, glm::vec2& Differential);
	_ControlPointList& GetPatchControlPoints() { return ControlPointList;  }

	ControlPointIndex* FindControlPointIndex(int u, int v);
	glm::vec3	FindControlPointPosition(int u, int v);

	int	GetUSize() { return uSize;  }
	int	GetVSize() { return vSize;   }

protected:
	int uSize;
	int vSize;
	float stepSize;
	_ControlPointList ControlPointList;

	void		MakeControlPointIndex();
	

};








