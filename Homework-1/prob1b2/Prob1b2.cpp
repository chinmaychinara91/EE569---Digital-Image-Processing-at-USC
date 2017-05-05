// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob1b2
// Date: February 5, 2017
// Name : Chinmay Chinara
// ID : 2527-2374-52
// email : chinara@usc.edu
// ------------------------------------------------------------------------------------

#include <iostream>
#include <algorithm> 
#include "MyImgHeaders.h"
#include <cmath>

using namespace std;

int main(int argc, char* argv[])
{
	/* condition to ensure that correct parameters are passed in the command line */
	if (argc != 8)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <in_RGB>.raw <out_H>.raw <out_S>.raw <out_L>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "in_RGB: the name of the .raw file along with path of the RGB file to be taken as input for processing" << endl;
		cout << "out_H: the name of the .raw file along with path to which the H component of image will be saved" << endl;
		cout << "out_S: the name of the .raw file along with path to which the S component of image will be saved" << endl;
		cout << "out_L: the name of the .raw file along with path to which the L component of image will be saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (RGB = 3)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[5]); /* number of rows in the image */	int ImgHeight = atoi(argv[6]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[7]); /* bytes per pixel (RGB = 3) */

	/* Allocate RgbImage1d in heap */
	unsigned char* RgbImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate RgbImage3d in heap */
	unsigned char*** RgbImage3d = NULL;
	RgbImage3d = AllocHeap3d(RgbImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate HslImageH1d all components in heap */
	unsigned char* HslImageH1d = new unsigned char[ImgHeight*ImgWidth]();
	unsigned char* HslImageS1d = new unsigned char[ImgHeight*ImgWidth]();
	unsigned char* HslImageL1d = new unsigned char[ImgHeight*ImgWidth]();

	/* Allocate HslImage2d all components in heap */
	unsigned char** HslImageH2d = NULL;
	HslImageH2d = AllocHeap2d(HslImageH2d, ImgHeight, ImgWidth);
	unsigned char** HslImageS2d = NULL;
	HslImageS2d = AllocHeap2d(HslImageS2d, ImgHeight, ImgWidth);
	unsigned char** HslImageL2d = NULL;
	HslImageL2d = AllocHeap2d(HslImageL2d, ImgHeight, ImgWidth);

	/* Read RgbImage1d from file */
	FileRead(argv[1], RgbImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert RgbImage 1D to 3D so that image traversal becomes easier */
	RgbImage3d = Img1dTo3d(RgbImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* 1-b-2 RGB to HSL conversion logic*/
	double M = 0.0;
	double m = 0.0;
	double C = 0.0;
	double temp = 0.0;

	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			double R = (double)RgbImage3d[i][j][0] / 255.0;
			double G = (double)RgbImage3d[i][j][1] / 255.0;
			double B = (double)RgbImage3d[i][j][2] / 255.0;
			
			M = max(R, max(G, B));
			m = min(R, min(G, B));
			C = M - m;

			if (C == 0.0)
			{
				HslImageH2d[i][j] = (unsigned char)0;
			}

			if (M == R)
			{
				HslImageH2d[i][j] = (unsigned char)(((60 * (fmod(((G - B) / C), 6))) / 360.0) * 255.0);
			} 

			if (M == G)
			{
				HslImageH2d[i][j] = (unsigned char)(((60 * (((B - R) / C) + 2)) / 360.0) * 255.0);
			}

			if (M == B)
			{
				HslImageH2d[i][j] = (unsigned char)(((60 * (((R - G) / C) + 4)) / 360.0) * 255.0);
			}

			temp = (M + m) / 2.0;
			HslImageL2d[i][j] = (unsigned char)(((M + m) / 2.0) * 255);

			if (temp == 0.0 || temp == 1.0)
			{
				HslImageS2d[i][j] = (unsigned char)0; 
			}

			else if (temp > 0 && temp < 0.5)
			{
				HslImageS2d[i][j] = (unsigned char)((C / (2.0 * temp)) *255);
			}

			else
			{
				HslImageS2d[i][j] = (unsigned char)((C / (2 - (2 * temp))) *255);
			}

			/* renormalizing */
			if (HslImageH2d[i][j] < (unsigned char)0)
				HslImageH2d[i][j] = (unsigned char)0;
			else if (HslImageH2d[i][j] > (unsigned char)255)
				HslImageH2d[i][j] = (unsigned char)255;
			
			if (HslImageS2d[i][j] < (unsigned char)0)
				HslImageS2d[i][j] = (unsigned char)0;
			else if (HslImageS2d[i][j] > (unsigned char)255)
				HslImageS2d[i][j] = (unsigned char)255;
			
			if (HslImageL2d[i][j] < (unsigned char)0)
				HslImageL2d[i][j] = (unsigned char)0;
			else if (HslImageL2d[i][j] > (unsigned char)255)
				HslImageL2d[i][j] = (unsigned char)255;
		}
	}
	cout << "RGB to HSL converion successful !!!" << endl;

	/* Convert Image from 2D/3D to 1D so that it can be written to file */
	HslImageH1d = Img2dTo1d(HslImageH2d, ImgHeight, ImgWidth);
	HslImageL1d = Img2dTo1d(HslImageL2d, ImgHeight, ImgWidth);
	HslImageS1d = Img2dTo1d(HslImageS2d, ImgHeight, ImgWidth);

	/* Write Image to file */
	FileWrite(argv[2], HslImageH1d, ImgHeight*ImgWidth);
	FileWrite(argv[3], HslImageS1d, ImgHeight*ImgWidth);
	FileWrite(argv[4], HslImageL1d, ImgHeight*ImgWidth);

	/* Deallocate Image from heap */
	delete[] RgbImage1d;
	DeallocHeap3d(RgbImage3d, ImgHeight, ImgWidth);
	delete[] HslImageH1d;
	delete[] HslImageS1d;
	delete[] HslImageL1d;
	DeallocHeap2d(HslImageH2d, ImgHeight);
	DeallocHeap2d(HslImageS2d, ImgHeight);
	DeallocHeap2d(HslImageL2d, ImgHeight);

	return 0;
}

