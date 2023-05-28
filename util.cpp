#include "util.hpp"

int is_inside_image(Mat image, int row, int col)
{
	if (row >= image.rows)
		return 0;
	if (row < 0)
		return 0;
	if (col >= image.cols)
		return 0;
	if (col < 0)
		return 0;
	return 1;
}

Mat negate(Mat src)
{
	int i, j;
	int aux;
	Mat dst;

	dst = Mat(src.rows, src.cols, CV_8UC1);

	for (i = 0; i < src.rows; i++)
		for (j = 0; j < src.cols; j++) {
			aux = 255 - src.at<uchar>(i, j);
			if (aux < 0)
				dst.at<uchar>(i, j) = 0;
			else if (aux > 255)
				dst.at<uchar>(i, j) = 255;
			else
				dst.at<uchar>(i, j) = aux;
		}

	return dst;
}

Mat grayscale_to_binary(Mat src, int threshold)
{
	int i, j;
	Mat dst;

	dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));

	for (i = 0; i < src.rows; i++)
		for (j = 0; j < src.cols; j++)
			if (src.at<uchar>(i, j) <= threshold)
				dst.at<uchar>(i, j) = 0;

	return dst;
}

void rgb_to_hsv(Mat image, Mat hue, Mat saturation, Mat value)
{
	int i, j;
	float aux;
	Vec3b pixel;
	float r, g, b;
	float max, min, c;
	float h, s, v;

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			pixel = image.at<Vec3b>(i, j);
			r = (float)pixel[2] / 255;
			g = (float)pixel[1] / 255;
			b = (float)pixel[0] / 255;

			max = r;
			if (g > max)
				max = g;
			if (b > max)
				max = b;

			min = r;
			if (g < min)
				min = g;
			if (b < min)
				min = b;

			c = max - min;

			v = max;

			if (v)
				s = c / v;
			else
				s = 0;

			if (c) {
				if (max == r)
					h = 60 * (g - b) / c;
				if (max == g)
					h = 120 + 60 * (b - r) / c;
				if (max == b)
					h = 180 + 60 * (r - g) / c;
			}
			else {
				h = 0;
			}

			if (h < 0)
				h = h + 360;

			hue.at<uchar>(i, j) = h * 255 / 360;
			saturation.at<uchar>(i, j) = s * 255;
			value.at<uchar>(i, j) = v * 255;
		}
	}
}


int compute_histogram(Mat image, int *histogram)
{
	int i, j;

	if (histogram == NULL)
		return -1;

	for (i = 0; i < 256; i++)
		histogram[i] = 0;

	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			histogram[image.at<uchar>(i, j)]++;

	return 0;
}

int compute_histogram(Mat image, int *histogram, float *pdf)
{
	int i, j;
	int resolution;

	if (histogram == NULL)
		return -1;

	if (pdf == NULL)
		return -1;

	for (i = 0; i < 256; i++)
		histogram[i] = 0;

	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			histogram[image.at<uchar>(i, j)]++;

	resolution = image.rows * image.cols;

	for (i = 0; i < 256; i++)
		pdf[i] = (float) histogram[i] / resolution;

	return 0;
}

int compute_cumulative_histogram(int *histogram, int *cumulativeHistogram)
{
	int i;

	if (histogram == NULL)
		return -1;

	if (cumulativeHistogram == NULL)
		return -1;

	cumulativeHistogram[0] = histogram[0];

	for (i = 1; i < 256; i++)
		cumulativeHistogram[i] =
		    cumulativeHistogram[i - 1] + histogram[i];

	return 0;
}

void compute_histogram_bins(Mat image, int *histogram, int numberBins)
{
	int step = 256 / numberBins;
	int i, j;

	for (i = 0; i < numberBins; i++) {
		histogram[i] = 0;
	}

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			histogram[image.at<uchar>(i, j) / step]++;
		}
	}
}

Mat create_histogram_image(const String &name, int *hist, const int height,
			   const int width)
{
	cv::Mat imgHist(height, width, CV_8UC3, CV_RGB(255, 255, 255));
	int max_hist = 0;
	double scale = 1.0;

	for (int i = 0; i < width; i++)
		if (hist[i] > max_hist)
			max_hist = hist[i];

	scale = (double) height / max_hist;
	int baseline = height - 1;
	for (int x = 0; x < width; x++) {
		cv::Point p1 = cv::Point(x, baseline);
		cv::Point p2 = cv::Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255));
	}

	return imgHist;
}

float compute_mean(int *histogram, int numberPixels)
{
	int i;
	float mean = 0;

	for (i = 0; i < 256; i++)
		mean += i * histogram[i];

	return mean / numberPixels;
}

float compute_standard_deviation(Mat image, float mean)
{
	int i, j;
	float sum = 0;

	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			sum += pow(image.at<uchar>(i, j) - mean, 2);

	return sqrt(sum / (image.rows * image.cols));
}

int get_min_intensity(int *histogram)
{
	int i;

	for (i = 0; i < 256; i++)
		if (histogram[i] != 0)
			return i;

	return -1;
}

int get_max_intensity(int *histogram)
{
	int i;

	for (i = 255; i >= 0; i--)
		if (histogram[i] != 0)
			return i;

	return -1;
}

Mat automatic_thresholding(Mat src)
{
	int i;
	int histogram[256];
	int minIntensity, maxIntensity;
	float threshold, prevThreshold;
	float lowerMean, upperMean;
	int lowerCount, upperCount;
	float error = 0.1;
	Mat dst;

	dst = Mat(src.rows, src.cols, CV_8UC1);

	compute_histogram(src, histogram);

	minIntensity = get_min_intensity(histogram);
	maxIntensity = get_max_intensity(histogram);
	threshold = (minIntensity + maxIntensity) / 2;

	do {
		prevThreshold = threshold;
		lowerCount = 0;
		upperCount = 0;
		lowerMean = 0;
		upperMean = 0;

		for (i = minIntensity; i <= threshold; i++) {
			lowerCount += histogram[i];
			lowerMean += i * histogram[i];
		}
		lowerMean /= lowerCount;

		for (i = threshold + 1; i <= maxIntensity; i++) {
			upperCount += histogram[i];
			upperMean += i * histogram[i];
		}
		upperMean /= upperCount;

		threshold = (lowerMean + upperMean) / 2;

	} while (abs(threshold - prevThreshold) > error);

	std::cout << "Threshold: " << threshold << std::endl;

	dst = grayscale_to_binary(src, (int) threshold);

	return dst;
}

Mat histogram_stretch_shrink(Mat image, int newMin, int newMax)
{
	Mat dst;
	int g;
	int i, j;
	int histogram[256];
	int newHistogram[256];

	dst = Mat(image.rows, image.cols, CV_8UC1);

	compute_histogram(image, histogram);

	int min = get_min_intensity(histogram);
	int max = get_max_intensity(histogram);

	for (g = 0; g < 256; g++) {
		newHistogram[g] = newMin + (histogram[g] - min) * (newMax - newMin) / (max - min);
	}

	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			dst.at<uchar>(i, j) = newHistogram[image.at<uchar>(i, j)];

	return dst;
}

Mat gamma_correction(Mat image, float gamma)
{
	int g;
	int histogram[256];
	int newHistogram[256];
	Mat dst;

	dst = Mat(image.rows, image.cols, CV_8UC1);

	compute_histogram(image, histogram);

	for (g = 0; g < 256; g++) {
		newHistogram[g] = 255 * pow((float) g / 255, gamma);
		if (newHistogram[g] > 255)
			newHistogram[g] = 255;
		else if (newHistogram[g] < 0)
			newHistogram[g] = 0;
	}

	for (int i = 0; i < image.rows; i++)
		for (int j = 0; j < image.cols; j++)
			dst.at<uchar>(i, j) = newHistogram[image.at<uchar>(i, j)];

	return dst;
}

Mat histogram_equalization(Mat image)
{
	int i, j;
	int histogram[256];
	float pdf[256];
	float cpdf[256];
	int newHistogram[256];
	Mat dst;

	dst = Mat(image.rows, image.cols, CV_8UC1);

	compute_histogram(image, histogram, pdf);

	cpdf[0] = pdf[0];

	for (i = 1; i < 256; i++) {
		cpdf[i] = cpdf[i - 1] + pdf[i];
	}

	for (i = 0; i < 256; i++) {
		newHistogram[i] = 255 * cpdf[i];
	}

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			dst.at<uchar>(i, j) = newHistogram[image.at<uchar>(i, j)];
		}
	}

	imshow("Inital Histogram", create_histogram_image("Initial Histogram", histogram, 256, 256));
	imshow("Final Histogram", create_histogram_image("Final Histogram", newHistogram, 256, 256));

	return dst;
}