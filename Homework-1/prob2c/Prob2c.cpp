// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob2c
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
	if (argc != 8)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <inp_hist>.txt <out_image>.raw <out_hist>.txt <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "inp_hist: the name of the .txt file along with path to which the input image histogram data is saved" << endl;
		cout << "out_image: the name of the .raw file along with path for storing histogram matched image" << endl;
		cout << "out_hist: the name of the .txt file along with path to which the histogram matched image histogram data is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[5]); /* number of rows in the image */	int ImgHeight = atoi(argv[6]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[7]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate OriginalImage1d in heap */
	unsigned char* OriginalImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate OriginalImage3d in heap */
	unsigned char*** OriginalImage3d = NULL;
	OriginalImage3d = AllocHeap3d(OriginalImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate GaussImage1d in heap */
	unsigned char* GaussImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate GaussImage3d in heap */
	unsigned char*** GaussImage3d = NULL;
	GaussImage3d = AllocHeap3d(GaussImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate HistMatchImage1d in heap */
	unsigned char* HistMatchImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate HistMatchImage3d in heap */
	unsigned char*** HistMatchImage3d = NULL;
	HistMatchImage3d = AllocHeap3d(HistMatchImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate HistEqlImage3d in heap */
	unsigned char*** HistEqlImage3d = NULL;
	HistEqlImage3d = AllocHeap3d(HistEqlImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Read OriginalImage1d from file */
	FileRead(argv[1], OriginalImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert OriginalImage 1D to 3D so that image traversal becomes easier */
	OriginalImage3d = Img1dTo3d(OriginalImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* store histogram data for input image */
	unsigned long HistInp[256][3];
	Histogram3d(HistInp, OriginalImage3d, ImgHeight, ImgWidth, BytesPerPixel);
	FileWriteHist3d(argv[2], HistInp);

	/* Histogram matching logic */
	double ProbInp[256][3];
	double ProbGauss[256][3];
	double CumProbInp[256][3];
	double CumProbGauss[256][3];
	double NewPixelValInp[256][3];
	double NewPixelValGauss[256][3];
	double NewPixelMap[256][3];
	float mu = 70.0;
	float sigma = 20.0;
	float pi = 3.14159265;
	for (int i = 0;i < 256;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			CumProbInp[i][j] = 0;
			CumProbGauss[i][j] = 0;
		}
	}

	for (int i = 0;i < 256;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			HistInp[i][j]; // number of pixels
			ProbInp[i][j] = HistInp[i][j] / (1.0 * ImgHeight * ImgWidth); // Probability
			ProbGauss[i][j] = (1.0 / sqrt(2 * pi* pow(sigma,2))) * exp(-pow((i - mu), 2) / (2.0*pow(sigma, 2))); // Probability
			if (i == 0)
			{
				CumProbInp[i][j] = ProbInp[i][j];
				CumProbGauss[i][j] = ProbGauss[i][j];
			}
			else
			{
				CumProbInp[i][j] = CumProbInp[i - 1][j] + ProbInp[i][j];
				CumProbGauss[i][j] = CumProbGauss[i - 1][j] + ProbGauss[i][j];
			}
			NewPixelValInp[i][j] = CumProbInp[i][j] * 255.0;
			NewPixelValInp[i][j] = floor(NewPixelValInp[i][j]);
			NewPixelValGauss[i][j] = CumProbGauss[i][j] * 255.0;
			NewPixelValGauss[i][j] = floor(NewPixelValGauss[i][j]);
		}
	}

	/* store the histogram data of the Gaussian PDF */
	FILE *file;
	file = fopen("gaussian_pdf_hist.txt", "w");
	if (file != NULL)
	{
		for (int i = 0; i < 256;i++)
		{
			fprintf(file, "%d\t%f\t%f\t%f\n", i, ProbGauss[i][0], ProbGauss[i][1], ProbGauss[i][2]);
		}
		fclose(file);
		cout << "File " << "gaussian_pdf_hist.txt" << " written successfully !!!" << endl;
	}

	for (int r = 0;r < 256;r++)
	{
		for (int k = 0; k < BytesPerPixel; k++)
		{
			double tempInp = NewPixelValInp[r][k];
			int t = 0;
			for (int c2 = 0; c2 < 256; c2++)
			{
				double tempGauss = NewPixelValGauss[c2][k];
				if (tempGauss >= tempInp && t==0)
				{
					NewPixelMap[r][k] = c2;
					t = 1;
				}
			}			 
		}
	}

	/* write new pixel values to image */
	for (int i = 0;i < ImgHeight;i++)
	{
		for (int j = 0;j < ImgWidth;j++)
		{
			for (int k = 0;k < BytesPerPixel;k++)
			{
				HistMatchImage3d[i][j][k] = NewPixelMap[OriginalImage3d[i][j][k]][k];
			}
		}
	}
	/* Histogram matching logic ends here*/

	/* store histogram mathched data */
	unsigned long HistMatch[256][3];
	Histogram3d(HistMatch, HistMatchImage3d, ImgHeight, ImgWidth, BytesPerPixel);
	FileWriteHist3d(argv[4], HistMatch);

	/* Convert images 3D to 1D so that it can be written to file */
	HistMatchImage1d = Img3dTo1d(HistMatchImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Write MirrorImage to file */
	FileWrite(argv[3], HistMatchImage1d	, ImgHeight*ImgWidth*BytesPerPixel);

	delete[] OriginalImage1d;
	delete[] GaussImage1d;
	delete[] HistMatchImage1d;
	DeallocHeap3d(OriginalImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(GaussImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(HistMatchImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(HistEqlImage3d, ImgHeight, ImgWidth);

	return 0;
}

