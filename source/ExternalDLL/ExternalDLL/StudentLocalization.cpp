#include <math.h>
#include "StudentLocalization.h"
#include "IntensityImageStudent.h"
#include "RGBImage.h"
#include "ImageIO.h"
#include "ImageFactory.h"
#include "HereBeDragons.h"
#include <vector>
#include <array>

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
	std::cout << "=========Localization step 5=========" << std::endl;
	std::cout << "Searching for: Eye's" << std::endl;
	std::cout << "=====================================" << std::endl;

	RGBImage * debugImage = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage);

	Point2D<double> headMostLeftPoint = features.getFeature(Feature::FEATURE_HEAD_LEFT_NOSE_BOTTOM).getPoints()[0],
					headMostRightPoint = features.getFeature(Feature::FEATURE_HEAD_RIGHT_NOSE_BOTTOM).getPoints()[0],
					headTopPoint = features.getFeature(Feature::FEATURE_HEAD_TOP).getPoints()[0],
					headBottomPoint = features.getFeature(Feature::FEATURE_CHIN).getPoints()[0],
					
					HeadCenter((headMostLeftPoint.x + headMostRightPoint.x) / 2, (headTopPoint.y + headBottomPoint.y) / 2),
					
					noseEndLeft = features.getFeature(Feature::FEATURE_NOSE_END_LEFT).getPoints()[0],
					noseEndRight = features.getFeature(Feature::FEATURE_NOSE_END_RIGHT).getPoints()[0];

	const int	PADDING_BEGIN_EYE_Y = 10,
				MIXIMUM_SEARCH_EREA = 40,
				NOSE_END = static_cast<int>(std::fmin(noseEndLeft.y, noseEndRight.y)),
				BEGIN_EYE_SCAN_Y = static_cast<int>(HeadCenter.y) - PADDING_BEGIN_EYE_Y,
				END_EYE_SCAN_Y = NOSE_END - BEGIN_EYE_SCAN_Y < MIXIMUM_SEARCH_EREA ? NOSE_END : BEGIN_EYE_SCAN_Y + MIXIMUM_SEARCH_EREA,
				HEAD_CENTER_X = static_cast<int>(HeadCenter.x),
				HEAD_MOST_RIGHT_POINT_X = static_cast<int>(headMostRightPoint.x),
				HEAD_MOST_LEFT_POINT_X = static_cast<int>(headMostLeftPoint.x);

	int eyeHeightBegin = BEGIN_EYE_SCAN_Y,
		eyeHeightEnd = END_EYE_SCAN_Y;

	HereBeDragons::TriumphInLoveFleshStaysNoFatherReason(*debugImage, Point2D<double>(HEAD_CENTER_X, BEGIN_EYE_SCAN_Y), RGB(255, 0, 0));
	HereBeDragons::TriumphInLoveFleshStaysNoFatherReason(*debugImage, Point2D<double>(HEAD_CENTER_X, END_EYE_SCAN_Y), RGB(255, 0, 0));

	IntensityImageStudent slimmedEgedesImage{ image };

	for (int x = HEAD_MOST_LEFT_POINT_X; x < HEAD_MOST_RIGHT_POINT_X; x++) {
		for (int y = BEGIN_EYE_SCAN_Y; y < END_EYE_SCAN_Y; y++) {
			if (image.getPixel(x, y) == Color::WHITE) {
				if (x > 0 && y > 0 && x < HEAD_MOST_RIGHT_POINT_X - 1 && y < END_EYE_SCAN_Y - 1) {
					slimmedEgedesImage.setPixel(x - 1, y, Color::WHITE);
					slimmedEgedesImage.setPixel(x, y - 1, Color::WHITE);
					slimmedEgedesImage.setPixel(x + 1, y, Color::WHITE);
					slimmedEgedesImage.setPixel(x, y + 1, Color::WHITE);
				}
				else {
					if (x > 0) {
						slimmedEgedesImage.setPixel(x - 1, y, Color::WHITE);
					}
					if (y > 0) {
						slimmedEgedesImage.setPixel(x, y - 1, Color::WHITE);
					}
					if (x < HEAD_MOST_RIGHT_POINT_X - 1) {
						slimmedEgedesImage.setPixel(x + 1, y, Color::WHITE);
					}
					if (y < END_EYE_SCAN_Y - 1) {
						slimmedEgedesImage.setPixel(x, y + 1, Color::WHITE);
					}
				}
			}
		}
	}
#ifdef _DEBUG
	std::cout << std::endl << "HISTOGRAM VALUES:" << std::endl;
#endif
	Histogram histogram = createHistogram(HEAD_MOST_LEFT_POINT_X, HEAD_MOST_RIGHT_POINT_X, BEGIN_EYE_SCAN_Y, END_EYE_SCAN_Y, HISTOGRAM_TYPE::vertically, slimmedEgedesImage);

#ifdef _DEBUG
	std::cout << std::endl << "HISTOGRAM SINUS POSITIONS:" << std::endl;
#endif
	Histogram sinuses = getSinussesFromHistogram(histogram);

	if (!sinuses.empty()){
		const int	MINIMUM_SUNUS_LENGTH = 2,
					MAXIMUM_SUNUS_LENGTH = 20,
					PADDING_EYE = 1;
		
		//remove first element if direction up is not defined
		if (sinuses[0].first == DIRECTION::none){
			sinuses.erase(sinuses.begin());
		}

		//remove last element if direction down is not defined
		if (sinuses.back().second == DIRECTION::none){
			sinuses.pop_back();
		}

		//remove sinusses larger then MAXIMUM_SUNUS_LENGTH or lower then MINIMUM_SUNUS_LENGTH
		for (int i = sinuses.size() - 1; i >= 0; i--){
			int sinusLength = sinuses[i].second - sinuses[i].first;
			if (sinusLength <= MINIMUM_SUNUS_LENGTH || sinusLength >= MAXIMUM_SUNUS_LENGTH){
				sinuses.erase(sinuses.begin() + i);
			}
		}

		//if 2 or more sinuses found
		if (sinuses.size() == 1){
			eyeHeightBegin = sinuses[0].first;
			eyeHeightEnd = sinuses[0].second;
		}
		else if (sinuses.size() > 1){
			std::pair<int, int> & secondSinus = sinuses[1];
			eyeHeightBegin = secondSinus.first;
			eyeHeightEnd = secondSinus.second;
		}

		//add some padding to the bottom
		eyeHeightBegin -= PADDING_EYE;
		eyeHeightEnd += PADDING_EYE;
	}

	int beginLeftEyeWidthPosition = HEAD_MOST_LEFT_POINT_X,
		endLeftEyePosition = static_cast<int>(noseEndLeft.x),
		beginRightEyePosition = static_cast<int>(noseEndRight.x),
		endRightEyePosition = HEAD_MOST_RIGHT_POINT_X;


	bool breakLoop = false;
	for (int x = HEAD_CENTER_X; x > HEAD_MOST_LEFT_POINT_X; x--){
		for (int y = eyeHeightBegin; y < eyeHeightEnd; y++){
			if (slimmedEgedesImage.getPixel(x, y) == Color::BLACK){
				endLeftEyePosition = x;
				breakLoop = true;
				break;
			}
		}

		if (breakLoop){
			break;
		}
	}

	breakLoop = false;
	for (int x = HEAD_MOST_LEFT_POINT_X; x < HEAD_CENTER_X; x++){
		for (int y = eyeHeightBegin; y < eyeHeightEnd; y++){
			if (slimmedEgedesImage.getPixel(x, y) == Color::BLACK){
				beginLeftEyeWidthPosition = x -2;
				breakLoop = true;
				break;
			}
		}

		if (breakLoop){
			break;
		}
	}

	breakLoop = false;
	for (int x = HEAD_CENTER_X; x < HEAD_MOST_RIGHT_POINT_X; x++){
		for (int y = eyeHeightBegin; y < eyeHeightEnd; y++){
			if (slimmedEgedesImage.getPixel(x, y) == Color::BLACK){
				beginRightEyePosition = x;
				breakLoop = true;
				break;
			}
		}

		if (breakLoop){
			break;
		}
	}

	breakLoop = false;
	for (int x = HEAD_MOST_RIGHT_POINT_X; x > HEAD_CENTER_X; x--){
		for (int y = eyeHeightBegin; y < eyeHeightEnd; y++){
			if (slimmedEgedesImage.getPixel(x, y) == Color::BLACK){
				endRightEyePosition = x + 2;
				breakLoop = true;
				break;
			}
		}

		if (breakLoop){
			break;
		}
	}

	Feature leftEyeRect{ Feature::FEATURE_EYE_LEFT_RECT, Point2D<double>(beginLeftEyeWidthPosition, eyeHeightBegin) };
	leftEyeRect.addPoint(Point2D<double>( endLeftEyePosition, eyeHeightEnd) );

	Feature rightEyeRect{ Feature::FEATURE_EYE_RIGHT_RECT, Point2D<double>( beginRightEyePosition, eyeHeightBegin) };
	rightEyeRect.addPoint(Point2D<double> (endRightEyePosition, eyeHeightEnd ) );

	features.putFeature(leftEyeRect);
	features.putFeature(rightEyeRect);

	HereBeDragons::AsHisTriumphantPrizeProudOfThisPride(*debugImage, leftEyeRect.getPoints()[0], leftEyeRect.getPoints()[1], RGB(0, 0, 255));
	HereBeDragons::AsHisTriumphantPrizeProudOfThisPride(*debugImage, rightEyeRect.getPoints()[0], rightEyeRect.getPoints()[1], RGB(0, 0, 255));

	ImageIO::saveRGBImage(*debugImage, ImageIO::getDebugFileName("Localization-5/debug.png"));

	delete debugImage;

	return true;
}