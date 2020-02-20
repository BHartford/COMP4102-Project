#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;
int main()
{
	Mat sudoku = imread("sudoku.jpeg", 0);

	//Mat sudoku;
	//pyrDown(imread("sudoku3.jpg", 0), sudoku);

	Mat original = sudoku.clone();

	// Create a duplicate. We'll try to extract grid lines in this image
	Mat outerBox = Mat(sudoku.size(), CV_8UC1);


	//erode(sudoku, sudoku, kernel);

	GaussianBlur(sudoku, sudoku, Size(9, 9), 0);
	adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);

	bitwise_not(outerBox, outerBox);

	Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	dilate(outerBox, outerBox, kernel);
	vector<vector<Point>> contours;
	findContours(outerBox, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	sort(contours.begin(), contours.end(), [](const vector<Point>& c1, const vector<Point>& c2) {
		return contourArea(c1, false) < contourArea(c2, false);
	});

	vector<Point> polygon = contours[contours.size() - 1];
	for (int i = 0; i < polygon.size(); i++) {
		Point p = polygon[i];
		cout << p.x << "," << p.y << "\n";
	}
	map<int,int> list;
	for (int i = 0; i < polygon.size(); i++) {
		Point p = polygon[i];
		list[p.x + p.y] = i;
	}
	int largest = 0;
	for (int i = 0; i < list.size(); i++) {
		if (list[i] > largest) {
			largest = list[i];
		}
	}
	list.find(largest);
	Point bottomRight = polygon[]
	imshow("image", outerBox);
	waitKey(0);
	return 0;
}