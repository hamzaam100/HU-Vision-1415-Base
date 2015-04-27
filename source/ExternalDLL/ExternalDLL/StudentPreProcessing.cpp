#include "StudentPreProcessing.h"

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
	IntensityImageStudent * im = new IntensityImageStudent();

	for (int y = 0; y <= image.getHeight(); y++){
		for (int x = 0; x <= image.getWidth(); x++){
			Intensity tmpIntansity = (image.getPixel(x-1, y) + image.getPixel(x+1, y) + (2 * image.getPixel(x, y)))+127;

			im->setPixel(x, y, tmpIntansity);
		}
	}
	return im;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	return nullptr;
}