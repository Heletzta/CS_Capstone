// Homework 0 Part 4 Code
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <ctime>
//#include "Eigen/Dense"
#include <math.h>

//using namespace Eigen;
using namespace std;


int main(int argc,  char **argv)// (number of arguments, array of arguments) First value
//of the array is always the name of the document
{
	time_t start, finish;
	time(&start);

	const clock_t begin_time = clock();


	//the atoi function converts a string into an integer. 
	//The next two lines are converting the string parameters of the size of the image into integers

	int xres = atoi(argv[1]);
	int yres = atoi(argv[2]);
	//creating and opening a ppm image file
	ofstream outputFile;
	outputFile.open("PPM_Circle.ppm");
	int radius = 0;//radius of the circle to be drawn
	if(xres <= yres)//the assignment says that if the x is smaller than the y, than the radius is a fourth of the x. If not, then it is a fourth of the y
	{
		radius = xres/4;
	}
	else
	{
		radius = yres/4;
	}
	outputFile << "P3"  << endl << xres << " " << yres << endl << "255" << endl;//these are the starting lines of the ppm image(P3, size, and opacity)
	
	for(int y = 0; y < yres; y++)//the y loop goes before the x loop because a ppm image is drawn by rows, not columns
	{
		for(int x = 0; x < xres; x++)
		{
			if(pow(x-(xres/2), 2) + pow(y-(yres/2), 2) <= pow(radius, 2))//if it's inside the circle, make the color purple
			{
				outputFile << "17 239 219" << endl;
			}
			else
			{
				outputFile << "242 142 243" << endl;// if not make it blue
			}
		}
	}
	outputFile.close();
	/*time(&finish);
	cout << "Start time: " << ctime(&start) << " End time: " << ctime(&finish) << endl;
	cout << "Time required = " << difftime(finish, start) << " seconds";*/
	cout << "Time it takes: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC;

}


