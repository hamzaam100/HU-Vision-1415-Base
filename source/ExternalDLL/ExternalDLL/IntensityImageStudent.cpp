#include "IntensityImageStudent.h"
#include <iostream>

IntensityImageStudent::IntensityImageStudent() : IntensityImage() {
	//int throwError = 0, e = 1 / throwError; //Throws error without the need to include a header
	//TODO: Nothing
}

IntensityImageStudent::IntensityImageStudent(const IntensityImageStudent &other) : IntensityImage(other.getWidth(), other.getHeight()) {
	//int throwError = 0, e = 1 / throwError;
	//TODO: Create a copy from the other object

	set(other);
}

IntensityImageStudent::IntensityImageStudent(const int width, const int height) : IntensityImage(width, height) {
	//int throwError = 0, e = 1 / throwError;
	//TODO: Initialize pixel storage

	set(width, height);
}

IntensityImageStudent::~IntensityImageStudent() {
	//int throwError = 0, e = 1 / throwError;
	//TODO: delete allocated objects
}

void IntensityImageStudent::set(const int width, const int height) {
	IntensityImage::set(width, height);
	//int throwError = 0, e = 1 / throwError;
	//TODO: resize or create a new pixel storage (Don't forget to delete the old storage)

	if (getSize() > 0){
		pixelStorage.clear();
	}

	for (int i = 0; i <= (width * height); i++){
		pixelStorage.push_back(Intensity{});
	}
}

void IntensityImageStudent::set(const IntensityImageStudent &other) {
	IntensityImage::set(other.getWidth(), other.getHeight());
	//int throwError = 0, e = 1 / throwError;
	//TODO: resize or create a new pixel storage and copy the object (Don't forget to delete the old storage)

	if (getSize() > 0){
		pixelStorage.clear();
	}

	for (int i = 0; i <= other.getSize(); i++){
		pixelStorage.push_back(other.getPixel(i));
	}
}

void IntensityImageStudent::setPixel(int x, int y, Intensity pixel) {
	//int throwError = 0, e = 1 / throwError;
	//TODO: no comment needed :)

	if ((x*y) <= getSize()){
		pixelStorage[x + (y*getWidth())] = pixel;
	}
}

void IntensityImageStudent::setPixel(int i, Intensity pixel) {
	//int throwError = 0, e = 1 / throwError;

	if (i <= getSize()){
		pixelStorage[i] = pixel;
	}
}

Intensity IntensityImageStudent::getPixel(int x, int y) const {
	//int throwError = 0, e = 1 / throwError;
	//TODO: no comment needed :)

	if ((x*y) <= getSize()){
		return pixelStorage[x + (y*getWidth())];
	}

	return 0;
}

Intensity IntensityImageStudent::getPixel(int i) const {
	//int throwError = 0, e = 1 / throwError;
	//TODO: see setPixel(int i, RGB pixel)

	if (i <= getSize()){
		return pixelStorage[i];
	}

	return 0;
}

std::vector<Intensity> IntensityImageStudent::getPixelStorage(){
	return pixelStorage;
}

void IntensityImageStudent::setPixelStorage(std::vector<Intensity> ps){
	pixelStorage = ps;
}

int IntensityImageStudent::getSize() const{
	return (pixelStorage._Mylast - pixelStorage._Myfirst);
}