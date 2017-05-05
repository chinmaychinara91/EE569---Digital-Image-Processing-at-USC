// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob1b1
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
	if (argc != 9)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <in_RGB>.raw <out_CMY>.raw <out_C>.raw <out_M>.raw <out_Y>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "in_RGB: the name of the .raw file along with path of the RGB file to be taken as input for processing" << endl;
		cout << "out_CMY: the name of the .raw file along with path to which the CMY converted image will be saved" << endl;
		cout << "out_C: the name of the .raw file along with path to which the Cyan component will be saved" << endl;
		cout << "out_M: the name of the .raw file along with path to which the Magenta component will be saved" << endl;
		cout << "out_Y: the name of the .raw file along with path to which the Yellow component will be saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (RGB = 3)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[6]); /* number of rows in the image */	int ImgHeight = atoi(argv[7]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[8]); /* bytes per pixel (RGB = 3) */

	/* Allocate RgbImage1d in heap */
	unsigned char* RgbImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate RgbImage3d in heap */
	unsigned char*** RgbImage3d = NULL;
	RgbImage3d = AllocHeap3d(RgbImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate CmyImage1d in heap */
	unsigned char* CmyImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate CmyImage3d in heap */
	unsigned char*** CmyImage3d = NULL;
	CmyImage3d = AllocHeap3d(CmyImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate CmyImage1d all components in heap */
	unsigned char* CmyImageCyan1d = new unsigned char[ImgHeight*ImgWidth]();
	unsigned char* CmyImageMagenta1d = new unsigned char[ImgHeight*ImgWidth]();
	unsigned char* CmyImageYellow1d = new unsigned char[ImgHeight*ImgWidth]();

	/* Allocate CmyImage2d all components in heap */
	unsigned char** CmyImageCyan2d = NULL;
	CmyImageCyan2d = AllocHeap2d(CmyImageCyan2d, ImgHeight, ImgWidth);
	unsigned char** CmyImageMagenta2d = NULL;
	CmyImageMagenta2d = AllocHeap2d(CmyImageMagenta2d, ImgHeight, ImgWidth);
	unsigned char** CmyImageYellow2d = NULL;
	CmyImageYellow2d = AllocHeap2d(CmyImageYellow2d, ImgHeight, ImgWidth);

	/* Read RgbImage1d from file */
	FileRead(argv[1], RgbImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert RgbImage 1D to 3D so that image traversal becomes easier */
	RgbImage3d = Img1dTo3d(RgbImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* 1-b-1 RGB to CMY conversion logic*/
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			for (int k = 0; k < BytesPerPixel; k++)
			{
				CmyImage3d[i][j][k] = (1 - (RgbImage3d[i][j][k] / 255.0)) * 255;
			}
		}
	}
	cout << "RGB to CMY converion successful !!!" << endl;

	/* Extract each component of CMY */
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			CmyImageCyan2d[i][j] = CmyImage3d[i][j][0];
			CmyImageMagenta2d[i][j] = CmyImage3d[i][j][1];
			CmyImageYellow2d[i][j] = CmyImage3d[i][j][2];
		}
	}
	cout << "C,M,Y components of CMY extracted successfully !!!" << endl;

	/* Convert Image from 2D/3D to 1D so that it can be written to file */
	CmyImage1d = Img3dTo1d(CmyImage3d, ImgHeight, ImgWidth, BytesPerPixel);
	CmyImageCyan1d = Img2dTo1d(CmyImageCyan2d, ImgHeight, ImgWidth);
	CmyImageMagenta1d = Img2dTo1d(CmyImageMagenta2d, ImgHeight, ImgWidth);
	CmyImageYellow1d = Img2dTo1d(CmyImageYellow2d, ImgHeight, ImgWidth);

	/* Write Image to file */
	FileWrite(argv[2], CmyImage1d, ImgHeight*ImgWidth*BytesPerPixel);
	FileWrite(argv[3], CmyImageCyan1d, ImgHeight*ImgWidth);
	FileWrite(argv[4], CmyImageMagenta1d, ImgHeight*ImgWidth);
	FileWrite(argv[5], CmyImageYellow1d, ImgHeight*ImgWidth);

	/* Deallocate Image from heap */
	delete[] RgbImage1d;
	DeallocHeap3d(RgbImage3d, ImgHeight, ImgWidth);
	delete[] CmyImage1d;
	DeallocHeap3d(CmyImage3d, ImgHeight, ImgWidth);
	delete[] CmyImageCyan1d;
	delete[] CmyImageMagenta1d;
	delete[] CmyImageYellow1d;
	DeallocHeap2d(CmyImageCyan2d, ImgHeight);
	DeallocHeap2d(CmyImageMagenta2d, ImgHeight);
	DeallocHeap2d(CmyImageYellow2d, ImgHeight);

	return 0;
}

