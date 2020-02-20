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

	imshow("image", outerBox);
	waitKey(0);
	return 0;
}