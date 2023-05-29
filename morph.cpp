#include "morph.hpp"
#include <random>
#include <iostream>
#include <fstream>
#include <json/json.h>

#define IMAGE_PATH_1 "src/images/morphing/image1.jpg"
#define IMAGE_PATH_2 "src/images/morphing/image2.jpg"
#define LINES_PATH "src/images/morphing/lines.json"
#define FRAMES_PATH "src/images/morphing/frames/"

enum State
{
	IDLE,
	DRAWING_IMAGE1,
	DRAWING_IMAGE2,
	DRAWING_IMAGE1_FINISH,
	DRAWING_IMAGE2_FINISH,
	WAITING_FOR_IMAGE1,
	WAITING_FOR_IMAGE2,
};

typedef struct correspondence
{
	Point startImage1;
	Point endImage1;
	Point startImage2;
	Point endImage2;
	Vec3b color;
} Correspondence;

typedef struct lines
{
	State state;
	std::vector<Correspondence> *correspondences;
	Mat *image1;
	Mat *image2;
} Lines;

Vec3b generate_random_color()
{
	Vec3b color;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 255);
	color[0] = dis(gen);
	color[1] = dis(gen);
	color[2] = dis(gen);
	return color;
}

void draw_last_correspondence(Lines *lines)
{
	if (lines->correspondences->size() > 0)
	{
		Correspondence last = lines->correspondences->back();
		line(*lines->image1, last.startImage1, last.endImage1, last.color, 4, 8, 0);
		line(*lines->image2, last.startImage2, last.endImage2, last.color, 4, 8, 0);
		imshow("image1", *lines->image1);
		imshow("image2", *lines->image2);
	}
}

void draw_temp_line(Mat *image, Point start, Point end, String imageName)
{
	Mat temp = image->clone();
	line(temp, start, end, Scalar(0, 0, 255), 4, 8, 0);
	imshow(imageName, temp);
}

void callback_first_image(int event, int x, int y, int flags, void *userdata)
{
	Lines *lines = (Lines *)userdata;

	if (event == EVENT_LBUTTONDOWN)
	{
		if (lines->state == IDLE)
		{
			lines->state = DRAWING_IMAGE1;
			lines->correspondences->push_back(Correspondence());
			lines->correspondences->back().startImage1 = Point(x, y);
		}
		else if (lines->state == DRAWING_IMAGE1)
		{
			lines->state = WAITING_FOR_IMAGE2;
			lines->correspondences->back().endImage1 = Point(x, y);
			std::cout << "startImage1: " << lines->correspondences->back().startImage1 << std::endl;
			std::cout << "endImage1: " << lines->correspondences->back().endImage1 << std::endl;
		}
		else if (lines->state == WAITING_FOR_IMAGE1)
		{
			lines->state = DRAWING_IMAGE1_FINISH;
			lines->correspondences->back().startImage1 = Point(x, y);
		}
		else if (lines->state == DRAWING_IMAGE1_FINISH)
		{
			lines->state = IDLE;
			lines->correspondences->back().endImage1 = Point(x, y);
			lines->correspondences->back().color = generate_random_color();
			draw_last_correspondence(lines);
			std::cout << "startImage1: " << lines->correspondences->back().startImage1 << std::endl;
			std::cout << "endImage1: " << lines->correspondences->back().endImage1 << std::endl;
		}
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
		std::cout << "Correspondences " << lines->correspondences->size() << std::endl;
	}
	else
	{
		if (lines->state == DRAWING_IMAGE1 || lines->state == DRAWING_IMAGE1_FINISH)
		{
			draw_temp_line(lines->image1, lines->correspondences->back().startImage1, Point(x, y), "image1");
		}
	}
	return;
}

void callback_second_image(int event, int x, int y, int flags, void *userdata)
{
	Lines *lines = (Lines *)userdata;

	if (event == EVENT_LBUTTONDOWN)
	{
		if (lines->state == IDLE)
		{
			lines->state = DRAWING_IMAGE2;
			lines->correspondences->push_back(Correspondence());
			lines->correspondences->back().startImage2 = Point(x, y);
		}
		else if (lines->state == DRAWING_IMAGE2)
		{
			lines->state = WAITING_FOR_IMAGE1;
			lines->correspondences->back().endImage2 = Point(x, y);
			std::cout << "startImage2: " << lines->correspondences->back().startImage2 << std::endl;
			std::cout << "endImage2: " << lines->correspondences->back().endImage2 << std::endl;
		}
		else if (lines->state == WAITING_FOR_IMAGE2)
		{
			lines->state = DRAWING_IMAGE2_FINISH;
			lines->correspondences->back().startImage2 = Point(x, y);
		}
		else if (lines->state == DRAWING_IMAGE2_FINISH)
		{
			lines->state = IDLE;
			lines->correspondences->back().endImage2 = Point(x, y);
			lines->correspondences->back().color = generate_random_color();
			draw_last_correspondence(lines);
			std::cout << "startImage2: " << lines->correspondences->back().startImage2 << std::endl;
			std::cout << "endImage2: " << lines->correspondences->back().endImage2 << std::endl;
		}
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
		std::cout << "Correspondences " << lines->correspondences->size() << std::endl;
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		if (lines->state == DRAWING_IMAGE2 || lines->state == DRAWING_IMAGE2_FINISH)
		{
			draw_temp_line(lines->image2, lines->correspondences->back().startImage2, Point(x, y), "image2");
		}
	}
	return;
}

void saveLinesToFile(const Lines &lines, const std::string &filename)
{
	// Create a JSON object to hold the data
	Json::Value root;

	// Serialize the state
	root["state"] = static_cast<int>(lines.state);

	// Serialize correspondences
	for (const auto &correspondence : *(lines.correspondences))
	{
		Json::Value corr;
		corr["startImage1"]["x"] = correspondence.startImage1.x;
		corr["startImage1"]["y"] = correspondence.startImage1.y;
		corr["endImage1"]["x"] = correspondence.endImage1.x;
		corr["endImage1"]["y"] = correspondence.endImage1.y;
		corr["startImage2"]["x"] = correspondence.startImage2.x;
		corr["startImage2"]["y"] = correspondence.startImage2.y;
		corr["endImage2"]["x"] = correspondence.endImage2.x;
		corr["endImage2"]["y"] = correspondence.endImage2.y;
		corr["color"]["b"] = correspondence.color[0];
		corr["color"]["g"] = correspondence.color[1];
		corr["color"]["r"] = correspondence.color[2];

		root["correspondences"].append(corr);
	}

	// Create a JSON writer
	Json::StreamWriterBuilder builder;
	builder["indentation"] = "\t";
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	// Open the file for writing
	std::ofstream outputFile(filename);
	if (!outputFile.is_open())
	{
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}

	// Write the JSON data to the file
	writer->write(root, &outputFile);
	outputFile.close();

	std::cout << "Saved to file: " << filename << std::endl;
}

void readLinesFromFile(Lines &lines, const std::string &filename)
{
	// Open the JSON file for reading
	std::ifstream inputFile(filename);
	if (!inputFile.is_open())
	{
		std::cerr << "Failed to open file for reading: " << filename << std::endl;
		return;
	}

	// Parse the JSON data
	Json::Value root;
	Json::CharReaderBuilder readerBuilder;
	std::string parseErrors;
	bool parsingSuccessful = Json::parseFromStream(readerBuilder, inputFile, &root, &parseErrors);
	inputFile.close();

	if (!parsingSuccessful)
	{
		std::cerr << "Failed to parse JSON: " << parseErrors << std::endl;
		return;
	}

	// Deserialize the state
	lines.state = static_cast<State>(root["state"].asInt());

	// Deserialize correspondences
	const Json::Value &correspondences = root["correspondences"];
	for (const auto &corr : correspondences)
	{
		Correspondence correspondence;
		correspondence.startImage1.x = corr["startImage1"]["x"].asInt();
		correspondence.startImage1.y = corr["startImage1"]["y"].asInt();
		correspondence.endImage1.x = corr["endImage1"]["x"].asInt();
		correspondence.endImage1.y = corr["endImage1"]["y"].asInt();
		correspondence.startImage2.x = corr["startImage2"]["x"].asInt();
		correspondence.startImage2.y = corr["startImage2"]["y"].asInt();
		correspondence.endImage2.x = corr["endImage2"]["x"].asInt();
		correspondence.endImage2.y = corr["endImage2"]["y"].asInt();
		correspondence.color[0] = corr["color"]["b"].asInt();
		correspondence.color[1] = corr["color"]["g"].asInt();
		correspondence.color[2] = corr["color"]["r"].asInt();

		lines.correspondences->push_back(correspondence);
		draw_last_correspondence(&lines);
	}

	std::cout << "Loaded from file: " << filename << std::endl;
}

int store_correspondences()
{
	Mat img1, img2;
	Mat image1, image2;
	Lines lines;
	lines.correspondences = new std::vector<Correspondence>();

	img1 = imread(IMAGE_PATH_1, IMREAD_COLOR);
	if (img1.empty())
	{
		perror("image_morphing: img1 empty\n");
		return -1;
	}

	img2 = imread(IMAGE_PATH_2, IMREAD_COLOR);
	if (img2.empty())
	{
		perror("image_morphing: img2 empty\n");
		return -1;
	}

	resize(img1, img1, Size(1024, 1024), INTER_LINEAR);
	resize(img2, img2, Size(1024, 1024), INTER_LINEAR);

	image1 = img1.clone();
	image2 = img2.clone();

	lines.image1 = &image1;
	lines.image2 = &image2;
	lines.state = IDLE;

	namedWindow("image1", 1);
	setMouseCallback("image1", callback_first_image, &lines);

	namedWindow("image2", 1);
	setMouseCallback("image2", callback_second_image, &lines);

	imshow("image1", image1);
	imshow("image2", image2);
	waitKey(0);
	destroyAllWindows();

	saveLinesToFile(lines, LINES_PATH);
	delete (lines.correspondences);

	return 0;
}

int add_correspondences()
{
	Mat img1, img2;
	Mat image1, image2;
	Lines lines;
	lines.correspondences = new std::vector<Correspondence>();

	img1 = imread(IMAGE_PATH_1, IMREAD_COLOR);
	if (img1.empty())
	{
		perror("image_morphing: img1 empty\n");
		return -1;
	}

	img2 = imread(IMAGE_PATH_2, IMREAD_COLOR);
	if (img2.empty())
	{
		perror("image_morphing: img2 empty\n");
		return -1;
	}

	resize(img1, img1, Size(1024, 1024), 2, 2, INTER_LINEAR);
	resize(img2, img2, Size(1024, 1024), 2, 2, INTER_LINEAR);

	image1 = img1.clone();
	image2 = img2.clone();

	lines.image1 = &image1;
	lines.image2 = &image2;
	lines.state = IDLE;

	readLinesFromFile(lines, LINES_PATH);

	namedWindow("image1", 1);
	setMouseCallback("image1", callback_first_image, &lines);

	namedWindow("image2", 1);
	setMouseCallback("image2", callback_second_image, &lines);

	imshow("image1", image1);
	imshow("image2", image2);
	waitKey(0);
	destroyAllWindows();

	saveLinesToFile(lines, LINES_PATH);
	delete (lines.correspondences);

	return 0;
}

int display_correspondences()
{
	Mat img1, img2;
	Mat image1, image2;
	Lines lines;
	lines.correspondences = new std::vector<Correspondence>();

	img1 = imread(IMAGE_PATH_1, IMREAD_COLOR);
	if (img1.empty())
	{
		perror("image_morphing: img1 empty\n");
		return -1;
	}

	img2 = imread(IMAGE_PATH_2, IMREAD_COLOR);
	if (img2.empty())
	{
		perror("image_morphing: img2 empty\n");
		return -1;
	}

	resize(img1, img1, Size(1024, 1024), 2, 2, INTER_LINEAR);
	resize(img2, img2, Size(1024, 1024), 2, 2, INTER_LINEAR);

	image1 = img1.clone();
	image2 = img2.clone();

	lines.image1 = &image1;
	lines.image2 = &image2;
	lines.state = IDLE;

	readLinesFromFile(lines, LINES_PATH);

	namedWindow("image1", 1);
	setMouseCallback("image1", callback_first_image, &lines);

	namedWindow("image2", 1);
	setMouseCallback("image2", callback_second_image, &lines);

	imshow("image1", image1);
	imshow("image2", image2);
	waitKey(0);
	destroyAllWindows();

	delete (lines.correspondences);

	return 0;
}

double compute_distance(Point2f start, Point2f end)
{
	double distance = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.x, 2));
	return distance;
}

double compute_distance_to_segment(Point2f X, Point2f lineStart, Point2f lineEnd)
{
	double distance = cv::pointPolygonTest(cv::Vec<cv::Point2f, 2>(lineStart, lineEnd), X, true);
	return std::abs(distance);
}

double compute_v(const cv::Point2f &P, const cv::Point2f &Q, const cv::Point2f &X)
{
	cv::Point2f diffX_P = X - P;
	cv::Point2f diffQ_P = Q - P;

	cv::Point2f perpQ_P(-diffQ_P.y, diffQ_P.x);

	double lengthQ_P = cv::norm(diffQ_P);

	double dotProduct = diffX_P.dot(perpQ_P);

	double v = dotProduct / lengthQ_P;

	return v;
}

double compute_u(const cv::Point2f &P, const cv::Point2f &Q, const cv::Point2f &X)
{
	cv::Point2f diffX_P = X - P;
	cv::Point2f diffQ_P = Q - P;

	double lengthQ_P_squared = diffQ_P.dot(diffQ_P);

	double dotProduct = diffX_P.dot(diffQ_P);

	double u = dotProduct / lengthQ_P_squared;

	return u;
}

Point2f compute_x(const Point2f &P, const Point2f &Q, double u, double v)
{
	Point2f diffQ_P = Q - P;
	Point2f perpQ_P(-diffQ_P.y, diffQ_P.x);
	double lengthQ_P = cv::norm(diffQ_P);

	Point2f X = P + u * diffQ_P + (v * perpQ_P) / lengthQ_P;

	if (X.x < 0)
		X.x = 0;
	else if (X.x > 1023)
		X.x = 1023;
	if (X.y < 0)
		X.y = 0;
	else if (X.y > 1023)
		X.y = 1023;

	return X;
}

// t = 0 -> start
// t = 1 -> end
Point2f interpolate_points(Point start, Point end, double t)
{
	Point2f result;
	result.x = (1 - t) * start.x + t * end.x;
	result.y = (1 - t) * start.y + t * end.y;
	return result;
}

Vec3b interpolate_color(Vec3b start, Vec3b end, double t)
{
	Vec3b result;
	result[0] = (1 - t) * start[0] + t * end[0];
	result[1] = (1 - t) * start[1] + t * end[1];
	result[2] = (1 - t) * start[2] + t * end[2];
	return result;
}

double compute_weight(Point2f start, Point2f end, Point2f X)
{
	double length = compute_distance(start, end);
	double distanceToSegment = compute_distance_to_segment(X, start, end);
	double weight = length / (distanceToSegment + 0.1);
	return weight;
}

Mat morph_two_images(const Mat image1, const Mat image2, double t, Lines lines)
{
	int r, c;
	double u, v;
	Mat dst;

	Point2f start, end;
	Point2f X1, X2;
	Point2f displacement1, displacement2;
	Point2f displacementTotal1(0, 0);
	Point2f displacementTotal2(0, 0);
	Point2f pointImage1, pointImage2;
	double weight1, weight2;
	double weightTotal1, weightTotal2;

	dst = Mat(1024, 1024, CV_8UC3, Scalar(0, 0, 0));

	for (r = 0; r < 1024; r++)
	{
		for (c = 0; c < 1024; c++)
		{
			displacementTotal1 = Point2f(0, 0);
			weightTotal1 = 0;

			displacementTotal2 = Point2f(0, 0);
			weightTotal2 = 0;
			// for each intermediate line
			for (const Correspondence correspondence : *(lines.correspondences))
			{
				start = interpolate_points(correspondence.startImage1, correspondence.startImage2, t);
				end = interpolate_points(correspondence.endImage1, correspondence.endImage2, t);
				u = compute_u(start, end, Point(c, r));
				v = compute_v(start, end, Point(c, r));

				X1 = compute_x(correspondence.startImage1, correspondence.endImage1, u, v);
				displacement1 = X1 - Point2f(c, r);
				weight1 = compute_weight(start, end, Point(c, r));
				weightTotal1 += weight1;
				displacementTotal1 += weight1 * displacement1;

				X2 = compute_x(correspondence.startImage2, correspondence.endImage2, u, v);
				displacement2 = X2 - Point2f(c, r);
				weight2 = compute_weight(start, end, Point(c, r));
				weightTotal2 += weight2;
				displacementTotal2 += weight2 * displacement2;
			}
			displacementTotal1 /= weightTotal1;
			pointImage1 = Point2f(c, r) + displacementTotal1;

			displacementTotal2 /= weightTotal2;
			pointImage2 = Point2f(c, r) + displacementTotal2;

			dst.at<Vec3b>(r, c) = interpolate_color(image1.at<Vec3b>(pointImage1), image2.at<Vec3b>(pointImage2), t);
		}
	}

	return dst;
}

int save_morphing_frames(int numberFrames)
{
	Lines lines;
	lines.correspondences = new std::vector<Correspondence>();
	Mat img1, img2;
	Mat image1, image2;
	Mat dst;
	double t;

	img1 = imread(IMAGE_PATH_1, IMREAD_COLOR);
	if (img1.empty())
	{
		perror("save_morphing_frames: img1 empty\n");
		return -1;
	}

	img2 = imread(IMAGE_PATH_2, IMREAD_COLOR);
	if (img2.empty())
	{
		perror("save_morphing_frames: img2 empty\n");
		return -1;
	}

	resize(img1, img1, Size(1024, 1024), 2, 2, INTER_LINEAR);
	resize(img2, img2, Size(1024, 1024), 2, 2, INTER_LINEAR);

	image1 = img1.clone();
	image2 = img2.clone();

	lines.image1 = &image1;
	lines.image2 = &image2;
	lines.state = IDLE;

	readLinesFromFile(lines, LINES_PATH);

	for (t = 0; t <= numberFrames; t += 1)
	{
		dst = morph_two_images(img1, img2, (t / numberFrames), lines);
		imwrite(FRAMES_PATH + std::to_string(t) + ".jpg", dst);
		std::cout << "Saved image " << t << std::endl;
	}

	std::cout << "Saved " << numberFrames << " images" << std::endl;

	delete(lines.correspondences);

	return 0;
}

int show_morphing_process(double t)
{
	int r, c;
	double u, v;
	Lines lines;
	lines.correspondences = new std::vector<Correspondence>();
	Mat image1, image2;
	Mat img1, img2;
	Mat warped1, warped2, dst;

	Point2f start, end;
	Point2f X1, X2;
	Point2f displacement1, displacement2;
	Point2f displacementTotal1(0, 0);
	Point2f displacementTotal2(0, 0);
	Point2f pointImage1, pointImage2;
	double weight1, weight2;
	double weightTotal1, weightTotal2;

	img1 = imread(IMAGE_PATH_1, IMREAD_COLOR);
	if (img1.empty())
	{
		perror("show_morphing_process: img1 empty\n");
		return -1;
	}

	img2 = imread(IMAGE_PATH_2, IMREAD_COLOR);
	if (img2.empty())
	{
		perror("show_morphing_process: img2 empty\n");
		return -1;
	}

	resize(img1, img1, Size(1024, 1024), 2, 2, INTER_LINEAR);
	resize(img2, img2, Size(1024, 1024), 2, 2, INTER_LINEAR);
	dst = Mat(1024, 1024, CV_8UC3, Scalar(0, 0, 0));
	warped1 = Mat(1024, 1024, CV_8UC3, Scalar(0, 0, 0));
	warped2 = Mat(1024, 1024, CV_8UC3, Scalar(0, 0, 0));

	image1 = img1.clone();
	image2 = img2.clone();

	lines.image1 = &image1;
	lines.image2 = &image2;
	lines.state = IDLE;

	readLinesFromFile(lines, LINES_PATH);

	for (r = 0; r < 1024; r++)
	{
		for (c = 0; c < 1024; c++)
		{
			displacementTotal1 = Point2f(0, 0);
			weightTotal1 = 0;

			displacementTotal2 = Point2f(0, 0);
			weightTotal2 = 0;
			// for each intermediate line
			for (const Correspondence correspondence : *(lines.correspondences))
			{
				start = interpolate_points(correspondence.startImage1, correspondence.startImage2, t);
				end = interpolate_points(correspondence.endImage1, correspondence.endImage2, t);
				u = compute_u(start, end, Point(c, r));
				v = compute_v(start, end, Point(c, r));

				X1 = compute_x(correspondence.startImage1, correspondence.endImage1, u, v);
				displacement1 = X1 - Point2f(c, r);
				weight1 = compute_weight(start, end, Point(c, r));
				weightTotal1 += weight1;
				displacementTotal1 += weight1 * displacement1;

				X2 = compute_x(correspondence.startImage2, correspondence.endImage2, u, v);
				displacement2 = X2 - Point2f(c, r);
				weight2 = compute_weight(start, end, Point(c, r));
				weightTotal2 += weight2;
				displacementTotal2 += weight2 * displacement2;
			}
			displacementTotal1 /= weightTotal1;
			pointImage1 = Point2f(c, r) + displacementTotal1;
			warped1.at<Vec3b>(r, c) = img1.at<Vec3b>(pointImage1);

			displacementTotal2 /= weightTotal2;
			pointImage2 = Point2f(c, r) + displacementTotal2;
			warped2.at<Vec3b>(r, c) = img2.at<Vec3b>(pointImage2);

			dst.at<Vec3b>(r, c) = interpolate_color(warped1.at<Vec3b>(r, c), warped2.at<Vec3b>(r, c), t);
		}
	}

	for (const Correspondence correspondence : *(lines.correspondences)) {
		start = interpolate_points(correspondence.startImage1, correspondence.startImage2, t);
		end = interpolate_points(correspondence.endImage1, correspondence.endImage2, t);
		line(warped1, start, end, correspondence.color, 4, 8, 0);
		line(warped2, start, end, correspondence.color, 4, 8, 0);
		line(dst, start, end, correspondence.color, 4, 8, 0);
	}

	destroyAllWindows();

	imshow("image1", image1);
	imshow("warped1", warped1);
	imshow("image2", image2);
	imshow("warped2", warped2);
	imshow("dst", dst);
	waitKey(0);
	destroyAllWindows();

	return 0;
}
