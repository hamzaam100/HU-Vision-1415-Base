#pragma once
#include <iostream>
class Kernel
{
public:

	Kernel(int * k = new int[8]) :kernel{ k }{
	}

	~Kernel(){
		delete(kernel);
	}

	void set(Kernel &other){
		this->kernel = other.getKernel();
	}

	void setP(const int p, const int x){
		this->kernel[p] = x;
	}

	int getP(int i) const {
		return kernel[i];
	}

	int * getKernel(){
		return kernel;
	}

	int * kernel;


	//Compare operators
	bool operator== (const Kernel &other) const {
		return (this->kernel == other.kernel);

	}

	bool operator!= (const Kernel &other) const{
		return (this->kernel != other. kernel);
	}

	//By Kernel
	Kernel operator+ (const Kernel &other) const {
		int tmp[8];

		for (int i = 0; i < 9; i++){
			tmp[i] = this->kernel[i] + other.kernel[i];
		}
		return Kernel(tmp);
	}

	Kernel operator- (const Kernel &other) const {
		int tmp[8];

		for (int i = 0; i < 9; i++){
			tmp[i] = this->kernel[i] - other.kernel[i];
		}
		return Kernel(tmp);
	}

	Kernel operator/ (const int &other) const{
		int tmp[8];

		for (int i = 0; i < 9; i++){
			tmp[i] = this->kernel[i] / other;
		}

		return Kernel(tmp);
	}

	Kernel operator=(const Kernel &other){
		this->kernel = other.kernel;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, Kernel &other){
		std::cout << "(";
		for (int i = 0; i < 9; i++){
			std::cout << other.kernel[i] << ", ";
		}

		std::cout << ")";
		return out;
	}
};

