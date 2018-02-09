#ifndef _OPTIONS
#define _OPTIONS

#include <array>
#include <vector>
#include <iostream>
#include "ImageIO.h"
#include "ImageFactory.h"
#include "HereBeDragons.h"
#include "IntensityImage.h"
#include "RGBImage.h"
#include "PixelType.h"

//#define _SPEED_TEST
//#define _DEBUG

typedef std::vector<std::pair<int, int>> Histogram;
typedef std::pair<int, int> Sinus;

enum DIRECTION{
	none = -1,
	up = 0,
	down = 1
};

enum HISTOGRAM_TYPE{
	horizontally = 1,
	vertically = 2
};

inline void movingAverage1D(Histogram & array){
	const int SIZE = array.size();
#ifdef _DEBUG
	std::cout << array[0].first << " : " << array[0].second << std::endl;
#endif
	for (int i = 1; i < SIZE - 1; i++){
		//if (i == 0){
		//	array[i].second = (array[i].second + array[i + 1].second) / 2;
		//}
		//else if (i == SIZE - 1){
		//	array[i].second = (array[i - 1].second + array[i].second) / 2;
		//}
		//else{
			array[i].second = (array[i - 1].second + array[i].second + array[i + 1].second) / 3;
		//}


	}

#ifdef _DEBUG
	std::cout << array.back().first << " : " << array.back().second << std::endl;
#endif
}

inline Histogram createHistogram(int beginX, int endX, int beginY, int endY, HISTOGRAM_TYPE histogramType, const IntensityImage &image, bool average = true){

	Histogram histogram;

	if (histogramType == HISTOGRAM_TYPE::vertically){
		for (int y = beginY, blackCount = 0; y < endY; y++) {
			blackCount = 0;
			for (int x = beginX; x < endX; x++) {
				if (image.getPixel(x, y) == Color::BLACK) {
					blackCount++;
				}
			}
			histogram.push_back(Sinus{ y, blackCount });
#ifdef _DEBUG
			std::cout << y << " : " << blackCount << std::endl;
#endif
		}	
	}
	else{
		for (int x = beginX, blackCount = 0; x < endX; x++) {
			blackCount = 0;
			for (int y = beginY; y < endY; y++) {
				if (image.getPixel(x, y) == Color::BLACK) {
					blackCount++;
				}
			}

			histogram.push_back(Sinus{ x, blackCount });
#ifdef _DEBUG
			std::cout << x << " : " << blackCount << std::endl;
#endif
		}
	}

	if (histogram.empty()){
		return histogram;
	}

	if (average){
		movingAverage1D(histogram);
	}

	return histogram;
}

inline Sinus heigestHistogramSinus(Histogram & histogram) {
	Sinus heigest;
	for (auto & sinus : histogram) {
		if (heigest.second < sinus.second){
			heigest = sinus;
		}
	}
	return heigest;
}

inline Histogram getSinussesFromHistogram(const Histogram & pixelCountContainer, RGBImage * debugImage = nullptr){
	const int	PIXEL_COUNT_CONTAINER = pixelCountContainer.size(),
				LAST_LOOP_INDEX = PIXEL_COUNT_CONTAINER,
				MARGIN = 2;

	Histogram sinuses;
	char direction = DIRECTION::none;
	bool changedDirection = false;

#ifdef _DEBUG
	std::cout << pixelCountContainer[0].first << " : " << pixelCountContainer[0].second << std::endl;
#endif

	for (int i = 1; i < PIXEL_COUNT_CONTAINER; i++){
#ifdef _DEBUG
		std::cout << pixelCountContainer[i].first << " : " << pixelCountContainer[i].second;
#endif

		if (!changedDirection && (direction == DIRECTION::down || direction == DIRECTION::none) && pixelCountContainer[i].second > pixelCountContainer[i - 1].second){
			if (pixelCountContainer[i].second < pixelCountContainer[i + 1].second){
				direction = DIRECTION::up;
				changedDirection = true;
			}
			else if (pixelCountContainer[i].second <= pixelCountContainer[i + 1].second){
				for (int x = i; x < PIXEL_COUNT_CONTAINER; x++){
					if (pixelCountContainer[i].second < pixelCountContainer[x + 1].second){
						direction = DIRECTION::up;
						changedDirection = true;
						break;
					}
					else if (pixelCountContainer[i].second > pixelCountContainer[x + 1].second){
						break;
					}
				}
			}
		}
		else if (!changedDirection && (direction == DIRECTION::up || direction == DIRECTION::none) && pixelCountContainer[i].second < pixelCountContainer[i - 1].second){
			if (pixelCountContainer[i].second > pixelCountContainer[i + 1].second){
				direction = DIRECTION::down;
				changedDirection = true;
			}
			else if (pixelCountContainer[i].second >= pixelCountContainer[i + 1].second){
				for (int x = i; x < PIXEL_COUNT_CONTAINER; x++){
					if (pixelCountContainer[i].second > pixelCountContainer[i + 1].second){
						direction = DIRECTION::down;
						changedDirection = true;
						break;
					}
					else if (pixelCountContainer[i].second > pixelCountContainer[i + 1].second){
						break;
					}
				}
			}
		}

		if (changedDirection){

			if (direction == DIRECTION::up){
#ifdef _DEBUG
				std::cout << " UP";
#endif
				if (sinuses.size() > 1 && pixelCountContainer[i].first - sinuses.back().second >= 15){
					break;
				}

				sinuses.push_back(std::pair<int, int>{ pixelCountContainer[i].first, DIRECTION::none});
				if (debugImage != nullptr){
					HereBeDragons::ButRisingAtThyNameDothPointOutThee(*debugImage, Point2D<double>(0, pixelCountContainer[i - 1].first), Point2D<double>(debugImage->getWidth(), pixelCountContainer[i - 1].first), RGB(255, 0, 255));
				}

				changedDirection = false;
			}
			else if (direction == DIRECTION::down && (sinuses.empty() || pixelCountContainer[i].second < MARGIN || i == LAST_LOOP_INDEX)){
#ifdef _DEBUG
				std::cout << " DOWN";
#endif
				if (sinuses.empty()){
					sinuses.push_back(std::pair<int, int>{DIRECTION::none, pixelCountContainer[i].first});
				}
				else{
					sinuses.back().second = pixelCountContainer[i].first;
				}

				if (debugImage != nullptr){
					HereBeDragons::ButRisingAtThyNameDothPointOutThee(*debugImage, Point2D<double>(0, pixelCountContainer[i].first), Point2D<double>(debugImage->getWidth(), pixelCountContainer[i].first), RGB(255, 255, 0));
				}

				changedDirection = false;
			}
		}
#ifdef _DEBUG
		std::cout << std::endl;
#endif
	}

#ifdef _DEBUG
	std::cout << std::endl << pixelCountContainer.back().first << " : " << pixelCountContainer.back().second << std::endl;
#endif

	return sinuses;
}

#endif