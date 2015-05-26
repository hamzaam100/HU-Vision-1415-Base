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
	int max_with = 200;
	int max_height = 200;
	int new_width = 0;
	int new_height = 0;
	int old_width = image.getWidth();
	int old_height = image.getHeight();
	double scaling = 1.0;
	if (old_width >= old_height) {
		new_width = max_with;
		scaling = old_width / (max_with*1.0);
		new_height = old_height / scaling;
	}
	else if (old_width < old_height) {
		new_height = max_height;
		scaling = old_height / (max_height*1.0);
		new_width = old_width / scaling;
	}
	IntensityImage* returnImage = ImageFactory::newIntensityImage(new_width, new_height);
	for (int y = 0; y < returnImage->getHeight(); y++) {
		for (int x = 0; x < returnImage->getWidth(); x++) {
			double old_y = y * scaling;
			double old_x = x * scaling;

			double upperLeftArea = (old_x - ((int)old_x)) * (old_y - ((int)old_y));
			double upperRightArea = (((int)old_x) + 1.0 - old_x) * (old_y - ((int)old_y));
			double lowerLeftArea = (old_x - ((int)old_x)) * (((int)old_y) + 1.0 - old_y);
			double lowerRightArea = (((int)old_x) + 1.0 - old_x) * (((int)old_y) + 1.0 - old_y);
			double sum = upperLeftArea + upperRightArea + lowerLeftArea + lowerRightArea;

			double newPixelValue = 0.0;
			newPixelValue += upperLeftArea * image.getPixel((int)old_x + 1, (int)old_y + 1);
			newPixelValue += upperRightArea * image.getPixel((int)old_x, (int)old_y + 1);
			newPixelValue += lowerLeftArea * image.getPixel((int)old_x + 1, (int)old_y);
			newPixelValue += lowerRightArea * image.getPixel((int)old_x, (int)old_y);
			returnImage->setPixel(x, y, newPixelValue);
		}
	}
	return returnImage;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	int kernelData[] = { 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, -4, -4, -4, 1, 1, 1, 1, 1, 1, -4, -4, -4, 1, 1, 1, 1, 1, 1, -4, -4, -4, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0 };
	Kernel kernel(9, 9, kernelData);
	return ConvolutionHelper::ConvolveImage(image, kernel);
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}