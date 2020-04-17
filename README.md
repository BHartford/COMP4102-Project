# COMP4102-Project

## Title
Sudoku solver

Abhi Santhosh<br />
Brandon Hartford<br />
Eliza Moore
## Summary
In this project, our application will take an image of an unsolved sudoku puzzle, and return an image with the correct numbers filled in to any blank box to solve the puzzle. Our project will include a feature that will colour the numbers and grid so that it easier to tell what was modified.
## Background
A sudoku solver has been done before with OpenCV, and most are achieved by detecting the grid, extracting the grid and detecting the digits within the grid, then solving it.1,2 Our project will also be a sudoku solver, but it will colour the grid and the numbers to help differentiate the numbers that were already there from the ones were added as part of the solution. The sudoku solvers that we have seen online use an older version of OpenCV, whereas we will be using a newer version of the computer vision library; this will change the project implementation and use of OpenCV. Most examples of this project did not include a training set for recognizing digits or hand-written digits - this will be something that we will have to figure out. 
## The Challenge
The challenge of creating the sudoku solver is that there are many steps that need to be considered and implemented. The photo must first be filtered and translated to to better detect edges and correct the image if it is skewed. The digits then have to be detected and read using neural networks. Once the data is extracted and organized,  the puzzle can be solved. The output image will be edited to make the sudoku grid more colourful so that it is easier to read, and existing numbers will be coloured differently than the ones that will be added to the image.
## Goals and Deliverables
The project has four natural sections that can be separated out as deliverables: Image capture and preparation, image analysis, determine puzzle solution, and create the final output image. 

The image capture and preparation stage includes capturing the image from the webcam or loaded from disk, then applying appropriate filters and transformations to the image so it can be analyzed in the next stage (Blurring and/or thresholding for edge detection to detect the sudoku grid). After this stage, the running application will be able to read and image, and output an image with a blurred filter applied.

The image analysis stage includes detecting the puzzle grid, and each individual box, then checking each box. The contents of each box can be recorded, and any existing numbers can be detected and captured for storage. After this stage, the application should be able to output the numbers read from the original image, and output the blurred image as done in the first stage. 

Once all the data from the image has been extracted, the sudoku solving algorithm can be applied to the data to find the solution - and determine which numbers will have to be added to the output image so show the answer. After this stage, the application should be able to also output a text version of the solution - to show that the numbers have been read and the solution has been determined. 

Finally, the output image can be created to include the solution, and functions will be applied to change colour-spaces of the image. The output image will be displayed with the correct numbers displayed over the empty boxes. The added numbers and grid will be colourized to make it easier to tell what was modified.

Another addition to the project, if time permits, is a staged reveal of the answer. If users want to use this application for hints, it might be useful to have multiple output images, each showing a little more of the solution. 

This project seems reasonable for the given time because it is something that has been done before so we have some material to check with and the bonus feature we are adding is reasonable

## Schedule
With 8 weeks until the end of March and 4, possibly 5 deliverables, we plan on finishing a deliverable every 2 weeks. The first week of each deliverable run will be dedicated to planning and dividing up the work, and the second week will be for each of us to finish our respective parts, and review new code. We plan to check in twice a week and help each other whenever needed. 

<br />Deliverable 1<br />
Thresholding the sudoku image
<br />Deliverable 2<br />
Detecting the sudoku grid is in the picture
<br />Deliverable 3<br />
Extract the grid from the picture and transform it so that the grid is the only thing in the photo for the next part
<br />Deliverable 4<br />
The digits will be detected and recognized through a neural net that can detect printed numbers and hand written numbers
<br />Deliverable 5<br />
The puzzle will be solved and the numbers will be placed onto the image to solve it

# Final Report

## Dependencies
The program requires the header dirent.h. It is included in the C POSIX library so if you are on a Mac or UNIX system you wont need the file. If you are using MSVC, you will need the dirent.h file added to the project.

## Instructions
* Pull code from repository
* If you are on windows, add the dirent.h file as a header in the project. If you are on Mac or Linux you will have to remove the dirent.h file.
* Run the program and it will output the solved image of the sudoku puzzle.
* There are 2 photos which are used for testing, sudoku.jpeg and sudoku2.jpg. The sudoku2.jpg file is currently in the code. To test the other image, change the image being loaded in, in the main method.
## Final Results
This is an image of an unsolved sudoku image
![Image of Unsolved Sudoku Image](sudoku2.jpg)

After running the image through the program. The answers to the sudoku puzzle are put back on the image and colorized to make it easier to see

![Image of Solved Sudoku Image](sudoku2SOLVED.jpg)

Another sudoku image

![Unsolved Sudoku Image](sudoku.jpeg)

![Unsolved Sudoku Image](sudokuSOLVED.jpg)
