#include "morphology.hpp"

int check_neighb_struct(Mat src, Mat structuringElement, int row, int col)
{
	int i, j;
	int x, y;
	int auxI, auxJ;

	for (x = 0; x < 3; x++)
		for (y = 0; y < 3; y++)
		{
			auxI = row + x - 1;
			auxJ = col + y - 1;
			if (is_inside_image(src, auxI, auxJ))
				if (structuringElement.at<uchar>(x, y) && (src.at<uchar>(auxI, auxJ) == 255))
					return 0;
		}

	return 1;
}

Mat dilate(Mat src, Mat structuringElement)
{
	int i, j;
	int x, y;
	int auxI, auxJ;
	Mat dst;

	dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));

	for (i = 0; i < src.rows; i++)
		for (j = 0; j < src.cols; j++)
			if (src.at<uchar>(i, j) == 0)
				for (x = 0; x < 3; x++)
					for (y = 0; y < 3; y++)
					{
						auxI = i + x - 1;
						auxJ = j + y - 1;
						if (is_inside_image(src, auxI, auxJ))
							if (structuringElement.at<uchar>(x, y))
								dst.at<uchar>(auxI, auxJ) = 0;
					}

	return dst;
}

Mat erode(Mat src, Mat structuringElement)
{
	int i, j;
	int x, y;
	int auxI, auxJ;
	Mat dst;

	dst = src.clone();

	for (i = 0; i < src.rows; i++)
		for (j = 0; j < src.cols; j++)
			if (src.at<uchar>(i, j) == 0)
				if (!check_neighb_struct(src, structuringElement, i, j))
					dst.at<uchar>(i, j) = 255;

	return dst;
}

Mat open(Mat src, Mat structuringElement)
{
	Mat dst = src.clone();

	dst = erode(dst, structuringElement);
	dst = dilate(dst, structuringElement);

	return dst;
}

Mat close(Mat src, Mat structuringElement)
{
	Mat dst = src.clone();

	dst = dilate(dst, structuringElement);
	dst = erode(dst, structuringElement);

	return dst;
}

Mat diff(Mat src1, Mat src2)
{
	int i, j;
	Mat dst = src1.clone();

	for (i = 0; i < src1.rows; i++)
		for (j = 0; j < src2.cols; j++)
			if (src2.at<uchar>(i, j) == 0)
				dst.at<uchar>(i, j) = 255;

	return dst;
}

Mat intersect(Mat src1, Mat src2)
{
	int i, j;
	Mat dst = Mat(src1.rows, src1.cols, CV_8UC1, Scalar(255));

	for (i = 0; i < src1.rows; i++)
		for (j = 0; j < src1.cols; j++)
			if (src1.at<uchar>(i, j) == 0 && src2.at<uchar>(i, j) == 0)
				dst.at<uchar>(i, j) = 0;

	return dst;
}

Mat unify(Mat src1, Mat src2)
{
	int i, j;
	Mat dst = Mat(src1.rows, src1.cols, CV_8UC1, Scalar(255));

	for (i = 0; i < src1.rows; i++)
		for (j = 0; j < src1.cols; j++)
			if (src1.at<uchar>(i, j) == 0 || src2.at<uchar>(i, j) == 0)
				dst.at<uchar>(i, j) = 0;

	return dst;
}

Mat extract_boundary(Mat src)
{
	Mat boundaryStructuringElement;

	boundaryStructuringElement = Mat(3, 3, CV_8UC1, Scalar(1));

	return diff(src, erode(src, boundaryStructuringElement));
}

Mat fill_region(Mat src, int startRow, int startCol)
{
	Mat dst, aux;
	Mat neg;
	Mat fillStructuringElement;

	fillStructuringElement = Mat(3, 3, CV_8UC1, Scalar(1));
	fillStructuringElement.at<uchar>(0, 0) = 0;
	fillStructuringElement.at<uchar>(2, 0) = 0;
	fillStructuringElement.at<uchar>(0, 2) = 0;
	fillStructuringElement.at<uchar>(2, 2) = 0;

	dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
	dst.at<uchar>(startRow, startCol) = 0;
	neg = negate(src);

	do {
		aux = dst.clone();
		dst = dilate(dst, fillStructuringElement);
		dst = intersect(dst, neg);
	} while (countNonZero(diff(dst, aux)) != aux.rows * aux.cols);

	return unify(src, dst);
}