#include "StudentPreProcessing.h"
#include "ConvolutionHelper.h"
#include "Kernel.h"
#include "ImageFactory.h"

#include "OpenCV\cv.h"

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	IntensityImageStudent * im = new IntensityImageStudent(image.getWidth(), image.getHeight());

	for (int i = 0; i <= (image.getHeight()*image.getWidth()); i++){
		Intensity tmp = (image.getPixel(i).r + image.getPixel(i).g + image.getPixel(i).b) / 3;
		im->setPixel(i, tmp);
	}

	return im;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	return nullptr;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	int kernelData[] = { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, -4, -4, -4, 1, 1, 1, 1, 1, 1, -4, -4, -4, 1, 1, 1, 1, 1, 1, -4, -4, -4, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0 };
	Kernel kernel(9, 9, kernelData);
	return ConvolutionHelper::ConvolveImage(image, kernel);
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}