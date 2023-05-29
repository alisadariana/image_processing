#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

extern int is_inside_image(Mat src, int row, int colc);

Mat filter_median(Mat src, int kernelSize);
Mat filter_gaussian_2d(Mat src, int kernelSize);
Mat filter_gaussian_vec(Mat src, int kernelSize);
