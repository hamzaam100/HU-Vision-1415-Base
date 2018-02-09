#include <math.h>
#include "StudentExtraction.h"
#include "RGBImage.h"
#include "ImageIO.h"
#include "ImageFactory.h"
#include "HereBeDragons.h"
#include "StudentLocalization.h"

bool StudentExtraction::stepExtractEyes(const IntensityImage &image, FeatureMap &features) const {
	RGBImage * debugImage = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage);

	std::vector<Point2D<double>> &	leftEyePoints = features.getFeature(Feature::FEATURE_EYE_LEFT_RECT).getPoints(),
									rightEyePoints = features.getFeature(Feature::FEATURE_EYE_RIGHT_RECT).getPoints();

	const int	BEGIN_LEFT_EYE = static_cast<int>(std::fmin(leftEyePoints[0].x, leftEyePoints[1].x)),
				END_LEFT_EYE = static_cast<int>(std::fmax(leftEyePoints[0].x, leftEyePoints[1].x)),
				TOP_LEFT_EYE = static_cast<int>(std::fmin(leftEyePoints[0].y, leftEyePoints[1].y)),
				BOTTOM_LEFT_EYE = static_cast<int>(std::fmax(leftEyePoints[0].y, leftEyePoints[1].y)),

				WIDTH_LEFT_EYE = END_LEFT_EYE - BEGIN_LEFT_EYE,
				HEIGHT_LEFT_EYE = BOTTOM_LEFT_EYE - TOP_LEFT_EYE,
				SIZE_LEFT_EYE = WIDTH_LEFT_EYE * HEIGHT_LEFT_EYE,

				BEGIN_RIGHT_EYE = static_cast<int>(std::fmin(rightEyePoints[0].x, rightEyePoints[1].x)),
				END_RIGHT_EYE = static_cast<int>(std::fmax(rightEyePoints[0].x, rightEyePoints[1].x)),
				TOP_RIGHT_EYE = static_cast<int>(std::fmin(rightEyePoints[0].y, rightEyePoints[1].y)),
				BOTTOM_RIGHT_EYE = static_cast<int>(std::fmax(rightEyePoints[0].y, rightEyePoints[1].y)),

				WIDTH_RIGHT_EYE = END_RIGHT_EYE - BEGIN_RIGHT_EYE,
				HEIGHT_RIGHT_EYE = BOTTOM_RIGHT_EYE - TOP_RIGHT_EYE,
				SIZE_RIGHT_EYE = WIDTH_RIGHT_EYE * HEIGHT_RIGHT_EYE;

	if (SIZE_RIGHT_EYE == 0 || SIZE_LEFT_EYE == 0) {
		return false;
	}

	IntensityImageStudent leftEye{ WIDTH_LEFT_EYE, HEIGHT_LEFT_EYE };
	IntensityImageStudent rightEye{ WIDTH_RIGHT_EYE, HEIGHT_RIGHT_EYE };

	for (int x = BEGIN_LEFT_EYE; x < END_LEFT_EYE; x++) {
		for (int y = TOP_LEFT_EYE; y < BOTTOM_LEFT_EYE; y++) {
			leftEye.setPixel(x - BEGIN_LEFT_EYE, y - TOP_LEFT_EYE, image.getPixel(x, y));
		}
	}

	for (int x = BEGIN_RIGHT_EYE; x < END_RIGHT_EYE; x++) {
		for (int y = TOP_RIGHT_EYE; y < BOTTOM_RIGHT_EYE; y++) {
			rightEye.setPixel(x - BEGIN_RIGHT_EYE, y - TOP_RIGHT_EYE, image.getPixel(x, y));
		}
	}	

	//HereBeDragons::TriumphInLoveFleshStaysNoFatherReason(*debugImage, Point2D<double>(END_LEFT_EYE, TOP_LEFT_EYE), RGB(255, 0, 0));
	//HereBeDragons::TriumphInLoveFleshStaysNoFatherReason(*debugImage, Point2D<double>(END_LEFT_EYE, BOTTOM_LEFT_EYE), RGB(255, 0, 0));

	int beginLeftEyeX = 0,
		endLeftEyeX = WIDTH_LEFT_EYE,
		beginRightEyeX = 0,
		endRightEyeX = WIDTH_RIGHT_EYE,

		beginLeftEyeY = 0,
		endLeftEyeY = HEIGHT_LEFT_EYE,
		beginRightEyeY = 0,
		endRightEyeY = HEIGHT_RIGHT_EYE;

#ifdef _DEBUG
	std::cout << std::endl;
	std::cout << "WIDTH LEFT EYE :" << std::endl;
#endif
	Histogram histogram = createHistogram(0, WIDTH_LEFT_EYE + 1, 0, HEIGHT_LEFT_EYE + 1, HISTOGRAM_TYPE::horizontally, leftEye, false);
	determineBeginAndEndOfEye(histogram, beginLeftEyeX, endLeftEyeX);

#ifdef _DEBUG
	std::cout << std::endl;
	std::cout << "HEIGHT LEFT EYE :" << std::endl;
#endif
	histogram = createHistogram(beginLeftEyeX, endLeftEyeX + 1, 0, HEIGHT_LEFT_EYE + 1, HISTOGRAM_TYPE::vertically, leftEye, false);
	determineBeginAndEndOfEye(histogram, beginLeftEyeY, endLeftEyeY);

	//delete debugImage;
	//debugImage = ImageFactory::newRGBImage();
	//ImageIO::intensityToRGB(image, *debugImage);

#ifdef _DEBUG
	std::cout << std::endl;
	std::cout << "WIDTH RIGHT EYE :" << std::endl;
#endif
	histogram = createHistogram(0, WIDTH_RIGHT_EYE + 1, 0, HEIGHT_RIGHT_EYE + 1, HISTOGRAM_TYPE::horizontally, rightEye, false);
	determineBeginAndEndOfEye(histogram, beginRightEyeX, endRightEyeX);

#ifdef _DEBUG
	std::cout << std::endl;
	std::cout << "HEIGHT RIGHT EYE ";
#endif
	histogram = createHistogram(beginRightEyeX, endRightEyeX + 1, 0, HEIGHT_RIGHT_EYE + 1, HISTOGRAM_TYPE::vertically, rightEye, false);
	determineBeginAndEndOfEye(histogram, beginRightEyeY, endRightEyeY);

	Feature leftEyeRect{ Feature::FEATURE_EYE_LEFT_RECT, Point2D<double>(beginLeftEyeX + BEGIN_LEFT_EYE, beginLeftEyeY + TOP_LEFT_EYE) };
	leftEyeRect.addPoint(Point2D<double>(endLeftEyeX + BEGIN_LEFT_EYE, endLeftEyeY + TOP_LEFT_EYE));

	Feature rightEyeRect{ Feature::FEATURE_EYE_RIGHT_RECT, Point2D<double>(beginRightEyeX + BEGIN_RIGHT_EYE, beginRightEyeY + TOP_RIGHT_EYE) };
	rightEyeRect.addPoint(Point2D<double>(endRightEyeX + BEGIN_RIGHT_EYE, endRightEyeY + TOP_RIGHT_EYE));

	features.putFeature(leftEyeRect);
	features.putFeature(rightEyeRect);

	HereBeDragons::AsHisTriumphantPrizeProudOfThisPride(*debugImage, leftEyeRect[0], leftEyeRect[1], RGB(0, 0, 255));
	HereBeDragons::AsHisTriumphantPrizeProudOfThisPride(*debugImage, rightEyeRect[0], rightEyeRect[1], RGB(0, 0, 255));

	ImageIO::saveRGBImage(*debugImage, ImageIO::getDebugFileName("Extraction-1/debug.png"));

	delete debugImage;

	return true;
}

bool StudentExtraction::stepExtractNose(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

bool StudentExtraction::stepExtractMouth(const IntensityImage &image, FeatureMap &features) const {
	return false;
}

void StudentExtraction::determineBeginAndEndOfEye(Histogram & histogram, int & begin, int & end) const{
	if (histogram.empty()){
		return;
	}

	const int	SIZE = histogram.size() - 1,
				HALF_SIZE = SIZE / 2,
				MARGIN = 1;

	bool	updatedBegin = false,
			updatedEnd = false;
	for (int i = 0; i < HALF_SIZE; i++){
		if (!updatedBegin && histogram[i].second <= MARGIN){
			bool findOther = false;
			for (int x = i; x < HALF_SIZE; x++){
				if (!findOther && histogram[x].second > MARGIN){
					begin = histogram[x].first;
					updatedBegin = true;
					findOther = true;
				}
				else if (findOther && histogram[x].second <= MARGIN){
					updatedBegin = false;
					break;
				}
			}
		}

		if (!updatedEnd && histogram[SIZE - i].second <= MARGIN){
			bool findOther = false;
			for (int x = SIZE - i; x > HALF_SIZE; x--){
				if (!findOther && histogram[x].second > MARGIN){
					end = histogram[x].first;
					updatedEnd = true;
					findOther = true;
				}
				else if (findOther && histogram[x].second <= MARGIN){
					updatedEnd = false;
					break;
				}
			}
		}

		if ((updatedEnd && updatedBegin)){
			break;
		}
	}
}