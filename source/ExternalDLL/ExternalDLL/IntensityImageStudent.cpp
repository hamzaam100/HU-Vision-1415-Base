#include "IntensityImageStudent.h"

IntensityImageStudent::IntensityImageStudent() : IntensityImage(), pixelMap(nullptr) { }

IntensityImageStudent::IntensityImageStudent(const IntensityImageStudent &other) : IntensityImage(other.getWidth(), other.getHeight()), pixelMap(nullptr) {
	const int SIZE = other.getSize();
	if (SIZE > 0) {
		pixelMap = new Intensity[SIZE];
		for (int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

IntensityImageStudent::IntensityImageStudent(const IntensityImage &other) : IntensityImage(other.getWidth(), other.getHeight()), pixelMap(nullptr) {
	const int SIZE = other.getSize();
	if(SIZE > 0) {
		pixelMap = new Intensity[SIZE];
		for(int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

IntensityImageStudent::IntensityImageStudent(const int width, const int height) : IntensityImage(width, height), pixelMap(nullptr) {
	const int SIZE = getSize();
	if (SIZE > 0) {
		pixelMap = new Intensity[SIZE]{};
	}
}

IntensityImageStudent::~IntensityImageStudent() {
	delete[] pixelMap;
}

void IntensityImageStudent::set(const int width, const int height) {
	IntensityImage::set(width, height);
	const int SIZE = getSize();

	if (SIZE > 0) {
		delete[] pixelMap;
		pixelMap = new Intensity[getSize()];
	}
}

void IntensityImageStudent::set(const IntensityImage &other) {
	const int	SIZE = other.getSize();
	IntensityImage::set(other.getWidth(), other.getHeight());

	if (SIZE > 0) {
		delete[] pixelMap;
		pixelMap = new Intensity[SIZE];
		for (int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

void IntensityImageStudent::set(const IntensityImageStudent &other) {
	const int	SIZE = other.getSize();
	IntensityImage::set(other.getWidth(), other.getHeight());

	if (SIZE > 0) {
		delete[] pixelMap;
		pixelMap = new Intensity[SIZE];
		for (int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

void IntensityImageStudent::setPixel(int x, int y, Intensity pixel) {
	pixelMap[ y * getWidth() + x ] = pixel;
}

void IntensityImageStudent::setPixel(int i, Intensity pixel) {
	pixelMap[i] = pixel;
}

Intensity IntensityImageStudent::getPixel(int x, int y) const {
	return pixelMap[ y * getWidth() + x ];
}

Intensity IntensityImageStudent::getPixel(int i) const {
	return pixelMap[i];
}