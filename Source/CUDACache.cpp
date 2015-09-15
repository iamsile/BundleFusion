#include "stdafx.h"
#include "CUDACache.h"

void CUDACache::storeFrame(const float* d_depth, const uchar4* d_color, unsigned int inputWidth, unsigned int inputHeight)
{
	CUDACachedFrame& frame = m_cache[m_currentFrame];
	//CUDAImageUtil::resample<float>(frame.d_depthDownsampled, m_width, m_height, d_depth, inputWidth, inputHeight);
	//CUDAImageUtil::resample<uchar4>(frame.d_colorDownsampled, m_width, m_height, d_color, inputWidth, inputHeight);
	CUDAImageUtil::resampleFloat(frame.d_depthDownsampled, m_width, m_height, d_depth, inputWidth, inputHeight);
	CUDAImageUtil::resampleUCHAR4(frame.d_colorDownsampled, m_width, m_height, d_color, inputWidth, inputHeight);

	m_currentFrame++;
}