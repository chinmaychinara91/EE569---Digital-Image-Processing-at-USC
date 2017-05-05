// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob2a
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
	if (argc != 11 && strcmp(argv[8],"mb")==0 || strcmp(argv[5], " ")==0 || strcmp(argv[5], " ") == 0 || strcmp(argv[5], " ") == 0)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <inp_hist>.txt <out_image>.raw <out_hist>.txt <ImgWidth> <ImgHeight> <BytesPerPixel> <method_name> <tfdata_filename>.txt <tf_pixelmapping_mb>.txt" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "inp_hist: the name of the .txt file along with path to to which input image histogram data is saved" << endl;
		cout << "out_image: the name of the .raw file along with path to which the processesed image is saved" << endl;
		cout << "out_hist: the name of the .txt file along with path to to which output image histogram data is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3 and for GRAY = 1)" << endl;		cout << "method_name: ma = Method-A, mb = Method-B" << endl;		cout << "tfdata_filename: the name of the .txt file along with path to to which transfer function data is saved" << endl;		cout << "tf_pixelmapping_mb: the name of the .txt file along with path to to which pixel mapped transfer function data for Method-B is saved (this parameter is only required for Method-B)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[5]); /* number of rows in the image */	int ImgHeight = atoi(argv[6]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[7]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate OriginalImage1d in heap */
	unsigned char* OriginalImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate OriginalImage2d in heap */
	unsigned char** OriginalImage2d = NULL;
	OriginalImage2d = AllocHeap2d(OriginalImage2d, ImgHeight, ImgWidth);

	/* Allocate HistEqlImage1d in heap */
	unsigned char* HistEqlImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate HistEqlImage3d in heap */
	unsigned char** HistEqlImage2d = NULL;
	HistEqlImage2d = AllocHeap2d(HistEqlImage2d, ImgHeight, ImgWidth);

	/* Read OriginalImage1d from file */
	FileRead(argv[1], OriginalImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert OriginalImage 1D to 2D so that image traversal becomes easier */
	OriginalImage2d = Img1dTo2d(OriginalImage1d, ImgHeight, ImgWidth);

	/* initialize histogram variable and store data to it from image */
	unsigned long HistData[256];
	Histogram2d(HistData, OriginalImage2d, ImgHeight, ImgWidth);

	/* store data for input image histogram */
	FileWriteHist2d(argv[2], HistData);

	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	/* Histogram equalization by linear transfer function logic */
	/* Find maximum and minimum value of input image pixel */
	if (strcmp("ma", argv[8]) == 0)
	{
		int InpMax = 0;
		int InpMin = 255;
		int OutMin = 0;
		int OutMax = 255;
		for (int i = 0; i < ImgHeight; i++)
		{
			for (int j = 0;j < ImgWidth;j++)
			{
				if ((int)OriginalImage2d[i][j] < InpMin)
					InpMin = (int)OriginalImage2d[i][j];
				if ((int)OriginalImage2d[i][j] > InpMax)
					InpMax = (int)OriginalImage2d[i][j];
			}
		}

		double slope = (OutMax - OutMin) / (1.0 * (InpMax - InpMin));
		for (int i = 0; i < ImgHeight; i++)
		{
			for (int j = 0;j < ImgWidth;j++)
			{
				HistEqlImage2d[i][j] = (unsigned char)(slope * ((int)OriginalImage2d[i][j] - InpMin)) + OutMin;
			}
		}

		/* write data to plot transfer function */
		FILE *file;
		file = fopen(argv[9], "w");
		if (file != NULL)
		{
			for (int i = 0; i < ImgHeight; i++)
			{
				for (int j = 0;j < ImgWidth;j++)
				{
					fprintf(file, "%d\t%d\n", (int)OriginalImage2d[i][j], (int)HistEqlImage2d[i][j]);
				}
			}
			fclose(file);
			cout << "File " << argv[9] << " written successfully !!!" << endl;
		}
		else
		{
			cout << "Cannot open file " << argv[9] << endl;
		}
		cout << "The transfer function using Method-A is:" << endl;
		cout << "G = " << OutMin << " + " << slope << " * (F - " << InpMin << " )" << endl;
	}
	/* Histogram equalization by linear transfer function logic ends here */

	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	/* Histogram equalization by cumulative probability logic */
	else if (strcmp("mb", argv[8]) == 0)
	{
		double Prob[256];
		double CumProb[256];
		double NewPixelVal[256];
		for (int i = 0;i < 256;i++)
		{
			CumProb[256] = 0;
		}

		for (int i = 0;i < 256;i++)
		{
			HistData[i]; // number of pixels
			Prob[i] = HistData[i] / (1.0 * ImgHeight * ImgWidth); // Probability
			if (i == 0)
				CumProb[i] = Prob[i];
			else
				CumProb[i] = CumProb[i - 1] + Prob[i];
			NewPixelVal[i] = CumProb[i] * 255.0;
			NewPixelVal[i] = floor(NewPixelVal[i]);
		}

		/* write new pixel values to image */
		for (int r = 0;r < 256;r++)
		{
			for (int i = 0;i < ImgHeight; i++)
			{
				for (int j = 0;j < ImgWidth; j++)
				{
					if (OriginalImage2d[i][j] == r)
					{
						HistEqlImage2d[i][j] = NewPixelVal[r];
					}
				}
			}
		}

		/* write data to plot transfer function wrt mapped pixels */
		FILE *file;
		file = fopen(argv[10], "w");
		if (file != NULL)
		{
			for (int i = 0; i < 256; i++)
			{
				fprintf(file, "%d\t%d\n", i, (int)NewPixelVal[i]);
			}
			fclose(file);
			cout << "File " << argv[10] << " written successfully !!!" << endl;
		}
		else
		{
			cout << "Cannot open file " << argv[10] << endl;
		}

		/* write data to plot transfer function showing distribution of all pixels in the image*/
		FILE *file1;
		file1 = fopen(argv[9], "w");
		if (file != NULL)
		{
			for (int i = 0; i < ImgHeight; i++)
			{
				for (int j = 0;j < ImgWidth;j++)
				{
					fprintf(file1, "%d\t%d\n", (int)OriginalImage2d[i][j], (int)HistEqlImage2d[i][j]);
				}
			}
			fclose(file1);
			cout << "File " << argv[9] << " written successfully !!!" << endl;
		}
		else
		{
			cout << "Cannot open file " << argv[9] << endl;
		}
	}
	/* Histogram equalization by cumulative probability logic ends here*/

	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	/* error handler */
	else
	{
		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <inp_hist>.txt <out_image>.raw <out_hist>.txt <ImgWidth> <ImgHeight> <BytesPerPixel> <method_name> <tfdata_filename>.txt <tf_pixelmapping_mb>.txt" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "inp_hist: the name of the .txt file along with path to to which input image histogram data is saved" << endl;
		cout << "out_image: the name of the .raw file along with path to which the processesed image is saved" << endl;
		cout << "out_hist: the name of the .txt file along with path to to which output image histogram data is saved" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3 and for GRAY = 1)" << endl;		cout << "method_name: ma = Method-A, mb = Method-B" << endl;		cout << "tfdata_filename: the name of the .txt file along with path to to which transfer function data is saved" << endl;		cout << "tf_pixelmapping_mb: the name of the .txt file along with path to to which pixel mapped transfer function data for Method-B is saved (this parameter is only required for Method-B)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;
		
		delete[] OriginalImage1d;
		delete[] HistEqlImage1d;
		DeallocHeap2d(OriginalImage2d, ImgHeight);
		DeallocHeap2d(HistEqlImage2d, ImgHeight);
		return 0;
	}

	/* initialize histogram variable and store data to it from image */
	unsigned long HistDataEq[256];
	Histogram2d(HistDataEq, HistEqlImage2d, ImgHeight, ImgWidth);

	/* store data for ouptput image histogram */
	FileWriteHist2d(argv[4], HistDataEq);

	/* Convert HistEqlImage2d 2D to 1D so that it can be written to file */
	HistEqlImage1d = Img2dTo1d(HistEqlImage2d, ImgHeight, ImgWidth);

	/* Write MirrorImage to file */
	FileWrite(argv[3], HistEqlImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	delete[] OriginalImage1d;
	delete[] HistEqlImage1d;
	DeallocHeap2d(OriginalImage2d, ImgHeight);
	DeallocHeap2d(HistEqlImage2d, ImgHeight);

	return 0;
}

