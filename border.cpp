#include "border.hpp"

#define NR_NEIGHBORS 8

extern int is_inside_image(Mat image, int row, int col);

void get_next_dir_n8(int *dir)
{
	if (*dir % 2 == 0)
		*dir = (*dir + 7) % 8;
	else
		*dir = (*dir + 6) % 8;
}

std::pair<int, int> get_first(Mat image)
{
	int i, j;
	for (i = 0; i < image.rows; i++)
		for (j = 0; j < image.cols; j++)
			if (image.at<uchar>(i, j) == 0)
				return std::make_pair(i, j);
	return std::make_pair(-1, -1);
}

std::pair<int, int> get_next_neighbor(Mat image, std::pair<int, int> prev, int *dir, int (*n8)[2])
{
	int i;

	for (i = 0; i < NR_NEIGHBORS; i++) {
		if (is_inside_image(image, prev.first + n8[*dir][0], prev.second + n8[*dir][1])) {
			if (image.at<uchar>(prev.first + n8[*dir][0], prev.second + n8[*dir][1]) == 0) {
				return std::make_pair(prev.first + n8[*dir][0], prev.second + n8[*dir][1]);
			}
		}
		*dir = (*dir + 1) % NR_NEIGHBORS;
	}
	return std::make_pair(-1, -1);
}

Mat border_of_single_object(Mat image, void *chainCode)
{
	int i, j, k;
	int dir;
	int count = 0;
	Mat out;
	std::vector <std::pair<int, int>> border;
	int n8[8][2] = { { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 },
			 { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

	if (chainCode != NULL)
		chainCode = (std::vector <int> *)chainCode;

	out = Mat(image.rows, image.cols, CV_8UC1, Scalar(255));

	border.push_back(get_first(image));

	if (border[0].first == -1)
		return out;

	out.at<uchar>(border[0].first, border[0].second) = 0;

	dir = 7;
	get_next_dir_n8(&dir);

	border.push_back(get_next_neighbor(image, border[0], &dir, &n8[0]));
	if (border[1].first == -1)
		return out;

	out.at<uchar>(border[1].first, border[1].second) = 0;
	if (chainCode != NULL)
		((std::vector <int> *)chainCode)->push_back(dir);

	count = 1;
	do {
		get_next_dir_n8(&dir);

		border.push_back(get_next_neighbor(image, border[count], &dir, &n8[0]));
		count++;
		if (border[count].first == -1)
			return out;
		
		out.at<uchar>(border[count].first, border[count].second) = 0;
		if (chainCode != NULL)
			((std::vector <int> *)chainCode)->push_back(dir);

	} while ((border[count] != border[1]) && (border[count-1] != border[0]));
	
	return out;
}

Mat object_reconstruction_from_chain_code(Mat image, std::vector<int> chainCode, int startRow, int startCol)
{
	int n8[8][2] = { { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 },
			 { 0, -1 }, { 1, -1 }, { 1, 0 }, { 1, 1 } };

	image.at<uchar>(startRow, startCol) = 0;
	
	for (int dir : chainCode) {
		startRow += n8[dir][0];
		startCol += n8[dir][1];
		image.at<uchar>(startRow, startCol) = 0;
	}

	return image;
}
