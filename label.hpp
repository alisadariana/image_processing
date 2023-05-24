#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

extern int is_inside_image(Mat image, int row, int col);

Mat breadth_first_traversal_labeling(Mat image);
Mat two_pass_labeling(Mat image);
