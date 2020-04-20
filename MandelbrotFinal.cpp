

//Mandelrot Version 3


#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
//#include "Eigen/Dense"
#include <math.h>
//#include <ComplexClass.cpp>

//using namespace Eigen;
using namespace std;

class Complex
{

private:
	double real, imag; 

	//constructors
public:
	Complex(double r, double i)
	{
		real = r;
		imag = i;
	}


	Complex()
	{
		real = imag = 0;
	}

	double getReal()
	{
		return real;
	}

	double getImag()
	{
		return imag;
	}

	void setReal(double re)
	{
		real = re;
	}

	void setImag(double im)
	{
		imag = im;
	}

	
	//addition + subtraction

	Complex add(Complex k)
	{
		Complex Sum;
		Sum.real = real + k.real;
		Sum.imag = imag + k.imag;
		return Sum;
	}

	Complex addNum(double z)
	{
		Complex sum;
		sum.real = real + z;
		sum.imag = imag;
		return sum;
	}

	Complex subtract(Complex p)
	{
		Complex Diff;
		Diff.real = real - p.real;
		Diff.imag = imag - p.imag;
		return Diff;

	}

	//multiplication + division(don't really need division)

	Complex multiply(Complex c)
	{
		Complex product;
		product.real = (real * c.real) - (imag * c.imag);
		product.imag = (real * c.imag) + (c.real * imag);
		return product;
	}

	Complex multiplyNum(double z)
	{
		Complex prod;
		prod.real = real*z;
		prod.imag = imag*z;
		return prod;
	}


	Complex divide(Complex k)
	{
		Complex quotient;
		Complex conj(k.real, -(k.imag));
		quotient.real = ((real * conj.real) - (imag * conj.imag))/((conj.real * k.real) - (conj.imag * k.imag));
		quotient.imag = ((real * conj.imag) + (conj.real * imag))/((conj.real * k.real) - (conj.imag * k.imag));
		return quotient;
	}


	//power of

	Complex power(int pow, Complex c)
	{
		
		for(int k = 0; k < pow-1; k++)
		{
			c = c.multiply(c);
		}
		return c;
	}

	double distance()
	{
		return sqrt((pow(real, 2) + pow(imag, 2)));
	}


	void print()
	{
		cout << real << " + " << imag << "i" << endl;
	}

};






//***************************START OF MANDELBROT CODE WITH PARAMETERS************************\\


#include <fstream>
#include <iostream>

float width = 1000;//width and height of the imag - I might make this a parameter later so the user can get whatever size they want
float height = 1000;


struct Triple
{
	int r;
	int g;
	int b;
};


Triple color(int x, int y, Triple inside, Triple outside, Triple rim, int numIter)//Separate function for finding the color of each coordinate
{
	
	Triple color;
	Complex coord((float)y/(0.5*width)-1.5, (float)x/(0.5*height)-1);//coordinate we are looking at. it is y first and then x because of the same reason in HW0P4
	Complex z(0, 0);//the z number
	int iterator = 0;
	while(z.distance() < 2 && iterator <= numIter)//if the equation is less than 2 and you haven't iterated more than 100 times, iterate again and add to iterator variable
	{
		z = z.power(2, z).add(coord);
		iterator++;
	}
	if(iterator <= numIter/2)//if it diverged before 50 iterations, color it blue
	{
		return outside;
	}
	else if(iterator > numIter/2 && iterator < numIter)//if it was more than 50 and less than 100, return -1 so that it is colored white
	{
		return rim;
	}
	else
	{
		return inside;
	}

}

int main(int argc,  char **argv)
{

	Triple inside;
	inside.r = atoi(argv[1]);
	inside.g = atoi(argv[2]);
	inside.b = atoi(argv[3]);
	Triple rim;
	rim.r = atoi(argv[4]);
	rim.g = atoi(argv[5]);
	rim.b = atoi(argv[6]);
	Triple outside;
	outside.r = atoi(argv[7]);
	outside.g = atoi(argv[8]);
	outside.b = atoi(argv[9]);

	int numIter = atoi(argv[10]); 


	ofstream image;//make a ppm image file
	image.open("Mandelbrot.ppm");//open the file
	image << "P3"  << endl << width << " " << height << endl << "255" << endl;//these are the starting lines of the ppm image(P3, size, and opacity)
	
		for(int i = 0; i < width; i++)
		{
			for(int j = 0; j < height; j++)
			{
				Triple colors = color(i, j, inside, outside, rim, numIter);
				
				
				image << colors.r << " " << colors.g  << " " << colors.b << endl;//else color it black or some form of blue depending on what "col" is
				
				
			}
		}
		image.close();//close the file
	
	
	return 0;

}

