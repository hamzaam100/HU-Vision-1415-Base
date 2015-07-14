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

bool StudentLocalization::stepFindExactEyes(const IntensityImage &image, FeatureMap &features) const {

	std::cout << std::endl << std::endl;
	std::cout << "=====Student Localization step 5=====" << std::endl;
	std::cout << "Searching for: Eye's" << std::endl;
	std::cout << "================Debug================" << std::endl;

	clock_t functionTime = clock();

	Point2D<double> headLeftPoint{ features.getFeature(Feature::FEATURE_HEAD_LEFT_NOSE_BOTTOM).getPoints()[0] },
					headRightPoint{ features.getFeature(Feature::FEATURE_HEAD_RIGHT_NOSE_BOTTOM).getPoints()[0] },
					headTop{ features.getFeature(Feature::FEATURE_HEAD_TOP).getPoints()[0] },

					noseEndLeft{ features.getFeature(Feature::FEATURE_NOSE_END_LEFT).getPoints()[0] },
					noseEndRight{ features.getFeature(Feature::FEATURE_NOSE_END_RIGHT).getPoints()[0] },
					noseBottom{ features.getFeature(Feature::FEATURE_NOSE_BOTTOM).getPoints()[0] },

					eyeRightTop{ headRightPoint.x, headTop.y },
					eyeRightBottom{ noseEndRight.x, noseBottom.y },

					eyeLeftTop{ headLeftPoint.x, headTop.y },
					eyeLeftBottom{ noseEndLeft.x, noseBottom.y };


	Feature featureLeftEye{ Feature(Feature::FEATURE_EYE_LEFT_RECT) },
			featureRightEye{ Feature(Feature::FEATURE_EYE_RIGHT_RECT) };

	int heightEye = (int)noseBottom.getY() - (int)headTop.getY(),
		heightTop = (int)headTop.getY() + (heightEye / 2),
		heightBottom = (int)noseBottom.getY(),
		width = image.getWidth();

	RGBImage * debugImage = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage);


	while (heightTop < heightBottom){
		bool	breakLeft = false,
				breakRight = false;

		int pixelCount = 0,
			procent = (int)(100 / (eyeLeftBottom.getX() - eyeLeftTop.getX()));
		for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
			int i = x + (heightTop*width);
			if (debugImage->getPixel(i) == RGB{ 0, 0, 0 }){
				pixelCount++;
			}
		}

		if ((procent*pixelCount) >= 40 && (procent*pixelCount) <= 75){
			breakLeft = true;
		}

		std::cout << heightTop << ". T: " << heightTop << " B: " << heightBottom << std::endl;
		std::cout << heightTop << ". L: " << breakLeft << " R: " << breakRight << std::endl;
		std::cout << heightTop << ". PC: " << procent*pixelCount << std::endl;

		pixelCount = 0;
		procent = (int)(100 / (eyeRightTop.getX() - eyeRightBottom.getX()));
		for (int x = eyeRightTop.getX(); x > eyeRightBottom.getX(); x--){
			int i = x + (heightTop*width);

			if (debugImage->getPixel(i) == RGB{ 0, 0, 0 }){
				pixelCount++;
			}
		}

		if ((procent*pixelCount) >= 40 && (procent*pixelCount) <= 75){
			breakRight = true;
		}

		std::cout << heightTop << ". L: " << breakLeft << " R: " << breakRight << std::endl;
		std::cout << heightTop << ". PC: " << procent*pixelCount << std::endl << std::endl;

		if (breakLeft && breakRight){
			heightTop--;
			break;
		}

		heightTop++;
	}

	RGBImage * debugImage2 = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage2);

	//Dilation
	for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
		for (int y = heightTop; y < eyeLeftBottom.getY(); y++){
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
		for (int y = heightTop; y < eyeLeftBottom.getY(); y++){
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
	delete(debugImage2);
		
	//Bottom
	while (heightBottom > heightTop){
		bool	breakLeft = false,
				breakRight = false;

		int pixelCount = 0,
			procent = (int)(100 / (eyeLeftBottom.getX() - eyeLeftTop.getX()));
		for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
			int i = x + (heightBottom * width);

			if (debugImage->getPixel(i) == RGB{ 0, 0, 0 }){
				pixelCount++;
			}
		}

		if ((procent*pixelCount) >= 30 && (procent*pixelCount) <= 80){
			breakLeft = true;
		}

		pixelCount = 0;
		procent = (int)(100 / (eyeRightTop.getX() - eyeRightBottom.getX()));
		for (int x = eyeRightTop.getX(); x > eyeRightBottom.getX(); x--){
			int i = x + (heightBottom * width);

			if (debugImage->getPixel(i) == RGB{ 0, 0, 0 }){
				pixelCount++;
			}
		}


		if ((procent*pixelCount) >= 30 && (procent*pixelCount) <= 80){
			breakRight = true;
		}

		if (breakLeft && breakRight){
			break;
		}

		heightBottom--;
	}

	if (heightTop > heightBottom){
		delete(debugImage);
		return false;
	}

	//TOP
	while (heightTop < heightBottom-maxMargin){
		bool	breakLeft = false,
			breakRight = false;

		int pixelCount = 0,
			procent = (int)(100 / (eyeLeftBottom.getX() - eyeLeftTop.getX()));
		for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
			int i = x + (heightTop*width);
			if (debugImage->getPixel(i) == RGB{ 0, 0, 0 }){
				pixelCount++;
			}
		}

		if ((procent*pixelCount) >= 10 && (procent*pixelCount) <= 60){
			breakLeft = true;
		}

		pixelCount = 0;
		procent = (int)(100 / (eyeRightTop.getX() - eyeRightBottom.getX()));
		for (int x = eyeRightTop.getX(); x > eyeRightBottom.getX(); x--){
			int i = x + (heightTop*width);

			if (debugImage->getPixel(i) == RGB{ 0, 0, 0 }){
				pixelCount++;
			}
		}

		if ((procent*pixelCount) >= 10 && (procent*pixelCount) <= 60){
			breakRight = true;
		}

		if (breakLeft && breakRight){
			break;
		}

		heightTop++;
	}

	int count = 0;
	for (int x = eyeLeftTop.getX(); x < eyeLeftBottom.getX(); x++){
		bool breakLoop = false;
		count++;
		for (int y = heightTop; y < heightBottom; y++){
			int i = x + (y*width);

			if (debugImage->getPixel(i) == RGB{ 0, 0, 0 }){
				breakLoop = true;
				break;
			}
		}

		if (breakLoop){
			break;
		}
	}

	double tempRightTop = eyeRightTop.x;
	eyeRightTop.set(double(eyeRightBottom.x), (double)heightTop);
	eyeRightBottom.set(double(tempRightTop - count), (double)heightBottom);

	eyeLeftTop.set(double(eyeLeftTop.x + count), (double)heightTop);
	eyeLeftBottom.set(double(eyeLeftBottom.x), (double)heightBottom);

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

	std::cout << "Totale Functie tijd: " << functionTime << std::endl;

	//Save debug image
	ImageIO::saveRGBImage(*debugImage, ImageIO::getDebugFileName("Localization-5/debug.png"));
	delete(debugImage);

	return true;
}