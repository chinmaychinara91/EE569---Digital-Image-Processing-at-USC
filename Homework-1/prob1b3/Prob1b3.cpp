// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob1b3
// Date: February 5, 2017
// Name : Chinmay Chinara
// USC-ID : 2527-2374-52
// email : chinara@usc.edu
// ------------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <algorithm>
#include "MyImgHeaders.h"

using namespace std;

int main(int argc, char* argv[])
{
	/* condition to ensure that correct parameters are passed in the command line */
	if (argc != 7)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <in_RGB>.raw <out_gray>.raw <out_sepia>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "in_RGB: the name of the .raw file along with path of the RGB file to be taken as input for processing" << endl;
		cout << "out_gray: the name of the .raw file along with path to which the gray image is saved" << endl;
		cout << "out_sepia: the name of the .raw file along with path to which the sepia filtered image is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[4]); /* number of rows in the image */	int ImgHeight = atoi(argv[5]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[6]); /* bytes per pixel (RGB = 3) */

	/* Allocate RgbImage1d in heap */
	unsigned char* RgbImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate RgbImage3d in heap */
	unsigned char*** RgbImage3d = NULL;
	RgbImage3d = AllocHeap3d(RgbImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate GrayImage1d in heap */
	unsigned char* GrayImage1d = new unsigned char[ImgHeight*ImgWidth]();

	/* Allocate GrayImage2d in heap */
	unsigned char** GrayImage2d = NULL;
	GrayImage2d = AllocHeap2d(GrayImage2d, ImgHeight, ImgWidth);

	/* Allocate SepiaImage1d in heap */
	unsigned char* SepiaImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate SepiaImage3d in heap */
	unsigned char*** SepiaImage3d = NULL;
	SepiaImage3d = AllocHeap3d(SepiaImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Read RgbImage1d from file */
	FileRead(argv[1], RgbImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert RgbImage 1D to 3D so that image traversal becomes easier */
	RgbImage3d = Img1dTo3d(RgbImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* 1-b-3 RGB to Gray conversion logic*/
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			GrayImage2d[i][j] = (unsigned char)(0.21 * (double)RgbImage3d[i][j][0] + 0.72 * (double)RgbImage3d[i][j][1] + 0.07 * (double)RgbImage3d[i][j][2]);
		}
	}
	cout << "RGB to Gray converion successful !!!" << endl;

	/* Using sepia filter over gray image */
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			double SfR = 0.393 * (double)GrayImage2d[i][j] + 0.769 * (double)GrayImage2d[i][j] + 0.189 * (double)GrayImage2d[i][j];
			double SfG = 0.349 * (double)GrayImage2d[i][j] + 0.686 * (double)GrayImage2d[i][j] + 0.168 * (double)GrayImage2d[i][j];
			double SfB = 0.272 * (double)GrayImage2d[i][j] + 0.534 * (double)GrayImage2d[i][j] + 0.131 * (double)GrayImage2d[i][j];
			
			if (SfR < 0.0)
				SepiaImage3d[i][j][0] = (unsigned char)0.0;
			else if (SfR > 255.0)
				SepiaImage3d[i][j][0] = (unsigned char)255.0;
			else
				SepiaImage3d[i][j][0] = (unsigned char)SfR;
			
			if (SfG < 0.0)
				SepiaImage3d[i][j][1] = (unsigned char)0.0;
			else if (SfG > 255.0)
				SepiaImage3d[i][j][1] = (unsigned char)255.0;
			else
				SepiaImage3d[i][j][1] = (unsigned char)SfG;
			
			if (SfB < 0.0)
				SepiaImage3d[i][j][2] = (unsigned char)0.0;
			else if (SfB > 255.0)
				SepiaImage3d[i][j][2] = (unsigned char)255.0;
			else
				SepiaImage3d[i][j][2] = (unsigned char)SfB;
		}
	}
	cout << "Sepia filter applied successfully !!!" << endl;

	/* Convert Image from 2D/3D to 1D so that it can be written to file */
	GrayImage1d = Img2dTo1d(GrayImage2d, ImgHeight, ImgWidth);
	SepiaImage1d = Img3dTo1d(SepiaImage3d, ImgHeight, ImgWidth, BytesPerPixel);
	
	/* Write Image to file */
	FileWrite(argv[2], GrayImage1d, ImgHeight*ImgWidth);
	FileWrite(argv[3], SepiaImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Deallocate Image from heap */
	delete[] RgbImage1d;
	DeallocHeap3d(RgbImage3d, ImgHeight, ImgWidth);
	delete[] GrayImage1d;
	DeallocHeap2d(GrayImage2d, ImgHeight);
	delete[] SepiaImage1d;
	DeallocHeap3d(SepiaImage3d, ImgHeight, ImgWidth);

	return 0;
}

