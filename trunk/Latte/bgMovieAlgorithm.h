#pragma once 

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"

class bgMovieAlgorithm 
{
public:
	int BlurSizeX;
	int BlurSizeY;

	bool Initialize();
	bool Update(float t);
	cv::Mat Execute();
	
	void setInput(cv::Mat& FrameMat);
	cv::Mat& getOutput() { return currentFrameMat;  }

	void SetBeforeAlgorithm(bgMovieAlgorithm* _pAlgo){ pBeforeAlgorithm = _pAlgo; }
	bgMovieAlgorithm* GetBeforeAlgorithm() { return pBeforeAlgorithm; }

	bgMovieAlgorithm();
	virtual ~bgMovieAlgorithm();

protected:
	cv::Mat	currentFrameMat;
	bgMovieAlgorithm* pBeforeAlgorithm;


};

