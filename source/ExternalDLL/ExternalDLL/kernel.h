#pragma once
#include <iostream>

class Kernel
{
private:
	int* kernel = nullptr;
	int width;
	int height;
public:
	Kernel();
	Kernel(int width, int height);
	Kernel(int width, int height, int* data);
	~Kernel();

	void set(const Kernel& other) { *this = other; }
	void set(int x, int y, int value) const { kernel[x + y*width] = value; }
	void set(int i, int value) const { kernel[i] = value; }

	int get(int x, int y) const { return kernel[x + y*width]; };
	int get(int i) const { return kernel[i]; };

	int getWidth() const { return width; }
	int getHeight() const { return height; }

	int getKernelSum() const;

	//Compare operators
	bool operator== (const Kernel &other) const {
		return (this->kernel == other.kernel);

	}

	bool operator!= (const Kernel &other) const {
		return (this->kernel != other.kernel);
	}

	//By Kernel
	Kernel operator+ (const Kernel &other) const {
		Kernel tmp(*this);
		for (int i = 0; i < width*height; i++){
			tmp.set(i, tmp.get(i) + other.kernel[i]);
		}
		return tmp;
	}

	Kernel operator- (const Kernel &other) const {
		Kernel tmp(*this);
		for (int i = 0; i < width*height; i++){
			tmp.set(i, tmp.get(i) - other.kernel[i]);
		}
		return tmp;
	}

	Kernel operator/ (const int other) const {
		Kernel tmp(*this);
		for (int i = 0; i < width*height; i++) {
			tmp.set(i, tmp.get(i) / other);
		}
		return tmp;
	}

	Kernel operator=(const Kernel &other) {
		this->width = other.width;
		this->height = other.height;
		this->kernel = other.kernel;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, Kernel &other) {
		std::cout << "(";
		for (int i = 0; i < other.width*other.height; i++){
			std::cout << other.kernel[i] << ", ";
		}
		std::cout << ")";
		return out;
	}
};