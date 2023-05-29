#ifndef OPEN_H
#define OPEN_H
	#include <stdio.h>
	#include <opencv2/opencv.hpp>
#endif
using namespace cv;

int store_correspondences();
int display_correspondences();
int show_morphing_process(double t);
int save_morphing_frames(int numberFrames);
int add_correspondences();
