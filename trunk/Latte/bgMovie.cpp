#include "stdafx.h"
#include "bgMovie.h"
#include "bgProperty.h"
#include "bgSceneObserver.h"


bgMovie::bgMovie()
{
	totalFrame = currentFrame = 0;
	rate = 0;
	videoCapture = 0; 
	currentTexture = 0;
	NeedModify = true;
	IsOpened = false;
}

bgMovie::~bgMovie()
{
	CloseVideo();
}

bool bgMovie::OpenVideo(const std::string& filename)
{
	if (videoCapture == 0)
		videoCapture = new cv::VideoCapture(filename);
	else
		videoCapture->release();

	videoCapture->open(filename);
	if (!videoCapture->isOpened()) {
		IsOpened = false;
		return false;
	}

	initializeInformation();
	GetInformation();

	IsOpened = true;
	return true;
}

bool bgMovie::CloseVideo()
{
	if (videoCapture)
		videoCapture->release();
	
	clearFrameMemory();
	currentTexture = 0;
	currentFrameMat.release();

	NeedModify = true;
	return true;
}

bool bgMovie::Run(int frame)
{
	if (!IsOpened) return false;

	currentFrame = frame;

	if (!NeedModify)
	{
		if (bgTexture* pSavedTexture = GetTexture(frame))
		{
			currentTexture = pSavedTexture;
			return true;
		}
	}
	
	cv::Mat FrameMat;
	videoCapture->set(CV_CAP_PROP_POS_FRAMES, frame);
	if (!videoCapture->read(FrameMat))
		return false;

	bgTexture* pTexture = 0;
	if (AlgorithmList.size() > 0)
	{
		cv::Mat ConvertedMat;
		ConvertedMat.create(FrameMat.size(), FrameMat.type());
		ConvertedMat = executeAlgorithm(FrameMat);
		pTexture = ConvertTexture(currentFrame, ConvertedMat);

		ConvertedMat.release();
	}
	else
	{
		pTexture = ConvertTexture(currentFrame, FrameMat);
	}

	if (pTexture == 0)
		return false;

	currentTexture = pTexture;
	FrameTextureList[frame] = currentTexture;
	FrameMat.release();

	NeedModify = false;

	return true;
}

bgTexture*	bgMovie::GetTexture(int frame)
{
	if (!IsOpened) return NULL;
	_FrameTextureList::iterator iFrame = FrameTextureList.find(frame);
	if (iFrame != FrameTextureList.end())
		return iFrame->second;

	return NULL;
}

bgTexture*	bgMovie::ConvertTexture(int frame, cv::Mat& FrameMat)
{	
	if (!IsOpened) return NULL;
	bgTexture* Texture = new bgTexture();

	cv::flip(FrameMat, currentFrameMat, 0);	
	Texture->ConvertOpenCVMat(currentFrameMat);

	return Texture;
}

cv::Mat  bgMovie::executeAlgorithm(cv::Mat& FrameMat)
{
	cv::Mat TempMat;
	TempMat.create(FrameMat.size(), FrameMat.type());
	for (int index = 0; index < AlgorithmList.size(); index++)
	{
		bgMovieAlgorithm* pAlgo = AlgorithmList[index];
		if (index == 0)
			pAlgo->setInput(FrameMat);
		else
			pAlgo->setInput(TempMat);

		TempMat = pAlgo->Execute();
	}

	return TempMat;
}

bgMovieAlgorithm* bgMovie::GetMovieEffectAlgorithm(int algorithmIndex)
{
	_AlgorithmList::iterator iAlgo = AlgorithmList.find(algorithmIndex);
	if (iAlgo == AlgorithmList.end())
		return NULL;

	return iAlgo->second;
}

const bgMovieAlgorithm* bgMovie::GetMovieEffectAlgorithm(int algorithmIndex) const
{
	_AlgorithmList::const_iterator iAlgo = AlgorithmList.find(algorithmIndex);
	if (iAlgo == AlgorithmList.end())
		return NULL;

	return iAlgo->second;
}

void bgMovie::AddAlgorithm(eMovieEffect movieEffect)
{
	bgMovieAlgorithm* pAlgo = new bgMovieAlgorithm();
	//AlgorithmList[0] = pAlgo;
	AlgorithmList.insert(std::make_pair(AlgorithmList.size(), pAlgo));
	NeedModify = true;
}

void bgMovie::DeleteAlgorithm(int algorithmIndex)
{
	_AlgorithmList::iterator iAlgo = AlgorithmList.begin();
	if (iAlgo == AlgorithmList.end())
		return;
	AlgorithmList.erase(iAlgo);
	NeedModify = true;	
}

bool bgMovie::initializeInformation()
{
	rate = (int)videoCapture->get(CV_CAP_PROP_FPS);
	FrameSize.x = videoCapture->get(CV_CAP_PROP_FRAME_WIDTH);
	FrameSize.y = videoCapture->get(CV_CAP_PROP_FRAME_HEIGHT);
	totalFrame = videoCapture->get(CV_CAP_PROP_FRAME_COUNT);
	
	clearFrameMemory();
	
	return true;
}

void bgMovie::clearFrameMemory()
{
	_FrameTextureList::iterator iFrame = FrameTextureList.begin();
	for (; iFrame != FrameTextureList.end(); iFrame++)
	{
		bgTexture* pTexture = iFrame->second;
		delete pTexture;
	}

	FrameTextureList.clear();
}

void		bgMovie::GetInformation()
{
	bgProperty property;
	std::string message;
	message = std::to_string(rate);
	property.messageMap.insert(make_pair(std::string("FPS:"), message));
	message = std::to_string(FrameSize.x);
	property.messageMap.insert(make_pair(std::string("Frame Width:"), message));
	message = std::to_string(FrameSize.y);
	property.messageMap.insert(make_pair(std::string("Frame Height:"), message));
	message = std::to_string(totalFrame);
	property.messageMap.insert(make_pair(std::string("Total Frame:"), message));

	bgSceneObserver::GetInstance().sendInformation(INFO_TARGET_OUTWND_OUTPUT, property);
}