#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include "StudentPreProcessing.h"
#include "ImageIO.h"
#include "ImageFactory.h"
#include "HereBeDragons.h"

/**
* @function CannyThreshold
* @brief Trackbar callback - Canny thresholds input with a ratio 1:3
*/
void Canny(const IntensityImage & originalImage);
void Sobel(const IntensityImage & originalImage);
void Laplace(const IntensityImage & originalImage);
void ItensityImageToMat(const IntensityImage &src, cv::Mat &dst);
void MatToItensityImage(const cv::Mat &source, IntensityImage &dst);
void Resize(const IntensityImage & originalImage, int interpolation, std::string name);
void saveDebugImage(const IntensityImage & image, std::string fileName);

const StudentPreProcessing::LOGKernel LOG_KERNEL = {
	{
			0, 0, 0, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 0, 0, 0,
			1, 1, 1, -4, -4, -4, 1, 1, 1,
			1, 1, 1, -4, -4, -4, 1, 1, 1,
			1, 1, 1, -4, -4, -4, 1, 1, 1,
			0, 0, 0, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 0, 0, 0,
			0, 0, 0, 1, 1, 1, 0, 0, 0
	}
};

IntensityImage * StudentPreProcessing::stepToIntensityImage(const RGBImage &image) const {
	if (image.isEmpty()){
		return nullptr;
	}

	IntensityImageStudent * returnImage = new IntensityImageStudent(image.getWidth(), image.getHeight());

	for (int i = 0; i < image.getSize(); i++) {
		RGB pixel{ image.getPixel(i) };
		returnImage->setPixel(i, static_cast<Intensity>(0.0722 * pixel.b) + static_cast<Intensity>(0.7152 * pixel.g) + static_cast<Intensity>(0.2126 * pixel.r));
	}

	return returnImage;
}

IntensityImage * StudentPreProcessing::stepScaleImage(const IntensityImage &image) const {
	const int	HEIGHT = image.getHeight() - 1,
				WIDTH = image.getWidth() - 1,
				MAX_RESOLUTION = 200,
				FACTOR = static_cast<int>(MAX_RESOLUTION / std::fmax(HEIGHT, WIDTH)),
				HALF_FACTOR = static_cast<int>(FACTOR / 2);

	if (HEIGHT >= MAX_RESOLUTION || WIDTH >= MAX_RESOLUTION || FACTOR <= 1){
		return new IntensityImageStudent(image);
	}

	IntensityImageStudent * returnImage = new IntensityImageStudent((WIDTH - 1) * FACTOR, (HEIGHT - 1) * FACTOR);
	Intensity currentPixel;

	for (int x = 1, newX; x < WIDTH; x++) {
		newX = (x - 1) * FACTOR;
		for (int y = 1, newY; y < HEIGHT; y++) {
			newY = (y - 1) * FACTOR;
			currentPixel = image.getPixel(x, y);

			for (int xf = 0; xf < FACTOR; xf++){
				for (int yf = 0; yf < FACTOR; yf++){
					returnImage->setPixel(newX + xf, newY + yf, currentPixel);
				}
			}
		}
	}

	return returnImage;
}

IntensityImage * StudentPreProcessing::stepEdgeDetection(const IntensityImage &image) const {
	if (image.isEmpty()){
		return nullptr;
	}

	const int	HEIGHT = image.getHeight(),
				WIDTH = image.getWidth();

	IntensityImageStudent * returnImage = new IntensityImageStudent{ WIDTH, HEIGHT };
			
	// LoG
	for (int x = LOG_KERNEL_HALF_SIZE; x < WIDTH - LOG_KERNEL_HALF_SIZE; x++) {
		for (int y = LOG_KERNEL_HALF_SIZE, total; y < HEIGHT - LOG_KERNEL_HALF_SIZE; y++) {

			total = 0;
			for (int logKernelX = -LOG_KERNEL_HALF_SIZE; logKernelX <= LOG_KERNEL_HALF_SIZE; logKernelX++) {
				for (int logKernelY = -LOG_KERNEL_HALF_SIZE, LoGValue; logKernelY <= LOG_KERNEL_HALF_SIZE; logKernelY++) {
					LoGValue = LOG_KERNEL[(LOG_KERNEL_HALF_SIZE + logKernelY) * LOG_KERNEL_SIZE + (LOG_KERNEL_HALF_SIZE + logKernelX)];
					if (LoGValue == 1){
						total += static_cast<int>(image.getPixel(x + logKernelX, y + logKernelY));
					}
					else if (LoGValue != 0){
						total += static_cast<int>(image.getPixel(x + logKernelX, y + logKernelY)) * LoGValue;
					}
				}
			}
		
			if (total < 0) {
				returnImage->setPixel(x, y, Color::BLACK);
			}
			else if (total > 255) {
				returnImage->setPixel(x, y, Color::WHITE);
			}
			else{
				returnImage->setPixel(x, y, static_cast<Intensity>(total));
			}
		}
	}

	return returnImage;
}

IntensityImage * StudentPreProcessing::stepThresholding(const IntensityImage &image) const {
	if (image.isEmpty()){
		return nullptr;
	}

	const int	HEIGHT = image.getHeight(),
				WIDTH = image.getWidth();

	IntensityImageStudent * returnImage = new IntensityImageStudent(image);

	for (int i = 0; i < image.getSize(); i++) {
		returnImage->setPixel(i, image.getPixel(i) > TRESHOLD ? Color::BLACK : Color::WHITE);
	}

	return returnImage;
}

void Resize(const IntensityImage & originalImage, int interpolation, std::string name){
	IntensityImageStudent image{ originalImage };

	cv::Mat detected_edges, src, dst, src_gray;
	int lowThreshold = 35;
	int ratio = 3;
	int kernel_size = 3;

	ItensityImageToMat(image, src);
	ItensityImageToMat(image, src_gray);

	dst.create(src.size(), src.type());

	resize(src, dst, cv::Size(), 4.0, 4.0, interpolation);

	MatToItensityImage(dst, image);

	saveDebugImage(image, name);
}

void Canny(const IntensityImage & originalImage)
{
	IntensityImageStudent image{ originalImage };

	cv::Mat detected_edges, src, dst, src_gray;
	int lowThreshold = 35;
	int ratio = 3;
	int kernel_size = 3;
	
	ItensityImageToMat(image, src);
	ItensityImageToMat(image, src_gray);

	dst.create(src.size(), src.type());

	/// Reduce noise with a kernel 3x3
	cv::blur(src_gray, detected_edges, cv::Size(3, 3));

	/// Canny detector
	cv::Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

	/// Using Canny's output as a mask, we display our result
	dst = cv::Scalar::all(0);

	src.copyTo(dst, detected_edges);

	MatToItensityImage(dst, image);

	saveDebugImage(image, "Canny");
}

void Sobel(const IntensityImage & originalImage)
{
	IntensityImageStudent image{ originalImage };

	cv::Mat grad,src, src_gray;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	ItensityImageToMat(image, src);
	ItensityImageToMat(image, src_gray);


	cv::GaussianBlur(src, src, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
	
	/// Generate grad_x and grad_y
	cv::Mat grad_x, grad_y;
	cv::Mat abs_grad_x, abs_grad_y;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	cv::Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	cv::Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

	MatToItensityImage(grad, image);

	saveDebugImage(image, "Sobel");
}

void Laplace(const IntensityImage & originalImage)
{
	IntensityImageStudent image{ originalImage };

	cv::Mat src, src_gray, dst;
	int kernel_size = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	ItensityImageToMat(image, src);
	ItensityImageToMat(image, src_gray);

	/// Remove noise by blurring with a Gaussian filter
	cv::GaussianBlur(src, src, cv::Size(5, 5), 0, 0, cv::BORDER_DEFAULT);

	/// Apply Laplace function
	cv::Mat abs_dst;

	cv::Laplacian(src_gray, dst, ddepth, kernel_size, scale, delta, cv::BORDER_DEFAULT);
	cv::convertScaleAbs(dst, abs_dst);

	MatToItensityImage(abs_dst, image);

	saveDebugImage(image, "Laplace");
}

void saveDebugImage(const IntensityImage & image, std::string fileName){
	RGBImage * debugImage = ImageFactory::newRGBImage();
	ImageIO::intensityToRGB(image, *debugImage);
	ImageIO::saveRGBImage(*debugImage, ImageIO::getDebugFileName("EDGE/" + fileName  + ".png"));

	delete debugImage;
}

void ItensityImageToMat(const IntensityImage &src, cv::Mat &dst){
	int w = src.getWidth();
	int h = src.getHeight();

	dst.create(h, w, CV_8UC1);

	for (int x = 0; x < dst.cols; x++) {
		for (int y = 0; y < dst.rows; y++) {
			dst.at<uchar>(y, x) = src.getPixel(x, y);
		}
	}
}

void MatToItensityImage(const cv::Mat &source, IntensityImage &dst) {
	int type = source.type();
	if (type != CV_8UC1) {
		throw std::exception("OpenCV Mat source image not of type CV_8UC1!");
	}

	dst.set(source.cols, source.rows);

	for (int x = 0; x < source.cols; x++) {
		for (int y = 0; y < source.rows; y++) {
			dst.setPixel(x, y, source.at<uchar>(y, x));
		}
	}
}