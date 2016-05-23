#pragma once
#include "Renderer.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "bgMovieAlgorithm.h"

#include <map>
using namespace std;

class bgMovie
{
public:
	typedef std::map<int, bgTexture*> _FrameTextureList;
	typedef std::map<int, bgMovieAlgorithm*> _AlgorithmList;

	virtual bool OpenVideo(const std::string& filename);
	virtual bool CloseVideo();
	virtual bool Run(int frame);

	void AddAlgorithm(eMovieEffect movieEffect);
	void DeleteAlgorithm(int algorithmIndex);

	bgTexture*	ConvertTexture(int frame,cv::Mat& targetFrame);
	bgTexture*	GetTexture(int frame);

	bgMovieAlgorithm* GetMovieEffectAlgorithm(int algorithmIndex);
	const bgMovieAlgorithm* GetMovieEffectAlgorithm(int algorithmIndex) const;
	
	bgTexture* GetCurrentTexture() { return currentTexture; }
	cv::Mat*		GetCurrentFrame(){ return &currentFrameMat; }
	int				GetMovieFrame(){ return (int)currentFrame; }
	
	int			GetFPS() { return rate;  }
	double	GetTotalFrame(){ return totalFrame; }

	void		SetNeedModify(bool _modified) { NeedModify = _modified;  }
	void		GetInformation();

	bool		IsOpenVideo() { return IsOpened; }

	bgMovie();
	virtual ~bgMovie();

protected:
	_FrameTextureList	FrameTextureList;
	_AlgorithmList		AlgorithmList;

	cv::VideoCapture* videoCapture;
	cv::Mat	currentFrameMat;
	bgTexture*	currentTexture;

	bool initializeInformation();
	void	clearFrameMemory();
	cv::Mat executeAlgorithm(cv::Mat& FrameMat);

private:
	int rate;
	double totalFrame;
	double currentFrame;
	glm::vec2 FrameSize;
	bool NeedModify;
	bool IsOpened;


};
