// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob2b
// Date: February 5, 2017
// Name : Chinmay Chinara
// ID : 2527-2374-52
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
		cout << "<program_name> <inp_image>.raw <inp_hist>.txt <out_image>.raw <out_hist>.txt <ImgWidth> <ImgHeight> <BytesPerPixel> <method_name>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "inp_hist: the name of the .txt file along with path to to which input image histogram data is saved" << endl;
		cout << "out_image: the name of the .raw file along with path to which the processesed image is saved" << endl;
		cout << "out_hist: the name of the .txt file along with path to to which output image histogram data is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3 and for GRAY = 1)" << endl;		cout << "method_name: ma = Method-A, mb = Method-B" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[5]); /* number of rows in the image */	int ImgHeight = atoi(argv[6]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[7]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate OriginalImage1d in heap */
	unsigned char* OriginalImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate OriginalImage3d in heap */
	unsigned char*** OriginalImage3d = NULL;
	OriginalImage3d = AllocHeap3d(OriginalImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate HistEqlImage1d in heap */
	unsigned char* HistEqlImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate HistEqlImage3d in heap */
	unsigned char*** HistEqlImage3d = NULL;
	HistEqlImage3d = AllocHeap3d(HistEqlImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Read OriginalImage1d from file */
	FileRead(argv[1], OriginalImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert OriginalImage 1D to 3D so that image traversal becomes easier */
	OriginalImage3d = Img1dTo3d(OriginalImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* initialize histogram variable and store data to it from image */
	unsigned long HistData[256][3];
	Histogram3d(HistData, OriginalImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* store data for input image histogram */
	FileWriteHist3d(argv[2], HistData);

	/* Histogram equalization by linear transfer function logic */
	/* Find maximum and minimum value of input image pixel */
	if (strcmp("ma", argv[8]) == 0)
	{
		int InpMax[3] = { 0,0,0 };
		int InpMin[3] = { 255,255,255 };
		int OutMin[3] = { 0,0,0 };
		int OutMax[3] = { 255,255,255 };
		for (int i = 0; i < ImgHeight; i++)
		{
			for (int j = 0;j < ImgWidth;j++)
			{
				for (int k = 0;k < BytesPerPixel;k++)
				{
					if ((int)OriginalImage3d[i][j][k] < InpMin[k])
						InpMin[k] = (int)OriginalImage3d[i][j][k];
					if ((int)OriginalImage3d[i][j][k] > InpMax[k])
						InpMax[k] = (int)OriginalImage3d[i][j][k];
				}
			}
		}

		for (int i = 0; i < ImgHeight; i++)
		{
			for (int j = 0;j < ImgWidth;j++)
			{
				for (int k = 0;k < BytesPerPixel;k++)
				{
					double slope = (OutMax[k] - OutMin[k]) / (1.0 * (InpMax[k] - InpMin[k]));
					HistEqlImage3d[i][j][k] = (unsigned char)(slope * ((int)OriginalImage3d[i][j][k] - InpMin[k])) + OutMin[k];
				}
			}
		}
	}
	/* Histogram equalization by linear transfer function logic ends here */

	/* Histogram equalization by cumulative probability logic */
	else if (strcmp("mb", argv[8]) == 0)
	{
		double Prob[256][3];
		double CumProb[256][3];
		double NewPixelVal[256][3];
		for (int i = 0;i < 256;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				CumProb[i][j] = 0;
			}
		}

		for (int i = 0;i < 256;i++)
		{
			for (int j = 0;j < 3;j++)
			{
				HistData[i][j]; // number of pixels
				Prob[i][j] = HistData[i][j] / (1.0 * ImgHeight * ImgWidth); // Probability
				if (i == 0)
					CumProb[i][j] = Prob[i][j];
				else
					CumProb[i][j] = CumProb[i - 1][j] + Prob[i][j];
				NewPixelVal[i][j] = CumProb[i][j] * 255.0;
				NewPixelVal[i][j] = floor(NewPixelVal[i][j]);
			}
		}

		/* write new pixel values to image */
		for (int r = 0;r < 256;r++)
		{
			for (int i = 0;i < ImgHeight; i++)
			{
				for (int j = 0;j < ImgWidth; j++)
				{
					for (int k = 0; k < BytesPerPixel; k++)
					{
						if (OriginalImage3d[i][j][k] == r)
						{
							HistEqlImage3d[i][j][k] = NewPixelVal[r][k];
						}
					}
				}
			}
		}
	}
	/* Histogram equalization by cumulative probability logic ends here*/

	/* error handler */
	else
	{
		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <inp_hist>.txt <out_image>.raw <out_hist>.txt <ImgWidth> <ImgHeight> <BytesPerPixel> <method_name>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "inp_hist: the name of the .txt file along with path to to which input image histogram data is saved" << endl;
		cout << "out_image: the name of the .raw file along with path to which the processesed image is saved" << endl;
		cout << "out_hist: the name of the .txt file along with path to to which output image histogram data is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3 and for GRAY = 1)" << endl;		cout << "method_name: ma = Method-A, mb = Method-B" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;

		delete[] OriginalImage1d;
		delete[] HistEqlImage1d;
		DeallocHeap3d(OriginalImage3d, ImgHeight, ImgWidth);
		DeallocHeap3d(HistEqlImage3d, ImgHeight, ImgWidth);
		return 0;
	}

	/* initialize histogram variable and store data to it from image */
	unsigned long HistDataEq[256][3];
	Histogram3d(HistDataEq, HistEqlImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* store data for ouptput image histogram */
	FileWriteHist3d(argv[4], HistDataEq);

	/* Convert HistEqlImage3d 3D to 1D so that it can be written to file */
	HistEqlImage1d = Img3dTo1d(HistEqlImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Write MirrorImage to file */
	FileWrite(argv[3], HistEqlImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	delete[] OriginalImage1d;
	delete[] HistEqlImage1d;
	DeallocHeap3d(OriginalImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(HistEqlImage3d, ImgHeight, ImgWidth);

	return 0;
}

