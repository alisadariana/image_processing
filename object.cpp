#include "object.hpp"

#define PROJ_WIDTH 10

int object_area(cv::Mat image, cv::Vec3b objectLabel)
{
    int area = 0;
    int width, height;
    width = image.cols;
    height = image.rows;
    int label_b = objectLabel[0];
    int label_g = objectLabel[1];
    int label_r = objectLabel[2];
    int r, g, b;
    cv::Vec3b pixel;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            pixel = image.at<cv::Vec3b>(i, j);
            b = pixel[0];
            g = pixel[1];
            r = pixel[2];
            if (r == label_r && g == label_g && b == label_b)
                area++;
        }
    }
    return area;
}

void object_center_of_mass(cv::Mat image, cv::Vec3b objectLabel, int *center_row, int *center_col,
			   int object_area, void *out_mat)
{
	int width, height;
	width = image.cols;
	height = image.rows;
	int row = 0;
	int col = 0;
	float rf, cf;
	Mat out;

	if (out_mat)
		out = *(Mat *)out_mat;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (image.at<cv::Vec3b>(i, j) == objectLabel)
			{
				row += i;
				col += j;
			}
		}
	}

	rf = (float)row / object_area;
	cf = (float)col / object_area;
	*center_row = (int)rf;
	*center_col = (int)cf;

	if (out_mat)
		out.at<Vec3b>(*center_row, *center_col) = Vec3b(0, 0, 0);
	}

float object_axis_of_elongation(cv::Mat image, int center_row, int center_col,
				cv::Vec3b objectLabel)
{
    int width, height;
    width = image.cols;
    height = image.rows;
    int delta_r, delta_c;
    int prod = 0;
    int square_c = 0;
    int square_r = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image.at<cv::Vec3b>(i, j) == objectLabel)
            {
                delta_r = i - center_row;
                delta_c = j - center_col;
                prod += delta_c * delta_r;
                square_r += delta_r * delta_r;
                square_c += delta_c * delta_c;
            }
        }
    }
    return atan2(2 * prod, square_c - square_r);
}

int object_perimeter(cv::Mat image, cv::Vec3b objectLabel, void *out_mat)
{
    int width, height;
    width = image.cols;
    height = image.rows;
    int perim = 0;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    Vec3b pixel;
    int r, c;
    int i, j;
    bool isContour;
    Vec3b blackPixel;
    Mat out;

    if (out_mat) {
	blackPixel = Vec3b(0, 0, 0);
	out = *((Mat *)out_mat);
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            if (image.at<Vec3b>(i, j) == objectLabel)
            {
                isContour = false;
                for (int l = 0; l < 8; l++) {
                    r = i + dx[l];
                    c = j + dy[l];
                    if (r >= 0 && r < height && c >= 0 && c < width) {
                        pixel = image.at<cv::Vec3b>(r, c);
                        if (pixel != objectLabel)
                            isContour = true;
                    }
                }
                if (isContour) {
                    perim++;
		    if (out_mat)
		    	out.at<Vec3b>(i, j) = blackPixel;
                }
            }
        }
    }
    return perim;
}

float object_thinness_ratio(int object_area, int object_perimeter)
{
    return 4 * 3.14 * object_area / object_perimeter / object_perimeter;
}

float object_aspect_ratio(Mat image, Vec3b object_label)
{
	int i, j;
	int cmin = image.cols, rmin = image.rows;
	int cmax = 0, rmax = 0;

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			if (image.at<Vec3b>(i, j) == object_label) {
				if (i > rmax)
					rmax = i;
				if (i < rmin)
					rmin = i;
				if (j > cmax)
					cmax = j;
				if (j < cmin)
					cmin = j;
			}
		}
	}

	return (float)(cmax - cmin + 1) / (rmax - rmin + 1);
}

void object_draw_projections(Mat image, Vec3b object_label, Mat out)
{
	int i, j, k;
	int proj_row, proj_col;
	int row_sum[image.rows], col_sum[image.cols];
	float row_gradient[image.rows], col_gradient[image.cols];
	Vec3b color1(0, 0, 255);
	Vec3b color2(255, 0, 0);
	Vec3b intensity;

	memset(row_sum, 0, sizeof(row_sum));
	memset(col_sum, 0, sizeof(col_sum));

	proj_row = out.rows - PROJ_WIDTH;
	proj_col = out.cols - PROJ_WIDTH;

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			if (image.at<Vec3b>(i, j) == object_label) {
				row_sum[i]++;
				col_sum[j]++;
			}
		}
	}

	for (i = 0; i < image.rows; i++)
		row_gradient[i] = 10 * (float) row_sum[i] / (image.cols - 1);

	for (j = 0; j < image.cols; j++)
		col_gradient[j] = 10 * (float) col_sum[j] / (image.rows - 1);
	
	for (i = 0; i < image.rows; i++) {
		if (row_gradient[i] > 0) {
			intensity = color1 + row_gradient[i] * (color2 - color1);
			for (k = 0; k < PROJ_WIDTH; k++)
				out.at<Vec3b>(i, proj_col + k) = intensity;
		}
	}

	for (j = 0; j < image.cols; j++) {
		if (col_gradient[j] > 0) {
			intensity = color1 + col_gradient[j] * (color2 - color1);
			for (k = 0; k < PROJ_WIDTH; k++)
				out.at<Vec3b>(proj_row + k, j) = intensity;
		}
	}
}

void object_features_callback(int event, int col, int row, int flags, void* data)
{
	if (event == EVENT_LBUTTONDOWN) {
		int area;
		int perimeter;
		int center_row, center_col;
		float axis_of_elongation;
		float thinness_ratio;
		float aspect_ratio;
		std::cout << "Click position (" << row << ", " << col << ")" << std::endl;
		Mat image = *((Mat*)data);
		Vec3b pixel = image.at<Vec3b>(row, col);
		area = object_area(image, pixel);
		perimeter = object_perimeter(image, pixel, NULL);
		object_center_of_mass(image, pixel, &center_row, &center_col, area, NULL);
		axis_of_elongation = object_axis_of_elongation(image, center_row, center_col, pixel);
		thinness_ratio = object_thinness_ratio(area, perimeter);
		aspect_ratio = object_aspect_ratio(image, pixel);
		std::cout << "Pixel = " << pixel << std::endl;
		std::cout << "Area = " << area << std::endl;
		std::cout << "Perimeter = " << perimeter << std::endl;
		std::cout << "Center = " << '(' << center_row << ", " << center_col << ')' << std::endl;
		std::cout << "Axis of elongation = " << axis_of_elongation << std::endl;
		std::cout << "Thinness ratio = " << thinness_ratio << std::endl;
		std::cout << "Aspect ratio = " << aspect_ratio << std::endl;
		std::cout << std::endl;
	}
}

void object_features_display_callback(int event, int col, int row, int flags, void *data)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		int area;
		int perimeter;
		int center_row, center_col;
		int i, j;
		int v_line, h_line = 50;
		float axis_of_elongation;
		float thinness_ratio;
		float aspect_ratio;
		Mat out;
		Vec3b whitePixel = Vec3b(255, 255, 255);

		std::cout << "Click position: (" << row << ", " << col << ")" << std::endl;
		Mat image = *((Mat *)data);
		Vec3b pixel = image.at<Vec3b>(row, col);
	
		out = Mat(image.rows + PROJ_WIDTH, image.cols + PROJ_WIDTH, CV_8UC3);
		for (i = 0; i < out.rows; i++) {
			for (j = 0; j < out.cols; j++) {
				out.at<Vec3b>(i, j) = whitePixel;
			}
		}

		area = object_area(image, pixel);
		perimeter = object_perimeter(image, pixel, &out);
		object_center_of_mass(image, pixel, &center_row, &center_col, area, &out);
		axis_of_elongation = object_axis_of_elongation(image, center_row, center_col, pixel);

		line(out, Point(center_col, center_row), Point(center_col - h_line, center_row), Scalar(0, 0, 0));
		// phi = axis of elongation
		// tan phi = vertical edge / horizontal edge
		v_line = h_line * tan(axis_of_elongation);

		line(out, Point(center_col, center_row), Point(center_col, center_row - v_line), Scalar(0, 0, 0));
		line(out, Point(center_col - h_line, center_row), Point(center_col, center_row - v_line), Scalar(0, 0, 255));

		object_draw_projections(image, pixel, out);

		Mat out_resized;
		resize(out, out_resized, Size(out.cols * 4, out.rows * 4), INTER_LINEAR);
		imshow("Out", out_resized);
		waitKey(0);
		destroyAllWindows();
	}
}

int object_features(String imagePath, bool display = false)
{
	Mat image;

	image = imread(imagePath, IMREAD_COLOR);

	if (image.empty()) {
		perror("object_features: image is empty\n");
		return -1;
	}

	namedWindow("Image", 1);
	if (display)
		setMouseCallback("Image", object_features_display_callback, &image);
	else
		setMouseCallback("Image", object_features_callback, &image);

	imshow("Image", image);
	waitKey(0);
	destroyAllWindows();

	return 0;
}

int filter_objects_by_area_and_orientation(String imagePath, int maxArea,
				       float minPhi, float maxPhi)
{
	Mat image;

	image = imread(imagePath, IMREAD_COLOR);

	if (image.empty())
	{
		perror("filter_objects_by_area_and_orientation: image is empty\n");
		return -1;
	}

	// TODO

	return 0;
}