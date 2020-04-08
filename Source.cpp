#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

Point getTopLeft(vector<Point> points) {
	Point topLeftPoint = points[0];
	int smallestSum = points[0].x + points[0].y;
	for (int i = 0; i < points.size(); i++) {
		if ((points[i].x + points[i].y) < smallestSum) {
			topLeftPoint = points[i];
			smallestSum = points[i].x + points[i].y;
		}
	}
	return topLeftPoint;
}

Point getBottomRight(vector<Point> points) {
	Point bottomRightPoint = points[0];
	int largestSum = points[0].x + points[0].y;
	for (int i = 0; i < points.size(); i++) {
		if ((points[i].x + points[i].y) > largestSum) {
			bottomRightPoint = points[i];
			largestSum = points[i].x + points[i].y;
		}
	}
	return bottomRightPoint;
}

Point getTopRight(vector<Point> points, Point topLeft, Point bottomRight) {
	Point topRightPoint = points[0];
	int smallestDif = abs(points[0].y - bottomRight.y) + abs(points[0].x - topLeft.x);
	for (int i = 0; i < points.size(); i++) {
		int dif = abs(points[i].y - bottomRight.y) + abs(points[i].x - topLeft.x);
		if (dif > smallestDif) {
			topRightPoint = points[i];
			smallestDif = dif;
		}
	}
	return topRightPoint;
}

Point getBottomLeft(vector<Point> points, Point topLeft, Point bottomRight) {
	Point bottomLeftPoint = points[0];
	int smallestDif = abs(points[0].y - topLeft.y) + abs(points[0].x - bottomRight.x);
	for (int i = 0; i < points.size(); i++) {
		int dif = abs(points[i].y - topLeft.y) + abs(points[i].x - bottomRight.x);
		if (dif > smallestDif) {
			bottomLeftPoint = points[i];
			smallestDif = dif;
		}
	}
	return bottomLeftPoint;
}

/*int main()
{
	Mat sudoku = imread("ye.jpg", 0);

	//Mat sudoku;
	//pyrDown(imread("sudoku3.jpg", 0), sudoku);

	Mat original = sudoku.clone();
	Mat img = Mat::zeros(original.rows, original.cols, CV_8UC3);

	// Create a duplicate. We'll try to extract grid lines in this image
	Mat outerBox = sudoku.clone();


	//erode(sudoku, sudoku, kernel);

	GaussianBlur(sudoku, sudoku, Size(9, 9), 0);
	adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);

	bitwise_not(outerBox, outerBox);

	Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	//dilate(outerBox, outerBox, kernel);

	Canny(outerBox, outerBox, 30, 60, 3);

	vector<vector<Point>> contours;
	vector<Vec4i> output;

	findContours(outerBox, contours, output, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	int finalIndex;
	int index = 0;
	int largestArea = 0;
	for (; index >= 0; index = output[index][0])
	{
		int area = contourArea(contours[index]);
		if (area > largestArea) {
			largestArea = area;
			finalIndex = index;
		}
	}

	Scalar color(rand() & 255, rand() & 255, rand() & 255);
	Scalar colore(0,0,255);
	drawContours(img, contours, finalIndex, color, FILLED, 8, output);

	vector<Point> hull(contours[finalIndex].size());
	convexHull(Mat(contours[finalIndex]), hull, false);

	Point p1 = getTopLeft(hull);
	Point p2 = getBottomRight(hull);
	Point p3 = getTopRight(hull, p1, p2);
	Point p4 = getBottomLeft(hull, p1, p2);
	circle(img, p1, 3, colore, -1);
	circle(img, p2, 3, colore, -1);
	circle(img, p3, 3, colore, -1);
	circle(img, p4, 3, colore, -1);

	vector<Point2f> corners;
	vector<Point2f> dst;
	corners.push_back(Point2f(p1));
	dst.push_back(Point2f(0,0));
	corners.push_back(Point2f(p2));
	dst.push_back(Point2f(original.rows, original.cols));
	corners.push_back(Point2f(p3));
	dst.push_back(Point2f(original.cols, 0));
	corners.push_back(Point2f(p4));
	dst.push_back(Point2f(0, original.rows));

	Mat transformMatrix = getPerspectiveTransform(corners, dst);
	Mat originalTransformed = original.clone();
	warpPerspective(original, originalTransformed, transformMatrix, originalTransformed.size());

	for (int i = 0; i < hull.size(); i++) {
		cout << hull[i] << "\n";
	}
	imshow("original", original);
	imshow("contour and corner points", img);
	imshow("perspective warp", originalTransformed);
	waitKey(0);
	return 0;
}*/