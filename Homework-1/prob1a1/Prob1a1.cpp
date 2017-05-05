// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob1a1
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
	if (argc != 6)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <input_image>.raw <output_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "input_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "output_image: the name of the .raw file along with path to which the processesed image is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3 and for GRAY = 1)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[3]); /* number of rows in the image */	int ImgHeight = atoi(argv[4]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[5]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate OriginalImage1d in heap */
	unsigned char* OriginalImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate OriginalImage3d in heap */
	unsigned char*** OriginalImage3d = NULL;
	OriginalImage3d = AllocHeap3d(OriginalImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate MirrorImage1d in heap */
	unsigned char* MirrorImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate MirrorImage3d in heap */
	unsigned char*** MirrorImage3d = NULL;
	MirrorImage3d = AllocHeap3d(MirrorImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Read OriginalImage1d from file */
	FileRead(argv[1], OriginalImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert OriginalImage 1D to 3D so that image traversal becomes easier */
	OriginalImage3d = Img1dTo3d(OriginalImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* 1-a-1 Image mirroring logic*/
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			for (int k = 0; k < BytesPerPixel; k++)
			{
				MirrorImage3d[i][j][k] = OriginalImage3d[i][ImgWidth - 1 - j][k];
			}
		}
	}
	cout << "Image mirroring successful !!!" << endl;

	/* Convert MirrorImage 3D to 1D so that it can be written to file */
	MirrorImage1d = Img3dTo1d(MirrorImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Write MirrorImage to file */
	FileWrite(argv[2], MirrorImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Deallocate OriginalImage from heap */
	delete[] OriginalImage1d;

	/* Deallocate OriginalImage from heap */
	delete[] MirrorImage1d;

	/* deallocate OriginalImage from heap */
	DeallocHeap3d(OriginalImage3d, ImgHeight, ImgWidth);

	/* deallocate MirrorImage from heap */
	DeallocHeap3d(MirrorImage3d, ImgHeight, ImgWidth);

	return 0;
}

