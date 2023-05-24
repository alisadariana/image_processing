#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

Mat breadth_first_traversal_labeling(Mat image);
Mat two_pass_labeling(Mat image);