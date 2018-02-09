#include "RGBImageStudent.h"

RGBImageStudent::RGBImageStudent() : RGBImage(), pixelMap(nullptr) { }

RGBImageStudent::RGBImageStudent(const RGBImageStudent &other) : RGBImage(other.getWidth(), other.getHeight()), pixelMap(nullptr) {
	const int SIZE = other.getSize();
	if (SIZE > 0) {
		pixelMap = new RGB[SIZE];
		for (int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

RGBImageStudent::RGBImageStudent(const RGBImage &other) : RGBImage(other.getWidth(), other.getHeight()), pixelMap(nullptr) {
	const int SIZE = other.getSize();
	if (SIZE > 0) {
		pixelMap = new RGB[SIZE];
		for (int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

RGBImageStudent::RGBImageStudent(const int width, const int height) : RGBImage(width, height), pixelMap(nullptr) {
	const int SIZE = getSize();
	if (SIZE > 0) {
		pixelMap = new RGB[SIZE]{};
	}
}

RGBImageStudent::~RGBImageStudent() {
	delete[] pixelMap;
}

void RGBImageStudent::set(const int width, const int height) {
	RGBImage::set(width, height);
	const int SIZE = getSize();

	if (SIZE > 0) {
		delete[] pixelMap;
		pixelMap = new RGB[getSize()];
	}
}

void RGBImageStudent::set(const RGBImage &other) {
	const int SIZE = other.getSize();
	RGBImage::set(other.getWidth(), other.getHeight());

	if (SIZE > 0) {
		delete[] pixelMap;
		pixelMap = new RGB[SIZE];
		for (int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

void RGBImageStudent::set(const RGBImageStudent &other) {
	const int	SIZE = other.getSize();
	RGBImage::set(other.getWidth(), other.getHeight());

	if (SIZE > 0) {
		delete[] pixelMap;
		pixelMap = new RGB[SIZE];
		for (int i = 0; i < SIZE; i++) {
			pixelMap[i] = other.getPixel(i);
		}
	}
}

void RGBImageStudent::setPixel(int x, int y, RGB pixel) {
	pixelMap[y * getWidth() + x] = pixel;
}

void RGBImageStudent::setPixel(int i, RGB pixel) {
	pixelMap[i] = pixel;
}

RGB RGBImageStudent::getPixel(int x, int y) const {
	return pixelMap[y * getWidth() + x];
}

RGB RGBImageStudent::getPixel(int i) const {
	return pixelMap[i];
}