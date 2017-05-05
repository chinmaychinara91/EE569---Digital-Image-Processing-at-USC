// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob1c
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
		cout << "<program_name> <input_image1>.raw <input_image2>.raw <output_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "input_image1: the name of the .raw file along with path which has the top layer" << endl;
		cout << "input_image2: the name of the .raw file along with path which has the bottom layer" << endl;
		cout << "output_image: the name of the .raw file along with path to which the processesed image is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[4]); /* number of rows in the image */	int ImgHeight = atoi(argv[5]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[6]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate TopImage1d in heap */
	unsigned char* TopImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate TopImage3d in heap */
	unsigned char*** TopImage3d = NULL;
	TopImage3d = AllocHeap3d(TopImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate BottomImage1d in heap */
	unsigned char* BottomImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate BottomImage3d in heap */
	unsigned char*** BottomImage3d = NULL;
	BottomImage3d = AllocHeap3d(BottomImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate BlendImage1d in heap */
	unsigned char* BlendImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate BlendImage3d in heap */
	unsigned char*** BlendImage3d = NULL;
	BlendImage3d = AllocHeap3d(BlendImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Read from file */
	FileRead(argv[1], TopImage1d, ImgHeight*ImgWidth*BytesPerPixel);
	FileRead(argv[2], BottomImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert 1D to 3D so that image traversal becomes easier */
	TopImage3d = Img1dTo3d(TopImage1d, ImgHeight, ImgWidth, BytesPerPixel);
	BottomImage3d = Img1dTo3d(BottomImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* 1-c Image Multiply blending logic*/
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			for (int k = 0; k < BytesPerPixel; k++)
			{
				BlendImage3d[i][j][k] = ((TopImage3d[i][j][k] / 255.0) * (BottomImage3d[i][j][k] / 255.0)) * 255;
			}
		}
	}
	cout << "Image blending by Multiply blending method successful !!!" << endl;

	/* Convert 3D to 1D so that it can be written to file */
	BlendImage1d = Img3dTo1d(BlendImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Write MirrorImage to file */
	FileWrite(argv[3], BlendImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Deallocate from heap */
	delete[] TopImage1d;
	delete[] BottomImage1d;
	delete[] BlendImage1d;
	DeallocHeap3d(TopImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(BottomImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(BlendImage3d, ImgHeight, ImgWidth);

	return 0;
}

