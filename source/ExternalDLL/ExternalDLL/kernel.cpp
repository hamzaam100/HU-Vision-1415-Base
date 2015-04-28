#include "Kernel.h"

Kernel::Kernel()
{

}

Kernel::Kernel(int width, int height) : width(width), height(height)
{

}

Kernel::Kernel(int width, int height, int* data) : width(width), height(height)
{
	kernel = new int[width*height];
	for (int i = 0; i < width*height; i++) {
		kernel[i] = data[i];
	}
}

Kernel::~Kernel()
{
	if (kernel != nullptr)
		delete[] kernel;
}

int Kernel::getKernelSum() const
{
	int tmp = 0;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			tmp += this->get(x, y);
		}
	}
	return tmp;
}