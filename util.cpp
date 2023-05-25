#include "util.hpp"

Mat negate(Mat src)
{
	int i, j;
	int aux;
	Mat dst;

	dst = Mat(src.rows, src.cols, CV_8UC1);

	for (i = 0; i < src.rows; i++)
	{
		for (j = 0; j < src.cols; j++)
		{
			aux = 255 - src.at<uchar>(i, j);
			if (aux < 0)
			{
				dst.at<uchar>(i, j) = 0;
			}
			else if (aux > 255)
			{
				dst.at<uchar>(i, j) = 255;
			}
			else
			{
				dst.at<uchar>(i, j) = aux;
			}
		}
	}

	return dst;
}