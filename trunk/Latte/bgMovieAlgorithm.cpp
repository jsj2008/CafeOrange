#include "stdafx.h"
#include "bgMovieAlgorithm.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

bgMovieAlgorithm::bgMovieAlgorithm()
{
	BlurSizeX = 11;
	BlurSizeY = 11;
}

bgMovieAlgorithm::~bgMovieAlgorithm()
{


}

bool bgMovieAlgorithm::Initialize()
{
	BlurSizeX = 11;
	BlurSizeY = 11;

	return true;
}

bool bgMovieAlgorithm::Update(float t)
{

	return true;
}


cv::Mat bgMovieAlgorithm::Execute()
{
	cv::Mat ResultMat;
	ResultMat.create(currentFrameMat.size(), currentFrameMat.type());
	cv::GaussianBlur(currentFrameMat, ResultMat, cv::Size(BlurSizeX, BlurSizeY), 0);

	return ResultMat;
}

void bgMovieAlgorithm::setInput(cv::Mat& FrameMat)
{
	if (!currentFrameMat.empty())
		currentFrameMat.release();
	else
		currentFrameMat.create(FrameMat.size(), FrameMat.type());

	currentFrameMat = FrameMat.clone();
}

