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

int main(int argc, char** argv )
{
	int op;
	int ret = 0;
	String imagesPath;
	String imageName;
	String imagePath;
	char name[255];

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
		default:
			std::cout << "Unknown operation" << std::endl;
			break;
		}

		std::cout << std::endl;
	} while (op != 0);

	std::cout << "End" << std::endl;
	return 0;
}
