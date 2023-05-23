#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

void object_features_callback(int event, int col, int row, int flags, void* data);
int object_features(String imagePath, bool);
int filter_objects_by_area_and_orientation(String imagePath, int maxArea, float minPhi, float maxPhi);