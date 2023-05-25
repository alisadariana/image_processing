#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

extern int is_inside_image(Mat image, int row, int col);
extern Mat negate(Mat src);

Mat dilate(Mat src, Mat structuringElement);
Mat erode(Mat src, Mat structuringElement);
Mat open(Mat src, Mat structuringElement);
Mat close(Mat src, Mat structuringElement);
Mat extract_boundary(Mat src);
Mat fill_region(Mat src, int startRow, int startCol);