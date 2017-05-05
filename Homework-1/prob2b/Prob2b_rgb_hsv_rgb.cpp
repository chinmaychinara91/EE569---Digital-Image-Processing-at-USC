// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob2b_rgb_hsv_rgb
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
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "inp_hist: the name of the .txt file along with path to to which input image histogram data is saved" << endl;
		cout << "out_image: the name of the .raw file along with path to which the processesed image is saved" << endl;
		cout << "out_hist: the name of the .txt file along with path to to which output image histogram data is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3 and for GRAY = 1)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[5]); /* number of rows in the image */	int ImgHeight = atoi(argv[6]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[7]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate RgbImage1d in heap */
	unsigned char* RgbImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate RgbImage3d in heap */
	unsigned char*** RgbImage3d = NULL;
	RgbImage3d = AllocHeap3d(RgbImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate RgbImageNew1d in heap */
	unsigned char* RgbImageNew1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate RgbImageNew3d in heap */
	unsigned char*** RgbImageNew3d = NULL;
	RgbImageNew3d = AllocHeap3d(RgbImageNew3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate HsvImage2d all components in heap */
	unsigned char** HsvImageH2d = NULL;
	HsvImageH2d = AllocHeap2d(HsvImageH2d, ImgHeight, ImgWidth);
	unsigned char** HsvImageS2d = NULL;
	HsvImageS2d = AllocHeap2d(HsvImageS2d, ImgHeight, ImgWidth);
	unsigned char** HsvImageV2d = NULL;
	HsvImageV2d = AllocHeap2d(HsvImageV2d, ImgHeight, ImgWidth);
	unsigned char** HistEqlV2d = NULL;
	HistEqlV2d = AllocHeap2d(HistEqlV2d, ImgHeight, ImgWidth);

	/* Read OriginalImage1d from file */
	FileRead(argv[1], RgbImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert OriginalImage 1D to 3D so that image traversal becomes easier */
	RgbImage3d = Img1dTo3d(RgbImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* initialize histogram variable and store data to it from image */
	unsigned long HistData[256][3];
	Histogram3d(HistData, RgbImage3d, ImgHeight, ImgWidth, BytesPerPixel);
	FileWriteHist3d(argv[2], HistData);

	/* Convert RGB to HSV for the pupose of equalisation */
	/* Source for conversion formula: http://www.rapidtables.com/convert/color/rgb-to-hsv.htm */
	double CMax = 0.0;
	double CMin = 0.0;
	double Delta = 0.0;
	double H = 0.0;
	double S = 0.0;
	double V = 0.0;

	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			double R = (double)RgbImage3d[i][j][0] / 255.0;
			double G = (double)RgbImage3d[i][j][1] / 255.0;
			double B = (double)RgbImage3d[i][j][2] / 255.0;

			CMax = max(max(R, G), B);
			CMin = min(min(R, G), B);
			Delta = CMax - CMin;

			if (Delta == 0.0)
			{
				H = 0;
			}

			if (CMax == R)
			{
				H = 60 * (fmod((G - B) / Delta, 6));
			}

			else if (CMax == G)
			{
				H = 60 * (((B - R) / Delta) + 2);
			}

			else if (CMax == B)
			{
				H = 60 * (((R - G) / Delta) + 4);
			}

			if (CMax == 0.0)
				S = 0;
			else
				S = Delta / CMax;

			V = CMax;

			HsvImageH2d[i][j] = (H / 360) * 255;
			HsvImageS2d[i][j] = S * 255;
			HsvImageV2d[i][j] = V * 255;

			/* renormalizing */
			if (HsvImageH2d[i][j] < 0)
				HsvImageH2d[i][j] = 0;
			else if (HsvImageH2d[i][j] > 255)
				HsvImageH2d[i][j] = 255;

			if (HsvImageS2d[i][j] < 0)
				HsvImageS2d[i][j] = 0;
			else if (HsvImageS2d[i][j] > 255)
				HsvImageS2d[i][j] = 255;

			if (HsvImageV2d[i][j] < 0)
				HsvImageV2d[i][j] = 0;
			else if (HsvImageV2d[i][j] > 255)
				HsvImageV2d[i][j] = 255;
		}
	}
	cout << "RGB to HSV converion successful !!!" << endl;

	/* store data for histogram of V component to be used for equalisation */
	unsigned long HistDataV[256];
	Histogram2d(HistDataV, HsvImageV2d, ImgHeight, ImgWidth);

	/* Histogram equalization by cumulative probability logic for V component */
	double Prob[256];
	double CumProb[256];
	double NewPixelVal[256];
	for (int i = 0;i < 256;i++)
	{
		CumProb[i] = 0;
	}

	for (int i = 0;i < 256;i++)
	{
		HistDataV[i]; // number of pixels
		Prob[i] = HistDataV[i] / (1.0 * ImgHeight * ImgWidth); // Probability
		if (i == 0)
			CumProb[i] = Prob[i];
		else
			CumProb[i] = CumProb[i - 1] + Prob[i];
		NewPixelVal[i] = CumProb[i] * 255.0;
		NewPixelVal[i] = floor(NewPixelVal[i]);
	}

	/* write new pixel values to image */
	for (int i = 0;i < ImgHeight; i++)
	{
		for (int j = 0;j < ImgWidth; j++)
		{
			HistEqlV2d[i][j] = NewPixelVal[HsvImageV2d[i][j]];
		}
	}
	/* Histogram equalization by cumulative probability logic ends here*/

	/* Convert HSV to RGB for the pupose of equalisation */
	/* Source for conversion formula: http://www.rapidtables.com/convert/color/hsv-to-rgb.htm */
	double R = 0.0;
	double G = 0.0;
	double B = 0.0;

	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			double H = ((double)HsvImageH2d[i][j] * 360) / 255.0;
			double S = (double)HsvImageS2d[i][j] / 255.0;
			double V = (double)HistEqlV2d[i][j] / 255.0;
			double C = V * S;
			double X = C * (1 - abs(fmod((H / 60.0), 2) - 1));
			double m = V - C;

			if (H >= 0 && H < 60)
			{
				R = C;
				G = X;
				B = 0;
			}

			if (H >= 60 && H < 120)
			{
				R = X;
				G = C;
				B = 0;
			}

			if (H >= 120 && H < 180)
			{
				R = 0;
				G = C;
				B = X;
			}

			if (H >= 180 && H < 240)
			{
				R = 0;
				G = X;
				B = C;
			}

			if (H >= 240 && H < 300)
			{
				R = X;
				G = 0;
				B = C;
			}

			if (H >= 300 && H < 360)
			{
				R = C;
				G = 0;
				B = X;
			}

			RgbImageNew3d[i][j][0] = (unsigned char)((R + m) * 255);
			RgbImageNew3d[i][j][1] = (unsigned char)((G + m) * 255);
			RgbImageNew3d[i][j][2] = (unsigned char)((B + m) * 255);

			/* renormalizing */
			if (RgbImageNew3d[i][j][0] < 0)
				RgbImageNew3d[i][j][0] = 0;
			else if (RgbImageNew3d[i][j][0] > 255)
				RgbImageNew3d[i][j][0] = 255;

			if (RgbImageNew3d[i][j][1] < 0)
				RgbImageNew3d[i][j][1] = 0;
			else if (RgbImageNew3d[i][j][1] > 255)
				RgbImageNew3d[i][j][1] = 255;

			if (RgbImageNew3d[i][j][2] < 0)
				RgbImageNew3d[i][j][2] = 0;
			else if (RgbImageNew3d[i][j][2] > 255)
				RgbImageNew3d[i][j][2] = 255;
		}
	}
	cout << "HSV to RGB converion successful !!!" << endl;

	/* initialize histogram variable and store data to it from image */
	unsigned long HistDataEq[256][3];
	Histogram3d(HistDataEq, RgbImageNew3d, ImgHeight, ImgWidth, BytesPerPixel);
	FileWriteHist3d(argv[4], HistDataEq);

	/* Convert RgbImage3d 3D to 1D so that it can be written to file */
	RgbImageNew1d = Img3dTo1d(RgbImageNew3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Write RgbImage1d to file */
	FileWrite(argv[3], RgbImageNew1d, ImgHeight*ImgWidth*BytesPerPixel);

	delete[] RgbImage1d;
	delete[] RgbImageNew1d;
	DeallocHeap2d(HsvImageH2d, ImgHeight);
	DeallocHeap2d(HsvImageS2d, ImgHeight);
	DeallocHeap2d(HsvImageV2d, ImgHeight);
	DeallocHeap2d(HistEqlV2d, ImgHeight);
	DeallocHeap3d(RgbImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(RgbImageNew3d, ImgHeight, ImgWidth);

	return 0;
}

