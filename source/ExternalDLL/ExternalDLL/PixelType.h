/*
* Copyright (c) 2015 DottedEye Designs, Alexander Hustinx, NeoTech Software, Rolf Smit - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
*/

#pragma once
#include <iostream>


struct RGB {

	RGB(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0)
		:r(r), g(g), b(b){
	}

	unsigned char b;
	unsigned char g;
	unsigned char r;

	bool operator== (const RGB &other) const {
		return (this->r == other.r && this->g == other.g && this->b == other.b);

	}

	bool operator!= (const RGB &other) const {
		return (this->r != other.r || this->g != other.g || this->b != other.b);

	}


	RGB operator/ (const int & other) const{
		return RGB(this->r / other, this->g / other, this->b / other);
	}

	RGB operator= (const RGB & rgb) {
		this->b = rgb.b;
		this->g = rgb.g;
		this->r = rgb.r;

		return *this;
	}

	friend std::ostream& operator<< (std::ostream &out, RGB &other){
		std::cout << "(" << (int)other.r << ", " << (int)other.g << ", " << (int)other.b << ")";
		return out;
	}
};

typedef unsigned char Intensity;