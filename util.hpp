#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

int is_inside_image(Mat image, int row, int col);
Mat negate(Mat src);
Mat grayscale_to_binary(Mat src, int threshold);
void rgb_to_hsv(Mat image, Mat hue, Mat saturation, Mat value);
