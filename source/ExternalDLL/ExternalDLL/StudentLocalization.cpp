#include "StudentLocalization.h"
#include "DefaultLocalization.h"
#include <vector>
#include "RGBImage.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "ImageIO.h"
#include <direct.h>
#include <math.h>
#include <cstdio>
#include <sstream>
#include "SonnetXVIII.h"
#include "HereBeDragons.h"
#include "RGBImageStudent.h"
#include "ImageFactory.h"

bool StudentLocalization::stepFindHead(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseMouthAndChin(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindChinContours(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentLocalization::stepFindNoseEndsAndEyes(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

/*
Functie bool stepExtractEyes(const IntensityImage &image, FeatureMap &features);
Input Binair edge-detected image
		FEATURE_HEAD_TOP
		FEATURE_HEAD_LEFT_SIDE
		FEATURE_HEAD_RIGHT_SIDE
		FEATURE_EYE_LEFT_RECT
		FEATURE_EYE_RIGHT_RECT
		FEATURE_NOSE_END_LEFT
		FEATURE_NOSE_END_RIGHT
		FEATURE_NOSE_BOTTOM
		FEATURE_HEAD_LEFT_NOSE_BOTTOM
		FEATURE_HEAD_RIGHT_NOSE_BOTTOM
Output
		FEATURE_EYE_LEFT_RECT
		FEATURE_EYE_RIGHT_RECT

*/

bool StudentLocalization::stepFindExactEyes(const IntensityImage &image, FeatureMap &features) const {

	std::cout << std::endl << std::endl;
	std::cout << "=========Localization step 5=========" << std::endl;
	std::cout << "Searching for: Eye's --<" << std::endl;
	std::cout << "================Debug================" << std::endl;


	Point2D<double> headLeftPoint = features.getFeature(Feature::FEATURE_HEAD_LEFT_NOSE_BOTTOM).getPoints()[0];
	Point2D<double> headRightPoint = features.getFeature(Feature::FEATURE_HEAD_RIGHT_NOSE_BOTTOM).getPoints()[0];
	Point2D<double> headTop = features.getFeature(Feature::FEATURE_HEAD_TOP).getPoints()[0];

	Point2D<double> noseEndLeft = features.getFeature(Feature::FEATURE_NOSE_END_LEFT).getPoints()[0];
	Point2D<double> noseEndRight = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT).getPoints()[0];

	Point2D<double> noseBottom = features.getFeature(Feature::FEATURE_NOSE_BOTTOM).getPoints()[0];

	Point2D<double> eyeRightTop = Point2D < double > {headRightPoint};
	Point2D<double> eyeLeftTop = Point2D < double > {headLeftPoint};

	Point2D<double> eyeLeftBottom = Point2D < double > {noseEndLeft};
	Point2D<double> eyeRightBottom = Point2D < double > {noseEndRight};

	eyeRightTop.setY(headTop.getY());
	eyeLeftTop.setY(headTop.getY());

	RGBImage * debugImage = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage);

	Feature featureLeftEye = Feature(Feature::FEATURE_EYE_LEFT_RECT);
	Feature featureRightEye = Feature(Feature::FEATURE_EYE_RIGHT_RECT);

	int heightEye = (int)noseBottom.getY() - (int)headTop.getY();
	int heightTop = (int)headTop.getY() + (heightEye / 2);
	int heightBottom = (int)noseBottom.getY();
	int width = debugImage->getWidth();

	RGB pixelLeft, pixelRight;
	//TOP
	while (true){
		pixelLeft = debugImage->getPixel(eyeLeftBottom.getX() + (debugImage->getWidth() * heightTop));
		pixelRight = debugImage->getPixel(eyeRightBottom.getX() + (debugImage->getWidth() * heightTop));

		if (pixelLeft == RGB{ 0, 0, 0 } && pixelRight == RGB{ 0, 0, 0 }){
			break;
		}

		//Other side of the line
		pixelLeft = debugImage->getPixel(eyeLeftTop.getX() + (debugImage->getWidth() * heightTop));
		pixelRight = debugImage->getPixel(eyeRightTop.getX() + (debugImage->getWidth() * heightTop));

		if (pixelLeft == RGB{ 0, 0, 0 } && pixelRight == RGB{ 0, 0, 0 }){
			break;
		}

		heightTop++;
	}

	RGBImage * debugImage2 = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage2);

	//Bottom
	while (true){
		double count = 0.0;
		double procent = (100 / (eyeLeftBottom.getX() - eyeLeftTop.getX()));
		for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
			int i = x + (heightBottom * width);

			if (debugImage2->getPixel(i) == RGB{ 0, 0, 0 }){
				count++;
			}
			if ((procent*count) >(procent * 7.5)){
				break;
			}
		}

		if ((procent*count) > (procent * 7.5)){
			break;
		}

		count = 0.0;
		procent = (100 / (eyeRightTop.getX() - eyeRightBottom.getX()));
		for (int x = eyeRightTop.getX(); x > eyeRightBottom.getX(); x--){
			int i = x + (heightBottom * width);

			if (debugImage2->getPixel(i) == RGB{ 0, 0, 0 }){
				count++;
			}
			if ((procent*count) >(procent * 7.5)){
				break;
			}
		}

		if ((procent*count) > (procent * 7.5)){
			break;
		}

		heightBottom--;
	}


	for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
		for (int y = eyeLeftTop.getY(); y < eyeLeftBottom.getY(); y++){
			int i = x + (y*width);

			if (debugImage2->getPixel(i) == RGB{ 0, 0, 0 }){
				debugImage->setPixel((i - 1), RGB{ 0, 0, 0 });
				debugImage->setPixel(i, RGB{ 0, 0, 0 });
				debugImage->setPixel((i + 1), RGB{ 0, 0, 0 });

				debugImage->setPixel((i + width), RGB{ 0, 0, 0 });
				debugImage->setPixel((i - width), RGB{ 0, 0, 0 });
			}
		}
	}

	for (int x = eyeRightTop.getX(); x > eyeRightBottom.getX(); x--){
		for (int y = eyeLeftTop.getY(); y < eyeLeftBottom.getY(); y++){
			int i = x + (y*width);

			if (debugImage2->getPixel(i) == RGB{ 0, 0, 0 }){
				debugImage->setPixel((i - 1), RGB{ 0, 0, 0 });
				debugImage->setPixel(i, RGB{ 0, 0, 0 });
				debugImage->setPixel((i + 1), RGB{ 0, 0, 0 });

				debugImage->setPixel((i + width), RGB{ 0, 0, 0 });
				debugImage->setPixel((i - width), RGB{ 0, 0, 0 });
			}
		}
	}

	bool passLeft = true;
	bool passRight = true;
	while (true){
		heightTop++;

		if (passLeft){
			pixelLeft = debugImage->getPixel(eyeLeftTop.getX() + (debugImage->getWidth() * heightTop));
			pixelRight = debugImage->getPixel(eyeRightTop.getX() + (debugImage->getWidth() * heightTop));

			if (pixelLeft == RGB{ 255, 255, 255 } && pixelRight == RGB{ 255, 255, 255 }){
				passLeft = false;
			}
		}

		if (passRight){
			pixelLeft = debugImage->getPixel(eyeLeftBottom.getX() + (debugImage->getWidth() * heightTop));
			pixelRight = debugImage->getPixel(eyeRightBottom.getX() + (debugImage->getWidth() * heightTop));

			if (pixelLeft == RGB{ 255, 255, 255 } && pixelRight == RGB{ 255, 255, 255 }){
				passRight = false;
			}
		}

		if (!passLeft && !passRight){
			double count = 0.0;
			double procent = (100 / (eyeLeftBottom.getX() - eyeLeftTop.getX()));

			for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
				int i = x + (heightTop*width);

				if (debugImage2->getPixel(i) == RGB{ 0, 0, 0 }){
					count++;
				}
				if ((procent*count) >(procent * 5.0)){
					break;
				}

			}
			//std::cout << "count: " << (procent*count) << " procent: " << (procent * 5.0) << std::endl;

			if ((procent*count) > (procent * 5.0)){
				//std::cout << "count: " << count << std::endl;
				break;
			}

			count = 0.0;
			procent = (100 / (eyeRightTop.getX() - eyeRightBottom.getX()));
			for (int x = eyeRightTop.getX(); x > eyeRightBottom.getX(); x--){
				int i = x + (heightTop*width);

				if (debugImage2->getPixel(i) == RGB{ 0, 0, 0 }){
					count++;
				}
				if ((procent*count) > (procent * 5.0)){
					break;
				}

			}

			//std::cout << "count: " << (procent*count) << " procent: " << (procent * 5.0) << std::endl;

			if ((procent*count) > (procent * 5.0)){
				//std::cout << "count: " << count << std::endl;
				break;
			}
		}
	}

	eyeRightBottom.setY(heightBottom);
	eyeLeftBottom.setY(heightBottom);

	eyeRightTop.setY(heightTop);
	eyeLeftTop.setY(heightTop);

	HereBeDragons::AsHisTriumphantPrizeProudOfThisPride(*debugImage, eyeRightTop, eyeRightBottom, RGB(255, 0, 0));
	HereBeDragons::AsHisTriumphantPrizeProudOfThisPride(*debugImage, eyeLeftTop, eyeLeftBottom, RGB(255, 0, 0));

	//Add the left eye rect
	featureLeftEye.addPoint(eyeLeftTop);
	featureLeftEye.addPoint(eyeLeftBottom);

	//Add the right eye rect
	featureRightEye.addPoint(eyeRightTop);
	featureRightEye.addPoint(eyeRightBottom);

	//Put the eye features
	features.putFeature(featureRightEye);
	features.putFeature(featureLeftEye);

	//Save debug image
	ImageIO::saveRGBImage(*debugImage, ImageIO::getDebugFileName("Localization-10/debug2.png"));
	delete debugImage;

	return true;
}