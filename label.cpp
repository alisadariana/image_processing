#include "label.hpp"
#include <queue>
#include <random>

int n4i[4] = {-1, 0, 1, 0};
int n4j[4] = {0, -1, 0, 1};

int n8i[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int n8j[8] = {-1, -1, -1, 0, 0, 1, 1, 1};

int npi[4] = {0, -1, -1, -1};
int npj[4] = {-1, -1, 0, 1};

void generate_random_colors(int number, Vec3b *colors)
{
	int k;
	uchar r, g, b;
	std::default_random_engine gen;
	std::uniform_int_distribution<int> d(0, 255);

	for (k = 0; k < number; k++) {
		r = d(gen);
		g = d(gen);
		b = d(gen);
		colors[k][0] = b;
		colors[k][1] = g;
		colors[k][2] = r;
	}
}

Mat breadth_first_traversal_labeling(Mat image)
{
	int i, j;
	int k;
	int auxI, auxJ;
	int label = 0;
	std::queue<cv::Point2i> queuePoints;
	cv::Point2i point;
	Mat out;

	out = Mat(image.rows, image.cols, CV_8UC3);

	int labels[image.rows][image.cols];
	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			labels[i][j] = 0;

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) == 0 && labels[i][j] == 0) {
				label++;
				labels[i][j] = label;
				queuePoints.push({i, j});
				while (!queuePoints.empty()) {
					point = queuePoints.front();
					queuePoints.pop();
					for (k = 0; k < sizeof(n8i) / sizeof(n8i[0]); k++) {
						auxI = point.x + n8i[k];
						auxJ = point.y + n8j[k];
						if (is_inside_image(image, auxI, auxJ))
							if (image.at<uchar>(auxI, auxJ) == 0 && labels[auxI][auxJ] == 0) {
								labels[auxI][auxJ] = label;
								queuePoints.push({auxI, auxJ});
							}
					}
				}
			}
		}
	}

	// label = number of objects
	cv::Vec3b colors[label + 1];

	// white color for background
	colors[0][0] = 255;
	colors[0][1] = 255;
	colors[0][2] = 255;

	generate_random_colors(label, &colors[1]);

	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			out.at<Vec3b>(i, j) = colors[labels[i][j]];

	return out;
}

Mat two_pass_labeling(Mat image)
{
	int i, j;
	int k, l;
	int x;
	int auxI, auxJ;
	int label = 0;
	int newLabel = 0;
	std::queue<int> queuePoints;
	cv::Point2i point;
	int labelVector[1000];
	int labelIter = 0;
	std::vector<std::vector<int>> edges(1000);
	Mat out;

	out = Mat(image.rows, image.cols, CV_8UC3);

	int labels[image.rows][image.cols];
	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			labels[i][j] = 0;

	for (i = 0; i < image.rows; i++) {
		for (j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) == 0 && labels[i][j] == 0) {
				labelIter = 0;
				for (k = 0; k < sizeof(n4i) / sizeof(n4i[0]); k++) {
					auxI = i + npi[k];
					auxJ = j + npj[k];
					if (is_inside_image(image, auxI, auxJ))
						if (labels[auxI][auxJ] > 0) {
							labelVector[labelIter] = labels[auxI][auxJ];
							labelIter++;
						}
				}
				if (labelIter == 0) {
					label++;
					labels[i][j] = label;
				}
				else {
					x = labelVector[0];
					for (l = 1; l < labelIter; l++)
						if (labelVector[l] < x)
							x = labelVector[l];

					labels[i][j] = x;
					for (l = 0; l < labelIter; l++)
						if (x != labelVector[l]) {
							edges[x].push_back(labelVector[l]);
							edges[labelVector[l]].push_back(x);
						}
				}
			}
		}
	}

	int newLabels[label + 1];

	for (int i = 0; i <= label; i++)
		newLabels[i] = 0;

	for (i = 1; i <= label; i++) {
		if (newLabels[i] == 0) {
			newLabel++;
			newLabels[i] = newLabel;
			queuePoints.push(i);
			while (!queuePoints.empty()) {
				x = queuePoints.front();
				queuePoints.pop();
				for (k = 0; k < edges[x].size(); k++)
					if (newLabels[edges[x].at(k)] == 0) {
						newLabels[edges[x].at(k)] = newLabel;
						queuePoints.push(edges[x].at(k));
					}
			}
		}
	}

	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			labels[i][j] = newLabels[labels[i][j]];

	// newLabel = number of objects
	cv::Vec3b colors[newLabel + 1];

	// white color for background
	colors[0][0] = 255;
	colors[0][1] = 255;
	colors[0][2] = 255;

	generate_random_colors(newLabel, &colors[1]);

	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			out.at<Vec3b>(i, j) = colors[labels[i][j]];

	return out;
}
