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
int compute_histogram(Mat image, int *histogram);
int compute_histogram(Mat image, int *histogram, float *pdf);
int compute_cumulative_histogram(int *histogram, int *cumulativeHistogram);
void compute_histogram_bins(Mat image, int *histogram, int numberBins);
Mat create_histogram_image(const String &name, int *hist, const int height,
			   const int width);
float compute_mean(int *histogram, int numberPixels);
float compute_standard_deviation(Mat image, float mean);
Mat automatic_thresholding(Mat src);
Mat histogram_stretch_shrink(Mat image, int newMin, int newMax);
Mat gamma_correction(Mat image, float gamma);
Mat histogram_equalization(Mat src);
