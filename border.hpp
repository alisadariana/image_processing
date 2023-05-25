#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

Mat border_of_single_object(Mat image, void *chain_code);
Mat object_reconstruction_from_chain_code(Mat image, std::vector<int> chainCode, int startRow, int startCol);