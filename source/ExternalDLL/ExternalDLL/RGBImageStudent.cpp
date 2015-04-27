#include "RGBImageStudent.h"
#include <stdlib.h>
#include <iostream>

RGBImageStudent::RGBImageStudent() : RGBImage() {
	//int throwError = 0, e = 1 / throwError; //Throws error without the need to include a header
	//TODO: Nothing
}

RGBImageStudent::RGBImageStudent(const RGBImageStudent &other) : RGBImage(other.getWidth(), other.getHeight()) {
	//int throwError = 0, e = 1 / throwError;
	//TODO: Create a copy from the other object

	set(other);
}


RGBImageStudent::RGBImageStudent(const int width, const int height) : RGBImage(width, height) {
	//int throwError = 0, e = 1 / throwError;
	
	set(width, height);
}

RGBImageStudent::~RGBImageStudent() {
	//int throwError = 0, e = 1 / throwError;
	//TODO: delete allocated objects
}

void RGBImageStudent::set(const int width, const int height) {
	RGBImage::set(width, height);
	//int throwError = 0, e = 1 / throwError;

	if (getSize() > 0){
		pixelStorage.clear();
	}

	for (int i = 0; i <= (width * height); i++){
		pixelStorage.push_back(RGB{});
	}
}

void RGBImageStudent::set(const RGBImageStudent &other) {
	RGBImage::set(other.getWidth(), other.getHeight());
	//int throwError = 0, e = 1 / throwError;
	if (getSize() > 0){
		pixelStorage.clear();
	}

	for (int i = 0; i <= other.getSize(); i++){
		pixelStorage.push_back(other.getPixel(i));
	}

}

void RGBImageStudent::setPixel(int x, int y, RGB pixel) {
	//int throwError = 0, e = 1 / throwError;

	if ((x*y) <= getSize()){
		pixelStorage[x + (y*getWidth())] = pixel;
	}
	//TODO: no comment needed :)
}

void RGBImageStudent::setPixel(int i, RGB pixel) {
	//int throwError = 0, e = 1 / throwError;
	if (i <= getSize()){
		pixelStorage[i] = pixel;
	}
}

RGB RGBImageStudent::getPixel(int x, int y) const {
	//int throwError = 0, e = 1 / throwError;

	if ((x*y) <= getSize()){
		return pixelStorage[x + (y*getWidth())];
	}
	return 0;
}

RGB RGBImageStudent::getPixel(int i) const {
	//int throwError = 0, e = 1 / throwError;

	if (i <= getSize()){
		return pixelStorage[i];
	}

	return 0;
}

std::vector<RGB> RGBImageStudent::getPixelStorage(){
	return pixelStorage;
}

void RGBImageStudent::setPixelStorage(std::vector<RGB> ps){
	pixelStorage = ps;
}

int RGBImageStudent::getSize() const{
	return (pixelStorage._Mylast - pixelStorage._Myfirst);
}