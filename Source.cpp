#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
//#include "ml.h"

#include<opencv2/opencv.hpp>
#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include "dirent.h"
using namespace std;
using namespace cv;

int GRID_SIZE = 9;

// UNASSIGNED is used for empty cells in sudoku
#define UNASSIGNED 0

// N is used for size of Sudoku grid. Size will be NxN
#define N 9

struct outputNum {
    int rowNum;
    int colNum;
    int valNum;
};


// Helper Methods

void printGrid(vector<vector<int>> grid) {
    
    String rowSeparator = "-------------------";
    
    cout << rowSeparator << endl;
    for(int row = 0; row < grid.size(); row++){
        
        cout << "|";
        
        for(int col = 0; col < grid.at(row).size(); col++){
            
            cout << grid.at(row).at(col);
            
            if(col == 2 || col == 5 || col == 8){
                cout << "|";
            } else {
                cout << " ";
            }
        }
        cout << endl;
        
        if(row == 2 || row == 5 || row == 8){
            cout << rowSeparator << endl;
        }
    }
}

vector<int> getQuadrantRange(int row, int col){
    //  ------------
    // | 1 | 2 | 3 |
    //  ------------
    // | 4 | 5 | 6 |
    //  ------------
    // | 7 | 8 | 9 |
    //  ------------
    vector<int> quadRange;
    
    if(row < 3){
        // quads 1 - 3
        quadRange.push_back(2);
        if(col < 3) {
            quadRange.push_back(2);
        } else if (col < 6) {
            quadRange.push_back(5);
        } else {
            quadRange.push_back(8);
        }
    } else if (row < 6){
        // quads 4 - 6
        quadRange.push_back(5);
        if(col < 3) {
            quadRange.push_back(2);
        } else if (col < 6) {
            quadRange.push_back(5);
        } else {
            quadRange.push_back(8);
        }
    } else {
        // quads 7 - 9
        quadRange.push_back(8);
        if(col < 3) {
            quadRange.push_back(2);
        } else if (col < 6) {
            quadRange.push_back(5);
        } else {
            quadRange.push_back(8);
        }
    }
    
    return quadRange;
}

vector<int> getQuadrantNumbers(vector<vector<int>> grid, vector<int> quadRanges){
    
    int maxRow = quadRanges.at(0);
    int maxCol = quadRanges.at(1);
    int startRow = maxRow - 2;
    int startCol = maxCol - 2;
    
    vector<int> quadNums;
    for(int row = startRow; row <= maxRow; row++){
        
        for(int col = startCol; col <= maxCol; col++){
            quadNums.push_back(grid.at(row).at(col));
        }
    }

    return quadNums;
}

bool checkRowConflict(vector<vector<int>> grid, int row, int num){
    
    for(int col = 0; col < GRID_SIZE; col++){
        if(grid.at(row).at(col) == num){
            // conflict found - num already exists in row
            return true;
        }
    }
    return false;
}

bool checkColConflict(vector<vector<int>> grid, int col, int num){
    
    for(int row = 0; row < GRID_SIZE; row++){
        if(grid.at(row).at(col) == num){
            // conflict found - num already exists in col
            return true;
        }
    }
    
    return false;
}

bool checkBoxConflict(vector<vector<int>> grid, int row, int col, int num){
    
    vector<int> quadRange = getQuadrantRange(row, col);
    vector<int> quadNums = getQuadrantNumbers(grid, quadRange);
    
    for(int i = 0; i < GRID_SIZE; i++){
        if(quadNums.at(i) == num){
            // conflict found - num already exists in the quadrant
            return true;
        }
        
    }
    
    return false;
}

vector<vector<int>> solveGrid(vector<vector<int>> grid){
    
    //    Find row, col of an unassigned cell
    //    If there is none, return true
    //    For digits from 1 to 9
    //      a) If there is no conflict for digit at row, col
    //          assign digit to row, col and recursively try fill in rest of grid
    //      b) If recursion successful, return true
    //      c) Else, remove digit and try another
    //    If all digits have been tried and nothing worked, return false
    
    vector<tuple<int,int>> visited;
    bool conflictFlag = true;
    bool backTracking = false;
    int count = 0;
    
    for(int row = 0; row < GRID_SIZE; row++){
        // each row
        
        for(int col = 0; col < GRID_SIZE; col++){
            //each col
            
            // check for empty cell
            if(grid.at(row).at(col) == 0 || backTracking){
                
                int startNum = backTracking ? grid.at(row).at(col) + 1 : 0;
                backTracking = false;
                tuple<int,int> currSpot(row, col);
                if(visited.empty()){
                    visited.push_back(currSpot);
                }
                
                // starting at one, add a number and check for conflicts
                for (int i = startNum; i < GRID_SIZE+1; i++){
                    
                    //check for conflicts with this number
                    if(!checkColConflict(grid, col, i) &&
                       !checkRowConflict(grid, row, i) &&
                       !checkBoxConflict(grid, row, col, i)){
                        // no conflict, so set the number
                        grid.at(row).at(col) = i;
                        conflictFlag = false;
                        break;
                    }
                    
                }
                if(conflictFlag){
                    // have to go back to last visited spot
                    
                    // get the last empty spot that was recorded - end of emptySpots list
                    tuple<int,int> prevCell = visited.back();
//                    cout << "Going back to cell: " << get<0>(prevCell) << ", " << get<1>(prevCell) << endl;
                    
                    // remove it from list
                    visited.pop_back();
                    
                    // make sure that current cell value is set to 0
                    grid.at(row).at(col) = 0;
                    
                    // set row,col coordinates to go to prev cell in next loop iteration
                    row = get<0>(prevCell);
                    col = get<1>(prevCell);
                    
                    row = col == 0 ? row - 1 : row;
                    
                    col = col == 0 ? 8 : col -1;
                    
                    // Just incase - Catch an infinite loop
                    if(row < 0 || col < 0 || count > 1000){
                        cout << "\n\n-- ERROR - Not Solveable --\nexiting to avoid infinite loop\n" << endl;
                        printGrid(grid);
                        exit(0);
                    }
                    
                    // flag to indicate that next iteration is a "backtrack"
                    backTracking = true;
                    
                } else {
                    conflictFlag = true;
                    
                    // add coordiantes to list of empty cells
                    visited.push_back(currSpot);
                }
            }
            count++;
            // otherwise it's not an emapty cell
        }
    }
    
    cout << "\n SOLVED: \n" << endl;
    printGrid(grid);
    return grid;
}

vector<outputNum> getNewNumbers(vector<vector<int>> ogGrid, vector<vector<int>> answerGrid){
    
    //TODO: What if no solution found? can't be sovled? used to return bool, no return solution grid...
    vector<outputNum> addedNums;
    
    for(int row = 0; row < GRID_SIZE; row++){
        
        for(int col = 0; col < GRID_SIZE; col++){
            
            // check to see if it was added
            if(ogGrid.at(row).at(col) == 0){
                outputNum addedNum;
                addedNum.rowNum = row;
                addedNum.colNum = col;
                addedNum.valNum = answerGrid.at(row).at(col);
                
                addedNums.push_back(addedNum);
            }
        }
    }
    
    return addedNums;
    
}

Mat drawSolutionOnImage(Mat sudoku, vector<outputNum> addedNums){
    int cellWidth = (sudoku.cols - 40) / GRID_SIZE;
    int cellHeight = (sudoku.rows - 40) / GRID_SIZE;
    int yPadding = 115; // extra padding needed for y-coordinates
    int xPadding = 40; // extra padding needed for x-coordinates
    
    for(int i = 0; i < addedNums.size(); i++){
        outputNum addedNum = addedNums.at(i);
        
        string value = to_string(addedNum.valNum);
        int y = addedNum.rowNum * cellHeight + yPadding;
        int x = addedNum.colNum * cellWidth + xPadding;
        
        putText(sudoku, value, Point(x,y), FONT_HERSHEY_DUPLEX, 4, Scalar(170, 150, 250), 1);
        
    }
    
    int start = 0;
    int end = 9 * cellHeight + yPadding;
    
    // draw vertical and horizontal lines that form "boxes"
    for(int i = 0; i < 4; i++){
        int linePos = (i*3) * cellWidth + 26;
        Point h_p1 = Point(start, linePos);
        Point h_p2 = Point(end, linePos);
        
        Point v_p1 = Point(linePos, start);
        Point v_p2 = Point(linePos, end);
        
        
        line(sudoku, h_p1, h_p2, Scalar(170, 0, 250), 3);
        line(sudoku, v_p1, v_p2, Scalar(170, 0, 250), 3);
    }
    
    return sudoku;
}

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

Mat warpSudokuGrid(Mat sudokuImg) {

	Mat original = sudokuImg.clone();
	Mat img = Mat::zeros(original.rows, original.cols, CV_8UC3);

	Mat outerBox = sudokuImg.clone();

	GaussianBlur(sudokuImg, sudokuImg, Size(9, 9), 0);
	adaptiveThreshold(sudokuImg, outerBox, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);

	bitwise_not(outerBox, outerBox);

	Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

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
	Scalar colore(0, 0, 255);
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
	dst.push_back(Point2f(0, 0));
	corners.push_back(Point2f(p2));
	dst.push_back(Point2f(original.rows, original.cols));
	corners.push_back(Point2f(p3));
	dst.push_back(Point2f(original.cols, 0));
	corners.push_back(Point2f(p4));
	dst.push_back(Point2f(0, original.rows));

	Mat transformMatrix = getPerspectiveTransform(corners, dst);
	Mat originalTransformed = original.clone();
	warpPerspective(original, originalTransformed, transformMatrix, originalTransformed.size());

	//for (int i = 0; i < hull.size(); i++) {
	//	cout << hull[i] << "\n";
	//}
	//imshow("original", original);
	//imshow("contour and corner points", img);
	//imshow("perspective warp", originalTransformed);

	return originalTransformed;
}
//
//int main()
//{
//    // Pre-processing the image
//
//	Mat sudoku = imread("sudokuGood.jpg", 0);
////    Mat sudoku = imread("/Users/jdlmmoore/Desktop/CarletonU/Year Three/Winter 2020/COMP 4102/COMP4102-Project/sudokuGood.jpg", 0);
//	//warpedSudoku is the image that has been warped to only show the grid
//	Mat warpedSudoku = warpSudokuGrid(sudoku);
//
//    // To show warped:
////    imshow("Warped Sudoku", warpedSudoku);
////    waitKey(0);
//
//
//    // identifying numbers to create grid
//
//
//
//    vector<vector<int>> grid = {{8, 0, 0, 0, 1, 0, 0, 0, 9},
//    {0, 5, 0, 8, 0, 7, 0, 1, 0},
//    {0, 0, 4, 0, 9, 0, 7, 0, 0},
//    {0, 6, 0, 7, 0, 1, 0, 2, 0},
//    {5, 0, 8, 0, 6, 0, 1, 0, 7},
//    {0, 1, 0, 5, 0, 2, 0, 9, 0},
//    {0, 0, 7, 0, 4, 0, 6, 0, 0},
//    {0, 8, 0, 3, 0, 9, 0, 4, 0},
//    {3, 0, 0, 0, 5, 0, 0, 0, 8}};
//
//
//
//    // solve puzzle, and show solution on original image
//
//    vector<vector<int>> solvedGrid = solveGrid(grid);
//
//    vector<outputNum> addedNums = getNewNumbers(grid, solvedGrid);
//
//    Mat solvedSudoku = drawSolutionOnImage(sudoku, addedNums);
//
//    imshow("Solved Sudoku", solvedSudoku);
//    waitKey(0);
//	return 0;
//}

Mat removeGridLines(Mat thresholded31){
    
    // TODO: Add comments in this section
    
    int p2=0;int p3=0;
    
    while(p3<thresholded31.cols)
    {
        for( int i=p3;i<p3+10;i++)
        {
            for(int j=0;j<thresholded31.rows;j++)
            {
                thresholded31.at<uchar>(j,i)=0;

            }
        }
        p3+=47;
    }

    while(p2<thresholded31.rows)
    {
        for( int i=0;i<thresholded31.cols;i++)
        {
            for(int j=p2;j<p2+10;j++)
            {
                thresholded31.at<uchar>(j,i)=0;

            }
        }
        p2+=47;
    }

    for(int i=thresholded31.cols - 10;i<thresholded31.cols;i++)
    {
        for(int j=0;j<thresholded31.rows;j++)
        {
            thresholded31.at<uchar>(j,i)=0;
        }
    }

    for(int i=0;i<thresholded31.cols;i++)
    {
        for(int j=thresholded31.rows - 10;j<thresholded31.rows;j++)
        {
            thresholded31.at<uchar>(j,i)=0;
        }
    }

    for(int i=0;i<thresholded31.cols;i++)
    {
        for(int j=150;j<160;j++)
        {
            thresholded31.at<uchar>(j,i)=0;
        }

    }
    
    return thresholded31;
}

vector<vector<int>> readImageNumbers(Mat thresholded31){
    int num = 797;
    int size = 16 * 16;
    Mat trainData = Mat(Size(size, num), CV_32FC1);
    Mat responces = Mat(Size(1, num), CV_32FC1);

    int counter = 0;
    for(int i=0;i<=9;i++)
    {

        DIR *dir;
        struct dirent *ent;
        char pathToImages[]="digits3";
        
        char path[255];
        sprintf(path, "%s/%d", pathToImages, i);
        if ((dir = opendir(path)) != NULL)
        {
            while ((ent = readdir (dir)) != NULL)
            {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0 )
                {
                    char text[255];
                    sprintf(text,"/%s",ent->d_name);
                    string digit(text);
                    digit=path+digit;

                    Mat mat=imread(digit,1);

                    cvtColor(mat,mat,CV_BGR2GRAY);

                    threshold(mat , mat , 200, 255 ,THRESH_OTSU);

                    mat.convertTo(mat,CV_32FC1,1.0/255.0);

                    resize(mat, mat, Size(16,16 ),0,0,INTER_NEAREST);

                    mat.reshape(1,1);

                    for (int k=0; k<size; k++)
                    {
                        trainData.at<float>(counter*size+k) = mat.at<float>(k);
                    }
                    responces.at<float>(counter) = i;
                    counter++;

                }

            }

        }
        closedir(dir);
    }

    Ptr<ml::KNearest> knearest = ml::KNearest::create();
    knearest->train(trainData, ml::ROW_SAMPLE, responces);

    vector <Mat> small; vector <Mat> smallt;
	imshow("yaga", thresholded31);
    int m = 0, n = 0; Mat smallimage; Mat smallimage2;
    for (; m < thresholded31.rows; m = m + 100)
    {
        for (n = 0; n < thresholded31.cols; n = n + 100)
        {
			if (m < thresholded31.rows - 100 && n < thresholded31.cols - 100) {
				smallimage = Mat(thresholded31, cv::Rect(n, m, 100, 100));
			}

            smallt.push_back(smallimage);
        }
    }


    int z[9][9];
    for(size_t i=0;i<smallt.size();i++)
    {

        Mat img123 = Mat(Size(size, 1), CV_32FC1);

        if(countNonZero(smallt[i])>200)
        {

            Mat thresholded32; Mat regionOfInterest; Mat img12;


            thresholded32=smallt[i].clone();

            vector < vector <Point> >contours2;

            findContours(thresholded32, contours2, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

            Rect prevb; double areaprev = 0; double area2; int q;

            for (int j = 0; j < contours2.size(); j++)
            {

                Rect bnd = boundingRect(contours2[j]);

                area2 = bnd.height*bnd.width;

                if (area2 > areaprev)
                {
                    prevb = bnd;
                    areaprev = area2;

                    q = j;
                }
            }

            Rect rec = prevb;

            regionOfInterest = smallt[i](rec);
			imshow("reg", regionOfInterest);

            resize(regionOfInterest, img12, Size(16,16),0,0,INTER_NEAREST);



            img12.convertTo(img12,CV_32FC1,1.0/255.0);
            img12.reshape(1,1);


            Mat output;
            if(countNonZero(img12)>50)
            {
               imshow("display" + i,img12);
               // waitKey(0);
                /*for(int k=0;k<size;k++)
                {
                    img123.at<float>(k) = img12.at<float>(k);
                }*/
			   img123 = img12.clone();
                Mat res;
                float p = knearest->findNearest(img123.reshape(1,1), 1, res);
				cout << p << "\n";
                //float p=knearest.find_nearest(img123.reshape(1,1),1);

                z[i/9][i%9]=p;
            }
            else

                z[i/9][i%9]=0;
        }
        else z[i/9][i%9]=0;

    }
    
    vector<vector<int>> grid;

    for(int i=0;i<9;i++)
    {
        vector<int> row;
        for(int j=0;j<9;j++)
        {
            row.push_back(z[i][j]);
        }
        grid.push_back(row);
    }
    
    return grid;
}

Mat testRemove(Mat img) {
	Mat horizontal = img.clone();
	Mat vertical = img.clone();
	// Specify size on horizontal axis
	int horizontal_size = horizontal.cols / 50;
	// Create structure element for extracting horizontal lines through morphology operations
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));
	// Apply morphology operations
	erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
	dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));
	// Show extracted horizontal lines
	// Specify size on vertical axis
	int vertical_size = vertical.rows / 50;
	// Create structure element for extracting vertical lines through morphology operations
	Mat verticalStructure = getStructuringElement(MORPH_RECT, Size(1, vertical_size));
	// Apply morphology operations
	erode(vertical, vertical, verticalStructure, Point(-1, -1));
	dilate(vertical, vertical, verticalStructure, Point(-1, -1));
	// Show extracted vertical lines
	// Inverse vertical image
	imshow("vertical", vertical);
	imshow("horizontal", horizontal);
	waitKey(0);
	return img;
}

Mat tester(Mat img) {
	//cvtColor(img, img, COLOR_BGR2GRAY);
	GaussianBlur(img, img, Size(7, 7), 0, 0);
	adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 5, 2);
	Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	dilate(img, img, kernel, Point(-1, -1), 1);
	erode(img, img, 2);
	Mat clone = img.clone();
	Mat detected_lines;
	Mat horizontal_kernel = getStructuringElement(MORPH_RECT, Size(25, 1));
	morphologyEx(img, detected_lines, MORPH_OPEN, horizontal_kernel, Point(-1,-1), 2);
	//cnts = cv2.findContours(detected_lines, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	vector<vector<Point> > contours;
	findContours(detected_lines, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	Scalar color = Scalar(0, 0, 0);
	//imshow("before", img);
	for (int i = 0; i < contours.size(); i++) {
		drawContours(img, contours, i, color, 5);
	}

	//imshow("horiz", img);

	vector<vector<Point> > contours_vert;
	Mat detected_lines_vert;
	Mat vertical_kernel = getStructuringElement(MORPH_RECT, Size(1, 25));
	morphologyEx(clone, detected_lines_vert, MORPH_OPEN, vertical_kernel, Point(-1, -1), 2);
	//cnts = cv2.findContours(detected_lines, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
	findContours(detected_lines_vert, contours_vert, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours_vert.size(); i++) {
		drawContours(clone, contours_vert, i, color, 5);
	}
	Mat yeet;
	bitwise_and(img, clone, yeet);
	//imshow("vert", clone);
	imshow("yeet", yeet);
	//waitKey(0);
	return yeet;
}

int main( int argc, char** argv )
{
    
    // Pre-processing the image
    
    Mat sudoku = imread("sudokuGood.jpg", 0);

    // warpedSudoku is the image that has been warped to only show the grid
    Mat warpedSudoku = warpSudokuGrid(sudoku);
	Mat test = warpedSudoku.clone();

//        imshow("Warped Sudoku", warpedSudoku);
//        waitKey(0);

    

    Mat ch; Mat thresholded31;
    ch = warpedSudoku;

    GaussianBlur(ch, ch, Size(11, 11), 0, 0);

    adaptiveThreshold(ch, thresholded31, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
    bitwise_not(thresholded31, thresholded31);

    Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
    dilate(thresholded31, thresholded31, kernel,Point(-1,-1),1);

    erode(thresholded31,thresholded31,2);

    //thresholded31 = removeGridLines(thresholded31);
	thresholded31 = tester(test);

    //imshow("thresholded new",thresholded31);
	//waitKey(0);

    // Identify numbers from image and create a grid
    

    vector<vector<int>> grid = readImageNumbers(thresholded31);
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			cout << grid[i][j] << " ";
		}
		cout << "\n";
	}

//        // For Testing:
//        vector<vector<int>> grid = {{8, 0, 0, 0, 1, 0, 0, 0, 9},
//        {0, 5, 0, 8, 0, 7, 0, 1, 0},
//        {0, 0, 4, 0, 9, 0, 7, 0, 0},
//        {0, 6, 0, 7, 0, 1, 0, 2, 0},
//        {5, 0, 8, 0, 6, 0, 1, 0, 7},
//        {0, 1, 0, 5, 0, 2, 0, 9, 0},
//        {0, 0, 7, 0, 4, 0, 6, 0, 0},
//        {0, 8, 0, 3, 0, 9, 0, 4, 0},
//        {3, 0, 0, 0, 5, 0, 0, 0, 8}};

    
    //    printGrid(grid);
    
    // solve puzzle, and show solution on original image
    
    vector<vector<int>> solvedGrid = solveGrid(grid);

    vector<outputNum> addedNums = getNewNumbers(grid, solvedGrid);

    Mat solvedSudoku = drawSolutionOnImage(thresholded31, addedNums);

    
    imshow("Solved Sudoku", solvedSudoku);
    
    waitKey(0);
    return 0;

}
