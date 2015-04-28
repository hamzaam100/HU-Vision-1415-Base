#include "ConvolutionHelper.h"
#include "ImageFactory.h"

const int ConvolutionHelper::sobelKernel[] = { 1, 2, 3 };
const int ConvolutionHelper::cannyKernel[] = { 1, 2, 3 };
const int ConvolutionHelper::prewittKernel[] = { 1, 2, 3 };

ConvolutionHelper::ConvolutionHelper()
{
}


ConvolutionHelper::~ConvolutionHelper()
{
}

IntensityImage* ConvolutionHelper::ConvolveImage(const IntensityImage& image, const Kernel& kernel)
{
	IntensityImage* returnImage = ImageFactory::newIntensityImage(image.getWidth(), image.getHeight());
	int kernelPadding = (int)(kernel.getWidth() / 2.0);

	for (int x = kernelPadding; x < image.getWidth() - (kernelPadding * 2); x++) {
		for (int y = kernelPadding; y < image.getHeight() - (kernelPadding * 2); y++) {

			int tmp = 0;
			for (int kx = 0; kx < kernel.getWidth(); kx++) {
				for (int ky = 0; ky < kernel.getHeight(); ky++) {
					tmp += image.getPixel(x+kx-kernelPadding, y+ky-kernelPadding) * kernel.get(kx, ky);
				}
			}
			tmp = abs(tmp);
			tmp = tmp > 255 ? 255 : tmp;
			tmp = tmp < 0 ? 0 : tmp;
			returnImage->setPixel(x, y, tmp);
		}
	}
	return returnImage;
}
