#pragma once

#include "GlobalEnum.h"
#include <vector>
#include <map>
using namespace std;

class bgData;
class bgTexture;
class bgAlgorithm
{
public:	
	eAlgorithmType Type;

	bgAlgorithm();
	virtual ~bgAlgorithm();

	virtual bool Initialize() = 0;
	virtual bool Update(float t) = 0;
	virtual bool Execute() = 0;

	void SetModify(bool _modified){ modified = _modified;  }
	
	void	SetBgData(bgData* _pbgData, bool bDeepCopy = false);
	bgData* GetBgData();


protected:
	bool		modified;
	bgData* pbgData;
	bgTexture*	pbgTexture;		 
};


////////////////////////////////////////////////////////////////////
///bgCurveAlgorithm
class bgCurveAlgorithm : public bgAlgorithm
{
public:
	typedef std::vector<glm::vec3>_ControlPoints;

	bgCurveAlgorithm();
	virtual ~bgCurveAlgorithm();

	void	 SetControlPoints(_ControlPoints& points,int resolution);
	void	 SetResolution(int resolution);

	virtual bool Initialize();
	virtual bool Update(float t);
	virtual bool Execute();

	virtual glm::vec3 GetPosition(float t);


protected:
	_ControlPoints ControlPoints;

	int	NumControlPoints;
	int	Resolution;

};

////////////////////////////////////////////////////////////////////
///bgBezierCurveAlgorithm
class bgBezierCurveAlgorithm : public bgCurveAlgorithm
{
public:
	bgBezierCurveAlgorithm();
	virtual ~bgBezierCurveAlgorithm();

	virtual bool Initialize();
	virtual bool Update(float t);
	virtual bool Execute();

	virtual glm::vec3 GetPosition(float t);

protected:
	static float B0(float t);
	static float B1(float t);
	static float B2(float t);
	static float B3(float t);

	static float D0(float t);
	static float D1(float t);
	static float D2(float t);
	static float D3(float t);
};

////////////////////////////////////////////////////////////////////
///bgSplineCurveAlgorithm
class bgSplineCurveAlgorithm : public bgBezierCurveAlgorithm
{
public:
	typedef std::vector<int> _KnotList;

	bgSplineCurveAlgorithm();
	virtual ~bgSplineCurveAlgorithm();

	virtual bool Initialize();
	virtual bool Update(float t);
	virtual bool Execute();
	virtual glm::vec3 GetPosition(float t);

	int GetOrder(){ return Order;  }

protected:
	_KnotList KnotList;
	int Order;

private:
	int Span;

};




////////////////////////////////////////////////////////////////////
///NeighborControlPoints
class ControlPointIndex;
class NeighborControlPoints
{
public:
	ControlPointIndex* pUp;
	ControlPointIndex* pDown;
	ControlPointIndex* pLeft;
	ControlPointIndex* pRight;

	NeighborControlPoints(){
		pUp = pDown = pLeft = pRight = 0;
	}

};

////////////////////////////////////////////////////////////////////
///ControlPointIndex
class ControlPointIndex
{
public:
	int row;
	int column;
	bool bEdgePoint;
	NeighborControlPoints* pNeighbor;

	ControlPointIndex(){
		pNeighbor = 0;
		bEdgePoint = false;
		Initialize();
	}

	void Initialize(){
		row = column = -1;
	}
};

////////////////////////////////////////////////////////////////////
///bgBezierPatchAlgorithm
class bgBezierPatchAlgorithm : public bgAlgorithm
{
public:
	typedef std::map<ControlPointIndex*, glm::vec3>_ControlPointList;
	typedef std::map<int, bgBezierCurveAlgorithm*>  _RowBezierCurveList;
	typedef std::map<int, bgBezierCurveAlgorithm*>  _ColumnBezierCurveList;


	bgBezierPatchAlgorithm();
	virtual ~bgBezierPatchAlgorithm();

	virtual bool Initialize();
	virtual bool Update(float t);
	virtual bool Execute();

	void SetControlNet(_ControlPointList& controlList, int usize, int vsize);


protected:
	_ControlPointList ControlPointList;
	_RowBezierCurveList		  RowBezierCurveList;
	_ColumnBezierCurveList ColumnBezierCurveList;

	void		MakeTriagleList();
	ControlPointIndex* FindControlPointIndex(int u, int v);
	glm::vec3	FindControlPointPosition(int u, int v);

private:
	int uSize;
	int vSize;
};








