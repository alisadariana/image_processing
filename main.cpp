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

int main(int argc, char** argv )
{
	int op;
	int ret = 0;
	String imagesPath;
	String imageName;
	String imagePath;
	char name[255];
	int val;
	float floatVal;

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
		default:
			std::cout << "Unknown operation" << std::endl;
			break;
		}

		std::cout << std::endl;
	} while (op != 0);

	std::cout << "End" << std::endl;
	return 0;
}
