//HW1
/* Homework 1


This program makes the image of a bunny in 3D
*/
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include "Eigen/Dense"
#include <math.h>
#include <cstring>
#include <map> 

using namespace Eigen;
using namespace std;

struct Triple //makes a type that can store 3 values: x, y, and z coordinates
{
    float x;
    float y;
    float z;
};

struct Double //makes a type that can store 2 values: x, y, and z coordinates
{
    float x;
    float y;
};


struct Quadruple
{
	float x;
	float y;
	float z;
	float w;
};


struct TripleUInt
{
	int x;
	int y;
	int z;
};

struct TripleInt
{
	 int x;
	 int y;
	 int z;
};


Matrix4d objTransProduct(vector<Triple> translations, vector<Triple> scaling, vector<Quadruple> rotations);
bool readTxtFile(const char * path);
bool loadObj(string sPath, Matrix4d proMatrix);
Matrix4d WorldtoCameraMatrix(Triple positionNum, Quadruple orientationNum);
Matrix4d projectionMatrix(int nearNum, int farNum, float leftNum, float rightNum, float topNum, float bottomNum);
Matrix4d finalNDC();
void fill();
void Bresenham(int x1, int y1, int x2, int y2);
int height = 800;
int width = 800;
vector<Triple> vertices; 
vector<TripleUInt> faces;
vector<Double> finalPoints;
Double coordinate;
map<int, map<int, int>> coordinates;
vector<Double> finalPointsArr;
Triple positionNum;
Quadruple orientationNum;
int nearNum, farNum;
float leftNum, rightNum, topNum, bottomNum;




Matrix4d WorldtoCameraMatrix(Triple positionNum, Quadruple orientationNum)
{
	Matrix4d t;
	t << 1, 0, 0, positionNum.x,
		 0, 1, 0, positionNum.y,
		 0, 0, 1, positionNum.z,
		 0, 0, 0, 1;

	Matrix4d r;
	Quadruple rot = orientationNum;

	r << pow(rot.x, 2) + (1-pow(rot.x, 2)) * cos(rot.w), rot.x * rot.y * (1 - cos(rot.w)) - rot.z * sin(rot.w), rot.x * rot.z * (1 - cos(rot.w)) + rot.y * sin(rot.w), 0,
		 rot.y * rot.x * (1 - cos(rot.w)) + rot.z * sin(rot.w), pow(rot.y, 2) + (1-pow(rot.y, 2)) * cos(rot.w), rot.y * rot.z * (1 - cos(rot.w)) - rot.x * sin(rot.w), 0,
		 rot.z * rot.x * (1 - cos(rot.w)) - rot.y * sin(rot.w), rot.z * rot.y * (1 - cos(rot.w)) + rot.x * sin(rot.w), pow(rot.z, 2) + (1-pow(rot.z, 2)) * cos(rot.w), 0,
		 0, 0, 0, 1;


//rot.z * rot.y * (1 - cos(rot.w)) - rot.x * sin(rot.w)                         should be                  rot.z * rot.y * (1 - cos(rot.w)) + rot.x * sin(rot.w)



	Matrix4d c = t * r;

	
	return c.inverse();	 	 

}



//Makes projection matrix
Matrix4d projectionMatrix(int nearNum, int farNum, float leftNum, float rightNum, float topNum, float bottomNum)
{
	Matrix4d p;
	float a = (2*nearNum)/(rightNum-leftNum);
	float b = (rightNum+leftNum)/(rightNum-leftNum);
	float c = (2*nearNum)/(topNum-bottomNum);
	float d = (topNum+bottomNum)/(topNum-bottomNum);
	float e = -(farNum+nearNum)/(farNum-nearNum);
	float f = (-2*farNum*nearNum)/(farNum-nearNum);

	p << a, 0, b, 0,
		 0, c, d, 0,
		 0, 0, e, f,
		 0, 0, -1, 0;

	return p;	 

	/*

	as the perspective projection matrix. This is the transformation matrix we use to transform a point in camera space to the homogeneous NDC. To go from homogeneous NDC 
	to Cartesian NDC, we would divide our xndc,yndc, and zndc terms by wndc=−zc. Note that sometimes the frustum we specify does not capture our entire scene, in which case,
	the points that were outside our frustum in camera space are mapped to the outside of the cube in NDC. These points are outside our field of view and should 
	not be considered for rendering. Hence, when we later render what we see, we need to check for whether a given point is outside the cube. If it is, then we do not render the point.
*/
}

//This function gives the final NDC matrix or the projection matrix and the world to camera matrix multiplied
Matrix4d finalNDC()
{

	cout << "Camera values: " << nearNum << " " << farNum << " " << leftNum << " " << rightNum << " " << topNum << " " << bottomNum << endl;
	Matrix4d p = projectionMatrix(nearNum, farNum, leftNum, rightNum, topNum, bottomNum);
	Matrix4d w = WorldtoCameraMatrix(positionNum, orientationNum);
	Matrix4d result = p * w;

	return result;
}



//This function reads the text file and saves the information
bool readTxtFile(const char * path)
{
	
	
	FILE * file = fopen(path, "r"); 
	if( file == NULL )
	{
	    printf("Impossible to open the file !\n");
	    return false;
	}

	char objName[100];

    char objFileName[100];

	vector<string> objNames;
	vector<string> objFileNames;

	vector<Triple> translations; 
	vector<Quadruple> rotations;
	vector<Triple> scaling;
	Triple trans_scale;
	Quadruple rotation;
	Matrix4d productMatrix;	
	vector<Matrix4d> matricesArr;

	LOOP: while( 1 )
	{


    	int res = fscanf(file, "%s", objName);
	    if (res == EOF)
	        break;
	   
	    cout << "objName variable: " << objName << endl;


	    if(strcmp(objName, "position") == 0)
	    {
	    	cout << "get past if statement position" << endl;
	    	fscanf(file, "%f %f %f\n", &positionNum.x, &positionNum.y, &positionNum.z);
	    	cout <<  "position: " << positionNum.x << " " <<  positionNum.y << " " <<  positionNum.z << endl;
	    }

	    else if(strcmp(objName, "orientation" ) == 0)
	    {
	    	
	    	fscanf(file, "%f %f %f %f\n", &orientationNum.x, &orientationNum.y, &orientationNum.z, &orientationNum.w);
	    	cout <<  "orientation: " << orientationNum.x << " " <<  orientationNum.y << " " <<  orientationNum.z << " " << orientationNum.z << endl;
	    }
	    
	   

	    else if(strcmp(objName, "near" ) == 0)
	    {
	    	
	    	fscanf(file, "%u\n", &nearNum);
	    	cout <<  "near: " << nearNum << endl;
	    }
		
	

	    else if(strcmp(objName, "far" ) == 0)
	    {
	    	
	    	fscanf(file, "%u\n", &farNum);
	    	cout << "far: " << farNum << endl;
	    }
	

	    else if(strcmp(objName, "left" ) == 0)
	    {
	    	
	    	fscanf(file, "%f\n", &leftNum);
	    	cout << "left: " << leftNum << endl;
	    }
	

	    else if(strcmp(objName, "right" ) == 0)
	    {
	    	fscanf(file, "%f\n", &rightNum);
	    	cout << "right: " << rightNum << endl;
	    	
	    }


	    else if(strcmp(objName, "top" ) == 0)
	    {
	    	fscanf(file, "%f\n", &topNum);
	    	cout << "top: " << topNum << endl;
	    	
	    }	  

	    else if(strcmp(objName, "bottom" ) == 0)
	    {
	    	fscanf(file, "%f\n", &bottomNum);
	    	cout << "bottom: " << bottomNum << endl;
	    	
	    }

	    else if(strlen(objName) > 1 && strcmp(objName, "objects:" ) != 0)
	    {

	    	res=fscanf(file, " %s\n", objFileName);
	    	if (strlen(objFileName) > 1)
	    	{
	    		objNames.push_back(objName);
	    		objFileNames.push_back(objFileName);
	    	}
	    	else
	    	{	

				strncpy(objName,objFileName,50);

	    		if( translations.size() != 0 || scaling.size() != 0 || rotations.size() != 0)
	    		{

	    			productMatrix = objTransProduct(translations, scaling, rotations);
	    		    matricesArr.push_back(productMatrix);

	    			translations.clear();
	    			scaling.clear();
	    			rotations.clear();
	    		}
		    }	
	    	
	    }

	    if(strcmp(objName, "t" ) == 0)

	    {
		    
		    fscanf(file, "%f %f %f\n", &trans_scale.x, &trans_scale.y, &trans_scale.z);

		    translations.push_back(trans_scale);

		}

		else if ( strcmp( objName, "s" ) == 0 )
			
		{
		   
		    
		    fscanf(file, "%f %f %f\n", &trans_scale.x, &trans_scale.y, &trans_scale.z);		    
		    scaling.push_back(trans_scale);

    	}


		else if ( strcmp( objName, "r" ) == 0 )
			
		{
		   
		    
		    fscanf(file, "%f %f %f %f\n", &rotation.x, &rotation.y, &rotation.z, &rotation.w);		    
		    rotations.push_back(rotation);


    	}
    	   
	}

	if(translations.size() != 0 || scaling.size() != 0 || rotations.size() != 0)
	{

		productMatrix = objTransProduct(translations, scaling, rotations);
	    matricesArr.push_back(productMatrix);
		translations.clear();
		scaling.clear();
		rotations.clear();
	}

	for(int k = 0; k < objFileNames.size(); k++)
	{
		cout << "object Name: " << objNames[k] << ", object File: " << objFileNames[k] << endl;
		loadObj(objFileNames[k], matricesArr[k]);
		cout << endl << endl;

	}
	

	return true;
}


//This function creates the final transformation matrix for scaling, rotation, and translation and returns it
Matrix4d objTransProduct(vector<Triple> translations, vector<Triple> scaling, vector<Quadruple> rotations)
{
	
	Matrix4d t;	
	vector<Matrix4d> proMatricesArr;
	for(int i = 0; i < translations.size(); i++)
	{
		t << 1, 0, 0, translations[i].x,
		 0, 1, 0, translations[i].y,
		 0, 0, 1, translations[i].z,
		 0, 0, 0, 1;
		proMatricesArr.push_back(t); 
	}

	Matrix4d s;
	for(int j = 0; j < scaling.size(); j++)
	{
		s << scaling[j].x, 0, 0, 0,
		 0, scaling[j].y, 0, 0,
		 0, 0, scaling[j].z, 0,
		 0, 0, 0, 1;
		proMatricesArr.push_back(s); 

	}


	Quadruple rot;	 
	Matrix4d r;// the given angle is in radians --> cosine function in c++ takes a parameter in radians
	for(int k = 0; k < rotations.size(); k++)
	{
		rot = rotations[k];
		r << pow(rot.x, 2) + (1-pow(rot.x, 2)) * cos(rot.w), rot.x * rot.y * (1 - cos(rot.w)) - rot.z * sin(rot.w), rot.x * rot.z * (1 - cos(rot.w)) + rot.y * sin(rot.w), 0,
		 rot.y * rot.x * (1 - cos(rot.w)) + rot.z * sin(rot.w), pow(rot.y, 2) + (1-pow(rot.y, 2)) * cos(rot.w), rot.y * rot.z * (1 - cos(rot.w)) - rot.x * sin(rot.w), 0,
		 rot.z * rot.x * (1 - cos(rot.w)) - rot.y * sin(rot.w), rot.z * rot.y * (1 - cos(rot.w)) + rot.x * sin(rot.w), pow(rot.z, 2) + (1-pow(rot.z, 2)) * cos(rot.w), 0,
		 0, 0, 0, 1;
		 proMatricesArr.push_back(r);
	}
	 

	cout << "t matrix: " << endl << t << endl << "s matrix: " << endl << s << endl << "r matrix: " << endl << r;
	Matrix4d m;
	for(int i = proMatricesArr.size()-1; i >= 0; i--)
	{
		if(i == proMatricesArr.size()-1)
		{
			m = proMatricesArr[i];
		}
		else
		{
			m = m * proMatricesArr[i];
		}
	}
/*
	for(int k = 0; k < proMatricesArr.size(); k++)
	{
		if(k == 0)
		{
			m = proMatricesArr[k];
		}
		else
		{
			m = m * proMatricesArr[k];
		}
	}

*/
	cout << "Final matrix: " << m << endl;
	return m;
	
}


//This function reads the object file specified in the text file for vertices and faces and creates a hashmap with the final screen coordinates
bool loadObj(string sPath, Matrix4d proMatrix)
{
	Triple vertex;
	TripleUInt face;
	int vCounter;

	const char * path;
	path = const_cast<char*>(sPath.c_str());
 // make another array storing faces
	//the above two lines show two different ways to make an array. The second one that has "std::" is not technically
	//necessary because line 10 tells the computer that we are using the std namespace already



	FILE * file = fopen(path, "r"); //open the file and read = "r"
	if( file == NULL ) //lines 32-36 are if terminal cannot open the file
	{
		cout << "Impossible to open the file " << path << endl;
	    return false;
	}
	while( 1 )//breaks at the end of the file
	{

	    char lineHeader[128];
	    // read the first word of the line
	    int res = fscanf(file, "%s", lineHeader);
	    if (res == EOF)
	        break; // EOF = End Of File. Quit the loop.

	    // else : parse lineHeader

	    if ( strcmp( lineHeader, "v" ) == 0)// if the lineheader starts with "v" (is a vector), add vertices
	    //to temporary vertices array
	    {
		    vCounter++;
		    fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
		    // & sign is a modifier. It makes sure that the variable can be changed/modified by the function
		    // It is a reference value. When you pass a reference, then you can modify it.
		    vertices.push_back(vertex);

		}

		else if ( strcmp( lineHeader, "f" ) == 0 )// if the lineheader starts with "f" (is a face), add face
			
		{
		   
		    
		    fscanf(file, "%u %u %u\n", &face.x, &face.y, &face.z);		    
		    faces.push_back(face);

    	}
    	
	}




VectorXd v(4,1);
VectorXd f(4,1);
Matrix4d NDCHomo = finalNDC();
Double finalPoint;
map<int, int> coordY;
	for(int k = 0; k < vertices.size(); k++)
	{
		
		v << vertices[k].x, vertices[k].y, vertices[k].z, 1;
		v = NDCHomo * proMatrix * v;
		double x = v(0,0)/v(3,0);
		double y = v(1,0)/v(3,0);
		if(x <= 1 && x >= -1 && y <= 1 && y >= -1)
		{
			int i = ((x-(-1))/2.0)*width;
			int j = ((y-(-1))/2.0)*height;
			cout << i << " " << j << endl;
			finalPoint.x = i;
			finalPoint.y = j;
			finalPointsArr.push_back(finalPoint);
			if(coordinates.find(i) != coordinates.end())
			{
				coordY = coordinates[i];
			}
			else
			{
				coordY.clear();
			}
			coordY[j] = j;
			coordinates[i] = coordY;
			
		}
		
	}

	return true;
	
}
//This function is used in the line algorithm
int Sign(int b)
{
	if(b < 0)
	{
		return -1;
	}
	else if(b == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


//This is the generalized line algorithm
void Bresenham(int x1, int y1, int x2, int y2)
{
	map<int, int> coordY;

	int Xs = x1;
	int Ys = y1;
	int Xe = x2;
	int Ye = y2;
	if (x1>x2){
		Xs=x2;
		Ys=y2;
		Xe=x1;
		Ye=y1;
	}
	
	int X = Xs;
	int Y = Ys;


	if(coordinates.find(X) != coordinates.end())
	{
		coordY = coordinates[X];
	}
	else
	{
		coordY.clear();
	}
		

	if(x1 == x2)
	{
		cout << "Drawing vertical line: (" << x1 << ", " << y1 <<") ("<<x2<<", "<<y2<<")" << endl;

		if(y1 > y2)
		{
			cout << "Y1 > Y2, Swapping points:" << endl;
			int T = y1;
			y1 = y2;
			y2 = T;
			cout << "Y1 > Y2, After swapping: Y1 = " << y1 << " Y2 = " << y2 << endl;
		}

		for(Y = y1; Y <= y2; Y++)
		{
			
			coordY[Y] = Y;
		}
		
		coordinates[X] = coordY;
		
		return;
	}

	
	int dx = abs(Xe-Xs);
	int dy = abs(Ye-Ys);
	int Sx = Sign(Xe-Xs);
	int Sy = Sign(Ye-Ys);
	bool interchange;

	if(dy > dx)
	{
		int T = dx;
		dx = dy;
		dy = T;
		interchange = true;
	}
	else
	{
		interchange = false;
	}
	
	int E = 2*dy - dx;
	int A = 2*dy;
	int B = 2*dy -2*dx;
	

	
	coordY[Y] = Y;
	coordinates[X] = coordY;

	for(int i = 1; i <= dx; i++)
	{
		if(E < 0)
		{
			if(interchange)
			{
				Y = Y + Sy;
			}
			else
			{
				X = X + Sx;
				
			}
			E = E + A;
		}
		else
		{
			Y = Y + Sy;
			X = X + Sx;
			E = E + B;
		}
		

		if(coordinates.find(X) != coordinates.end())
		{
			coordY = coordinates[X];

		}
		else
		{
			coordY.clear();
		}
		
		coordY[Y] = Y;
		
		coordinates[X] = coordY;
	
		
	}
	
}


//this function creates the ppm file and calls the Bresenham line algorithm to draw all faces. 
void fill()
{
	/*
	
Call algorithm:

	
	iterate through faces
	

Final print out:

	print out P3, width, height, color scale
	for loop through y and then x
	if point is in Double array, print out white
	else print out black
*/

	string drawnLine1,drawnLine2;
	map<string, string> usedpoints;
	for(int f = 0; f < faces.size(); f++)
	{

		
		drawnLine1 = to_string(faces[f].x) + "_" + to_string(faces[f].y);
		drawnLine2 = to_string(faces[f].y) + "_" + to_string(faces[f].x);
		if(usedpoints.find(drawnLine1) == usedpoints.end() && usedpoints.find(drawnLine2) == usedpoints.end())
		{
			Bresenham(finalPointsArr[faces[f].x-1].x, finalPointsArr[faces[f].x-1].y, finalPointsArr[faces[f].y-1].x,  finalPointsArr[faces[f].y-1].y);
			usedpoints[drawnLine1] = drawnLine1;
			usedpoints[drawnLine2] = drawnLine2;
			cout << "Printing line: " << f+1 << " Face:" << faces[f].x << " " << faces[f].y << endl;
			cout << "Coordinates: First point(" << finalPointsArr[faces[f].x-1].x << ", " << finalPointsArr[faces[f].x-1].y << ") Second point: (" << finalPointsArr[faces[f].y-1].x << ", " << finalPointsArr[faces[f].y-1].y << ") " << endl;

		}
		drawnLine1 = to_string(faces[f].y) + "_" + to_string(faces[f].z);
		drawnLine2 = to_string(faces[f].z) + "_" + to_string(faces[f].y);
		if(usedpoints.find(drawnLine1) == usedpoints.end() && usedpoints.find(drawnLine2) == usedpoints.end())
		{
			Bresenham(finalPointsArr[faces[f].y-1].x, finalPointsArr[faces[f].y-1].y, finalPointsArr[faces[f].z-1].x,  finalPointsArr[faces[f].z-1].y);
			usedpoints[drawnLine1] = drawnLine1;
			usedpoints[drawnLine2] = drawnLine2;
			cout << "Printing line: " << f+1 << " Face:" << faces[f].y << " " << faces[f].z << endl;
			cout << "Coordinates: First point(" << finalPointsArr[faces[f].y-1].x << ", " << finalPointsArr[faces[f].y-1].y << ") Second point: (" << finalPointsArr[faces[f].z-1].x << ", " << finalPointsArr[faces[f].z-1].y << ") " << endl;

		}	
		drawnLine1 = to_string(faces[f].x) + "_" + to_string(faces[f].z);
		drawnLine2 = to_string(faces[f].z) + "_" + to_string(faces[f].x);
		if(usedpoints.find(drawnLine1) == usedpoints.end() && usedpoints.find(drawnLine2) == usedpoints.end())
		{
			Bresenham(finalPointsArr[faces[f].x-1].x, finalPointsArr[faces[f].x-1].y, finalPointsArr[faces[f].z-1].x,  finalPointsArr[faces[f].z-1].y);
			usedpoints[drawnLine1] = drawnLine1;
			usedpoints[drawnLine2] = drawnLine2;
			cout << "Printing line: " << f+1 << " Face:" << faces[f].x << " " << faces[f].z << endl;
			cout << "Coordinates: First point(" << finalPointsArr[faces[f].x-1].x << ", " << finalPointsArr[faces[f].x-1].y << ") Second point: (" << finalPointsArr[faces[f].z-1].x << ", " << finalPointsArr[faces[f].z-1].y << ") " << endl;

		}	
	}

	
	map<int, int> coordY;
		cout << endl << endl << "printing out coordinates:" << endl;
	map<int,map<int,int>>::iterator it;
	map<int,int>::iterator it2;
	for(it=coordinates.begin(); it!=coordinates.end(); ++it)
	{
		cout << "Coordinate x = " << it->first << "has  the following y numbers: " <<endl;
		coordY = it->second;
		for(it2=coordY.begin(); it2!=coordY.end(); ++it2){
			cout << "y = " << it2->first << endl;
		}
	}







	ofstream outputFile;
	outputFile.open("3D_Object.ppm");

	outputFile << "P3 " << endl << width << " " << height << endl << " 255" << endl;
	for(int x = 0; x < width; x++)
	{
		for(int y = 0; y < height; y++)
		{


				if(coordinates.find(x) != coordinates.end())
				{
					 
					 coordY = coordinates[x];
					 if(coordY.find(y) != coordY.end())
					 {
					 	
						outputFile << "255 255 255" << endl;
					 }
					 else
					 {
					 	outputFile << "0 0 0" << endl;
					 }
					
				}
				else
				{
					outputFile << "0 0 0" << endl;
				}
			
		}
	}

	outputFile.close();

}


//this is the main function that gets the text file argument "scene_bunny1.txt" or others
int main(int argc,  char **argv)

{

	if (argc <= 1)
	{

		return 0;
	}

	
	
	for(int k = 1; k < argc; k++)
	{
		cout << k << " " << argv[k] << endl;
		
		
		readTxtFile(argv[k]);
		fill();

	}	


}







