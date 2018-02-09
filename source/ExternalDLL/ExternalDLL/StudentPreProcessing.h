/*
* Copyright (c) 2015 DottedEye Designs, Alexander Hustinx, NeoTech Software, Rolf Smit - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*/

#pragma once
#include "PreProcessing.h"
#include "IntensityImageStudent.h"
#include "ImageIO.h"
#include <iostream>
#include <functional>
#include <array>
#include "options.h"

#define LOG_KERNEL_SIZE 9
#define LOG_KERNEL_HALF_SIZE 4

#define GAUSSIAN_KERNEL_SUM 273
#define GAUSSIAN_KERNEL_SIZE 5
#define GAUSSIAN_KERNEL_HALF_SIZE 2
#define TRESHOLD 220
#define MIN_BLACK_NEIGHBOUR_COUNT 5

class StudentPreProcessing : public PreProcessing {
public:
	typedef std::array<int, LOG_KERNEL_SIZE * LOG_KERNEL_SIZE> LOGKernel;
	typedef std::array<double, GAUSSIAN_KERNEL_SIZE * GAUSSIAN_KERNEL_SIZE> GaussianKernel;

	IntensityImage * stepToIntensityImage(const RGBImage &image) const;
	IntensityImage * stepScaleImage(const IntensityImage &image) const;
	IntensityImage * stepEdgeDetection(const IntensityImage &image) const;
	IntensityImage * stepThresholding(const IntensityImage &image) const;
};