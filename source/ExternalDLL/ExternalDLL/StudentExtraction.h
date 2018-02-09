/*
* Copyright (c) 2015 DottedEye Designs, Alexander Hustinx, NeoTech Software, Rolf Smit - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*/

#pragma once
#include "Extraction.h"
#include "IntensityImageStudent.h"
#include <functional>
#include "options.h"

class StudentExtraction :
	public Extraction {
public:
	bool stepExtractEyes(const IntensityImage &image, FeatureMap &features) const;
	bool stepExtractNose(const IntensityImage &image, FeatureMap &features) const;
	bool stepExtractMouth(const IntensityImage &image, FeatureMap &features) const;

private:
	void StudentExtraction::determineBeginAndEndOfEye(Histogram & histogram, int & begin, int & end) const;
};

