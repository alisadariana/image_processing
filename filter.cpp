#include "filter.hpp"

Mat filter_median(Mat src, int kernelSize)
{
	Mat dst;
	int i, j;
	int u, v;
	int padding = (kernelSize - 1) / 2;
	std::vector<int> window;

	dst = Mat(src.rows, src.cols, CV_8UC1);

	for (i = padding; i < src.rows - padding; i++) {
		for (j = padding; j < src.cols - padding; j++) {
			for (u = 0; u < kernelSize; u++) {
				for (v = 0; v < kernelSize; v++) {
					window.push_back(src.at<uchar>(i + u - padding, j + v - padding));
				}
			}
			std::sort(window.begin(), window.end());
			dst.at<uchar>(i, j) = window[window.size() / 2];
			window.clear();
		}
	}

	return dst;
}

Mat filter_gaussian_2d(Mat src, int kernelSize)
{
	Mat dst;
	Mat kernel;
	double sigma = kernelSize / 6.0;
	double sum;
	int i, j;
	int u, v;
	int padding = (kernelSize - 1) / 2;

	dst = Mat(src.rows, src.cols, CV_8UC1);
	kernel = Mat::zeros(kernelSize, kernelSize, CV_64F);

	for (u = 0; u < kernelSize; u++)
	{
		for (v = 0; v < kernelSize; v++)
		{
			kernel.at<double>(u, v) = exp(-((u - padding) * (u - padding) + (v - padding) * (v - padding)) / (2 * sigma * sigma)) / (2 * CV_PI * sigma * sigma);
		}
	}

	for (i = padding; i < src.rows - padding; i++)
	{
		for (j = padding; j < src.cols - padding; j++)
		{
			sum = 0;
			for (u = 0; u < kernelSize; u++)
			{
				for (v = 0; v < kernelSize; v++)
				{
					sum += kernel.at<double>(u, v) * src.at<uchar>(i + u - padding, j + v - padding);
				}
			}
			dst.at<uchar>(i, j) = sum;
		}
	}

	return dst;
}

Mat filter_gaussian_vec(Mat src, int kernelSize)
{
	Mat dst;
	std::vector<double> Gx, Gy;
	double sigma = kernelSize / 6.0;
	double sum;
	int i, j, k;
	int padding = (kernelSize - 1) / 2;

	dst = Mat(src.rows, src.cols, CV_8UC1);

	for (k = 0; k < kernelSize; k++)
	{
		Gx.push_back(exp(-(k - padding) * (k - padding) / (2 * sigma * sigma)) / (sqrt(2 * CV_PI) * sigma));
		Gy.push_back(exp(-(k - padding) * (k - padding) / (2 * sigma * sigma)) / (sqrt(2 * CV_PI) * sigma));

	}

	for (i = padding; i < src.rows - padding; i++)
	{
		for (j = padding; j < src.cols - padding; j++)
		{
			sum = 0;
			for (k = 0; k < kernelSize; k++)
			{
				sum += Gx[k] * src.at<uchar>(i, j + k - padding);
				sum += Gy[k] * src.at<uchar>(i + k - padding, j);
			}
			dst.at<uchar>(i, j) = sum / 2;
		}
	}

	return dst;
}
