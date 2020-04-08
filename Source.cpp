#include<opencv2/opencv.hpp>
#include<stdlib.h>
#include<iostream>
using namespace std;
using namespace cv;

int GRID_SIZE = 9;

struct outputNum {
    int rowNum;
    int colNum;
    int valNum;
};


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

// TODO: Remove, not needed? 
vector<vector<int>> copyGrid(vector<vector<int>> source){
    
    vector<vector<int>> dest;
    
    for(int row = 0; row < GRID_SIZE; row++){
        dest.push_back(source.at(row));
    }
    return dest;
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
Mat lmao = imread("sudoku.jpeg", 0);
vector<vector<int>> solveGrid(vector<vector<int>> grid){
	vector<vector<int>> cloneGrid(grid);
    
    // TODO: check for correct grid first
    
    //vector<tuple<int,int>> visited;
    bool conflictFlag = true;
    bool backTracking = false;
    
    
    for(int row = 0; row < GRID_SIZE;){
        // each row
        
        for(int col = 0; col < GRID_SIZE;){
            //each col
            
            // check for empty cell
            if((cloneGrid.at(row).at(col) == 0 && backTracking) || grid.at(row).at(col) == 0){
                
                int startNum = backTracking ? grid.at(row).at(col) + 1 : 1;
                backTracking = false;
                //tuple<int,int> currSpot(row, col);
                //if(visited.empty()){
                //    visited.push_back(currSpot);
               // }
                
                // starting at one, add a number and check for conflicts
                for (int i = startNum; i < GRID_SIZE+1; i++){
                    
                    //check for conflicts with this number
                    if(!checkColConflict(grid, col, i) &&
                       !checkRowConflict(grid, row, i) &&
                       !checkBoxConflict(grid, row, col, i)){
                        // no conflict, so set the number
                        grid.at(row).at(col) = i;
						col++;
                        conflictFlag = false;
                        break;
                    }
                    
                    
                }
                if(conflictFlag){
                    // have to go back to last visited spot
                    
                    // get the last empty spot that was recorded - end of emptySpots list
                    //tuple<int,int> prevCell = visited.back();
                    //cout << "Going back to cell: " << get<0>(prevCell) << ", " << get<1>(prevCell) << endl;
                    
                    // remove it from list
                   // visited.pop_back();
                    
                    // make sure that current cell value is set to 0
                    grid.at(row).at(col) = 0;
                    
                    // set row,col coordinates to go to prev cell in next loop iteration

					if (col == 0) {
						row -= 1;
						col = 8;
					}
					else {
						col--;
					}
                    
                    // Just incase - Catch an infinite loop
                    if(row < 0 || col < 0){
                        cout << "\n\n-- ERROR --\nSexiting to avoid infinite loop\n" << endl;
                        printGrid(grid);
						imshow("lmao", lmao);
						waitKey(0);
                        //exit(0);
                    }
                    
                    // flag to indicate that next iteration is a "backtrack"
                    backTracking = true;
                    
                } else {
                    conflictFlag = true;
                    
                    // add coordiantes to list of empty cells
                    //visited.push_back(currSpot);
                    
                }
                
			} else {
				if (backTracking) {
					if (col == 0) {
						row -= 1;
						col = 8;
					}
					else {
						col--;
					}
				}
				else {
					col++;
				}
			}
            // otherwise it's not an emapty cell
        }
		row++;
    }
    cout << "\n SOLVED: \n" << endl;
    printGrid(grid);
    
//    Find row, col of an unassigned cell
//    If there is none, return true
//    For digits from 1 to 9
//      a) If there is no conflict for digit at row, col
//          assign digit to row, col and recursively try fill in rest of grid
//      b) If recursion successful, return true
//      c) Else, remove digit and try another
//    If all digits have been tried and nothing worked, return false
    
    return grid;
}

vector<outputNum> getNewNumbers(vector<vector<int>> ogGrid, vector<vector<int>> answerGrid){
    
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

int main() {
    // To test sudoku solver
    
	Mat sudoku = imread("sudoku.jpeg", 0);
 
    
    // TEST GRID:
    
//    vector<vector<int>> grid = {{3, 0, 6, 5, 0, 8, 4, 0, 0},
//                                {5, 2, 0, 0, 0, 0, 0, 0, 0},
//                                {0, 8, 7, 0, 0, 0, 0, 3, 1},
//                                {0, 0, 3, 0, 1, 0, 0, 8, 0},
//                                {9, 0, 0, 8, 6, 3, 0, 0, 5},
//                                {0, 5, 0, 0, 9, 0, 6, 0, 0},
//                                {1, 3, 0, 0, 0, 0, 2, 5, 0},
//                                {0, 0, 0, 0, 0, 0, 0, 7, 4},
//                                {0, 0, 5, 2, 0, 6, 3, 0, 0}};
    
    // TEST GRID:
    // hardcoded grid that matches input sudoku image - blank spaces as 0s
   /*vector<vector<int>> grid = {{8, 0, 0, 0, 1, 0, 0, 0, 9},
                               {0, 5, 0, 8, 0, 7, 0, 1, 0},
                               {0, 0, 4, 0, 9, 0, 7, 0, 0},
                               {0, 6, 0, 7, 0, 1, 0, 2, 0},
                               {5, 0, 8, 0, 6, 0, 1, 0, 7},
                               {0, 1, 0, 5, 0, 2, 0, 9, 0},
                               {0, 0, 7, 0, 4, 0, 6, 0, 0},
                               {0, 8, 0, 3, 0, 9, 0, 4, 0},
                               {3, 0, 0, 0, 5, 0, 0, 0, 8}};*/

	vector<vector<int>> grid = { {0, 0, 0, 6, 0, 4, 7, 0, 0},
							   {7, 0, 6, 0, 0, 0, 0, 0, 9},
							   {0, 0, 0, 0, 0, 5, 0, 8, 0},
							   {0, 7, 0, 0, 2, 0, 0, 9, 3},
							   {8, 0, 0, 0, 0, 0, 0, 0, 5},
							   {4, 3, 0, 0, 1, 0, 0, 7, 0},
							   {0, 5, 0, 2, 0, 0, 0, 0, 0},
							   {3, 0, 0, 0, 0, 0, 2, 0, 8},
							   {0, 0, 2, 3, 0, 1, 0, 0, 0} };
    
//    printGrid(grid);
    vector<vector<int>> solvedGrid = solveGrid(grid);
	//imshow("as", sudoku);
	//waitKey(0);
    vector<outputNum> addedNums = getNewNumbers(grid, solvedGrid);
    
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
    
//    line(Mat& img, Point pt1, Point pt2, const Scalar& color, int thickness=1, int lineType=8, int shift=0)
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
    
    imshow("image", sudoku);
    waitKey(0);
    return 0;
}

int main2()
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
