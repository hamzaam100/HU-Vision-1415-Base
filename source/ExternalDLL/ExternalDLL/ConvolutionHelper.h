#pragma once

#include "IntensityImageStudent.h"
#include "Kernel.h"

class ConvolutionHelper
{
private:
	static const int sobelKernel[];
	static const int prewittKernel[];
	static const int cannyKernel[];
public:
	ConvolutionHelper();
	~ConvolutionHelper();

	static IntensityImage* ConvolveImage(const IntensityImage& image, const Kernel& kernel);
};