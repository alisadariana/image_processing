#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

int display_image(String imagePath)
{
	Mat image;
	image = imread(imagePath, IMREAD_COLOR);
	if ( !image.data ) {
		printf("No image data \n");
		return -1;
	}
	namedWindow("Display Image", WINDOW_AUTOSIZE);
	imshow("Display Image", image);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

int negative_image(String imagePath)
{
	int i, j;
	int aux;
	Mat image;
	Mat outImage;

	image = imread(imagePath, IMREAD_GRAYSCALE);
	if (!image.data) {
		printf("negative_image: No image data\n");
		return -1;
	}

	outImage = Mat(image.rows, image.cols, CV_8UC1);

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			aux = 255 - image.at<uchar>(i, j);
			if (aux < 0) {
				outImage.at<uchar>(i, j) = 0;
			} else if (aux > 255) {
				outImage.at<uchar>(i, j) = 255;
			} else {
				outImage.at<uchar>(i, j) = aux;
			}
		}
	}
	imshow("Image", image);
	imshow("Negative Image", outImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

int add_value_to_grayscale_image(String imagePath, int value)
{
	int i, j;
	int aux;
	Mat image;
	Mat outImage;

	image = imread(imagePath, IMREAD_GRAYSCALE);
	if (!image.data) {
		printf("add_value_to_grayscale_image: No image data\n");
		return -1;
	}

	outImage = Mat(image.rows, image.cols, CV_8UC1);

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			aux = image.at<uchar>(i, j) + value;
			if (aux < 0) {
				outImage.at<uchar>(i, j) = 0;
			} else if (aux > 255) {
				outImage.at<uchar>(i, j) = 255;
			} else {
				outImage.at<uchar>(i, j) = aux;
			}
		}
	}
	imshow("Image", image);
	imshow("Final Image", outImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

int multiply_by_value_grayscale_image(String imagePath, float value)
{
	int i, j;
	float aux;
	Mat image;
	Mat outImage;

	image = imread(imagePath, IMREAD_GRAYSCALE);
	if (!image.data) {
		printf("multiply_by_value_grayscale_image: No image data\n");
		return -1;
	}

	outImage = Mat(image.rows, image.cols, CV_8UC1);

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			aux = (float)image.at<uchar>(i, j) * value;
			if (aux < 0) {
				outImage.at<uchar>(i, j) = 0;
			} else if (aux > 255) {
				outImage.at<uchar>(i, j) = 255;
			} else {
				outImage.at<uchar>(i, j) = (int)aux;
			}
		}
	}
	imshow("Image", image);
	imshow("Final Image", outImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

void create_four_section_image()
{
	Mat image;
	int dim = 256;
	int i, j;
	Vec3b whitePixel, redPixel, greenPixel, yellowPixel;

	whitePixel[0] = 255;
	whitePixel[1] = 255;
	whitePixel[2] = 255;

	redPixel[0] = 0;
	redPixel[1] = 0;
	redPixel[2] = 255;

	greenPixel[0] = 0;
	greenPixel[1] = 255;
	greenPixel[2] = 0;

	yellowPixel[0] = 0;
	yellowPixel[1] = 255;
	yellowPixel[2] = 255;

	image = Mat(dim, dim, CV_8UC3);

	for (i = 0; i < dim/2; i++)
		for (j = 0; j < dim/2; j++)
			image.at<Vec3b>(i, j) = whitePixel;

	for (i = dim/2; i < dim; i++)
		for (j = 0; j < dim/2; j++)
			image.at<Vec3b>(i, j) = redPixel;

	for (i = 0; i < dim/2; i++)
		for (j = dim/2; j < dim; j++)
			image.at<Vec3b>(i, j) = greenPixel;

	for (i = dim/2; i < dim; i++)
		for (j = dim/2; j < dim; j++)
			image.at<Vec3b>(i, j) = yellowPixel;

	imshow("Image", image);
	waitKey(0);
	destroyAllWindows();
}

int extract_rgb_channels(String imagePath)
{
	int i, j;
	float aux;
	Mat image;
	Mat redImage, greenImage, blueImage;
	Vec3b pixel;

	image = imread(imagePath, IMREAD_COLOR);
	if (!image.data) {
		printf("extract_rgb_channels: No image data\n");
		return -1;
	}

	redImage = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);
	greenImage = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);
	blueImage = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			pixel = image.at<Vec3b>(i, j);
			redImage.at<uchar>(i, j) = pixel[2];
			greenImage.at<uchar>(i, j) = pixel[1];
			blueImage.at<uchar>(i, j) = pixel[0];
		}
	}

	imshow("Image", image);
	imshow("Red Image", redImage);
	imshow("Green Image", greenImage);
	imshow("Blue Image", blueImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

int rgb_to_grayscale(String imagePath)
{
	int i, j;
	float aux;
	Mat image;
	Mat grayscaleImage;
	Vec3b pixel;

	image = imread(imagePath, IMREAD_COLOR);
	if (!image.data) {
		printf("rgb_to_grayscale: No image data\n");
		return -1;
	}

	grayscaleImage = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			pixel = image.at<Vec3b>(i, j);
			grayscaleImage.at<uchar>(i, j) = (pixel[0] + pixel[1] + pixel[2]) / 3;
		}
	}

	imshow("Image", image);
	imshow("Grayscale Image", grayscaleImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

int grayscale_to_binary(String imagePath, int treshold)
{
	int i, j;
	float aux;
	Mat image;
	Mat binaryImage;
	Vec3b pixel;

	image = imread(imagePath, IMREAD_GRAYSCALE);
	if (!image.data) {
		printf("grayscale_to_binary: No image data\n");
		return -1;
	}

	binaryImage = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) >= treshold)
				binaryImage.at<uchar>(i, j) = 255;
			else
				binaryImage.at<uchar>(i, j) = 0;
		}
	}

	imshow("Image", image);
	imshow("Binary Image", binaryImage);
	waitKey(0);
	destroyAllWindows();
	return 0;
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

int display_rgb_to_hsv(String imagePath)
{
	Mat image;
	Mat hue, saturation, value;

	image = imread(imagePath, IMREAD_COLOR);
	if (!image.data) {
		printf("display_rgb_to_hsv: No image data\n");
		return -1;
	}

	hue = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);
	saturation = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);
	value = Mat(image.rows, image.cols, IMREAD_GRAYSCALE);

	rgb_to_hsv(image, hue, saturation, value);

	imshow("Image", image);
	imshow("Hue", hue);
	imshow("Saturation", saturation);
	imshow("Value", value);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

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



void show_histogram(const String &name, int *hist, const int height,
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

	imshow(name, imgHist);
	waitKey(0);
	destroyAllWindows();
}

int display_histogram(String imagePath)
{
	Mat image;
	int ret;
	int histogram[256];

	image = imread(imagePath, IMREAD_COLOR);
	if (!image.data) {
		printf("display_histogram: No image data\n");
		return -1;
	}

	ret = compute_histogram(image, histogram);
	if (ret) {
		printf("display_histogram: Error in compute_histogram\n");
		return -1;
	}

	show_histogram("Histogram", histogram, 256, 256);
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

int print_histogram_bins(String imagePath, int numberBins)
{
	Mat image;
	int histogram[numberBins];
	int i;

	image = imread(imagePath, IMREAD_GRAYSCALE);
	if (!image.data) {
		printf("print_histogram_bins: No image data\n");
		return -1;
	}

	compute_histogram_bins(image, histogram, numberBins);

	for (i = 0; i < numberBins; i++) {
		std::cout << histogram[i] << std::endl;
	}

	return 0;
}

int get_multilevel_tresholding_max(Mat image, int *numberMax, int *max,
				   int *valueMap, int windowHeight = 5,
				   float treshold = 0.0003)
{
	int ret;
	int histogram[256];
	float pdf[256];
	int i, j, k;
	float average;
	bool check;
	int count = 1;
	int maxValues[256];

	ret = compute_histogram(image, histogram, pdf);
	if (ret) {
		printf("get_multilevel_tresholding_max: Error in compute_histogram\n");
		return -1;
	}

	max[0] = 0;

	for (k = windowHeight; k < 256 - windowHeight; k++) {
		average = 0.0f;
		check = true;
		for (i = k - windowHeight; i < k + windowHeight; i++) {
			average += pdf[i];
			if (pdf[k] < pdf[i])
				check = false;
		}
		average = average / (2 * windowHeight + 1);
		if ((pdf[k] > average + treshold) && check) {
			max[count] = k;
			maxValues[count - 1] = (max[count - 1] + max[count]) / 2;
			count++;
		}
	}

	max[count] = 255;
	maxValues[count - 1] = (max[count - 1] + max[count]) / 2;
	maxValues[count] = 255;

	count++;
	*numberMax = count;

	k = 0;
	for (i = 0; i < count; i++) {
		for (j = k; j < maxValues[i]; j++) {
			valueMap[j] = max[i];
		}
		k = maxValues[i];
	}

	return 0;
}

int display_multilevel_tresholding(String imagePath, bool floydSteinbergDitheringOn = false)
{
	Mat image;
	Mat outImage;
	int ret;
	int numberMax;
	int max[256];
	int valueMap[256];
	int i, j;
	int oldPixel, newPixel;
	int error;

	image = imread(imagePath, IMREAD_GRAYSCALE);
	if (!image.data) {
		printf("display_multilevel_tresholding: No image data\n");
		return -1;
	}

	ret = get_multilevel_tresholding_max(image, &numberMax, max, valueMap);
	if (ret) {
		printf("display_multilevel_tresholding: Error in get_multilevel_tresholding_max\n");
		return -1;
	}

	outImage = Mat(image.rows, image.cols, CV_8UC1);

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			outImage.at<uchar>(i, j) = image.at<uchar>(i, j);
		}
	}

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			oldPixel = outImage.at<uchar>(i, j);
			newPixel = valueMap[oldPixel];
			outImage.at<uchar>(i, j) = newPixel;
			if (floydSteinbergDitheringOn) {
				error = oldPixel - newPixel;
				if (i + 1 < image.rows)
					outImage.at<uchar>(i + 1, j) = outImage.at<uchar>(i + 1, j) + 7 * error / 16;
				if (i - 1 >= 0 && j + 1 < image.cols)
					outImage.at<uchar>(i - 1, j + 1) = outImage.at<uchar>(i - 1, j + 1) + 3 * error / 16;
				if (j + 1 < image.cols)
					outImage.at<uchar>(i, j + 1) = outImage.at<uchar>(i, j + 1) + 5 * error / 16;
				if (i + 1 < image.rows && j + 1 < image.cols)
					outImage.at<uchar>(i + 1, j + 1) = outImage.at<uchar>(i + 1, j + 1) + error / 16;
			}

		}
	}

	imshow("Multilevel Threshold", outImage);
	imshow("Image", image);
	waitKey(0);
	destroyAllWindows();
	return 0;
}

int main(int argc, char **argv)
{
	int op;
	int ret = 0;
	String imagesPath;
	String imageName;
	String imagePath;
	char name[255];
	int val;
	float floatVal;
	char c;
	bool option;

	imagesPath = "src/images/";
	imageName = "lena.jpg";
	imagePath = imagesPath + imageName;

	do {
		op = 0;
		std::cout << "Operation = ";
		scanf("%d", &op);
		std::cout << "Selected operation " << op << std::endl;

		switch (op)
		{
		case 1:
			std::cout << "Image name = ";
			scanf("%s", name);
			std::cout << "Selected image name = " << name << std::endl;
			imageName = name;
			imagePath = imagesPath + imageName;
			std::cout << "Image path = " << imagePath << std::endl;
			break;
		case 2:
			std::cout << "Displaying image " << imageName << std::endl;
			ret = display_image(imagePath);
			if (ret)
				return -1;
			break;
		case 3:
			std::cout << "Negative image " << imageName << std::endl;
			ret = negative_image(imagePath);
			if (ret)
				return -1;
			break;
		case 4:
			std::cout << "Add value to the gray levels of image " << imageName << std::endl;
			std::cout << "Value = ";
			scanf("%d", &val);
			ret = add_value_to_grayscale_image(imagePath, val);
			if (ret)
				return -1;
			break;
		case 5:
			std::cout << "Multiply by value the gray levels of image " << imageName << std::endl;
			std::cout << "Value = ";
			scanf("%f", &floatVal);
			ret = multiply_by_value_grayscale_image(imagePath, floatVal);
			if (ret)
				return -1;
			break;
		case 6:
			std::cout << "Create image with four equal colored sections" << std::endl;
			create_four_section_image();
			break;
		case 7:
			std::cout << "Extract RGB channels from image " << imageName << std::endl;
			ret = extract_rgb_channels(imagePath);
			if (ret)
				return -1;
			break;
		case 8:
			std::cout << "RBG to grayscale " << imageName << std::endl;
			ret = rgb_to_grayscale(imagePath);
			if (ret)
				return -1;
			break;
		case 9:
			std::cout << "Grayscale to binary " << imageName << std::endl;
			std::cout << "Treshold = ";
			scanf("%d", &val);
			ret = grayscale_to_binary(imagePath, val);
			if (ret)
				return -1;
			break;
		case 10:
			std::cout << "RGB to HSV " << imageName << std::endl;
			ret = display_rgb_to_hsv(imagePath);
			if (ret)
				return -1;
			break;
		case 11:
			std::cout << "Histogram of " << imageName << std::endl;
			ret = display_histogram(imagePath);
			if (ret)
				return -1;
			break;
		case 12:
			std::cout << "Print histogram divided in bins " << imageName << std::endl;
			std::cout << "Number of bins = ";
			scanf("%d", &val);
			ret = print_histogram_bins(imagePath, val);
			if (ret)
				return -1;
			break;
		case 13:
			std::cout << "Multilevel tresholding " << imageName << std::endl;
			std::cout << "Use Floyd-Steinberg dithering? (y/n) ";
			scanf("\n%c", &c);
			printf("Chosen %c\n", c);
			option = false;
			if (c == 'y') {
				option = true;
			}
			ret = display_multilevel_tresholding(imagePath, option);
			if (ret)
				return -1;
			break;
		default:
			std::cout << "Unknown operation" << std::endl;
			break;
		}

		std::cout << std::endl;
	} while (op != 0);

	std::cout << "End" << std::endl;
	return 0;
}
