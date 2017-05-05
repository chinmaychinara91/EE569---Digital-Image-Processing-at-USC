// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob1a3
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
	if (argc != 11)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <OriginalImage>.raw <ForegroundImage>.raw <ImgWidth1> <ImgHeight1> <BytesPerPixel1> <NewBackgroundImage>.raw <CompositeImage>.raw <ImgWidth2> <ImgHeight2> <BytesPerPixel2>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "OriginalImage: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "ForegroundImage: the name of the .raw file along with path to which the background removed image is saved" << endl;
		cout << "ImgWidth1: Width of the smaller image" << endl;
		cout << "ImgHeight1: Height of the smaller image" << endl;		cout << "BytesPerPixel1: bytes per pixel of smaller image (for RGB = 3 and for GRAY = 1)" << endl;		cout << "NewBackgroundImage: the name of the .raw file along with path to be taken as input for background of composite image" << endl;
		cout << "CompositeImage: the name of the .raw file along with path to which the composited image is saved" << endl;
		cout << "ImgWidth2: Width of the larger image" << endl;
		cout << "ImgHeight2: Height of the larger image" << endl;		cout << "BytesPerPixel2: bytes per pixel of larger image (for RGB = 3 and for GRAY = 1)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth1 = atoi(argv[3]); /* number of rows in the image */	int ImgHeight1 = atoi(argv[4]); /* number of columns in the image */
	int BytesPerPixel1 = atoi(argv[5]); /* bytes per pixel (RGB = 3, GRAY = 1) */
	int ImgWidth2 = atoi(argv[8]); /* number of rows in the image */	int ImgHeight2 = atoi(argv[9]); /* number of columns in the image */
	int BytesPerPixel2 = atoi(argv[10]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate OriginalImage1d in heap */
	unsigned char* OriginalImage1d = new unsigned char[ImgHeight1*ImgWidth1*BytesPerPixel1]();

	/* Allocate OriginalImage3d in heap */
	unsigned char*** OriginalImage3d = NULL;
	OriginalImage3d = AllocHeap3d(OriginalImage3d, ImgHeight1, ImgWidth1, BytesPerPixel1);

	/* Allocate ForegroundImage1d in heap */
	unsigned char* ForegroundImage1d = new unsigned char[ImgHeight1*ImgWidth1*BytesPerPixel1]();

	/* Allocate ForegroundImage3d in heap */
	unsigned char*** ForegroundImage3d = NULL;
	ForegroundImage3d = AllocHeap3d(ForegroundImage3d, ImgHeight1, ImgWidth1, BytesPerPixel1);

	/* Allocate BackgroundImage1d in heap */
	unsigned char* BackgroundImage1d = new unsigned char[ImgHeight2*ImgWidth2*BytesPerPixel2]();

	/* Allocate BackgroundImage3d in heap */
	unsigned char*** BackgroundImage3d = NULL;
	BackgroundImage3d = AllocHeap3d(BackgroundImage3d, ImgHeight2, ImgWidth2, BytesPerPixel2);

	/* Allocate CompositeImage1d in heap */
	unsigned char* CompositeImage1d = new unsigned char[ImgHeight2*ImgWidth2*BytesPerPixel2]();

	/* Allocate CompositeImage3d in heap */
	unsigned char*** CompositeImage3d = NULL;
	CompositeImage3d = AllocHeap3d(CompositeImage3d, ImgHeight2, ImgWidth2, BytesPerPixel2);

	/* Read Image from file */
	FileRead(argv[1], OriginalImage1d, ImgHeight1*ImgWidth1*BytesPerPixel1);
	FileRead(argv[6], BackgroundImage1d, ImgHeight2*ImgWidth2*BytesPerPixel2);

	/* Convert Image 1D to 3D so that image traversal becomes easier */
	OriginalImage3d = Img1dTo3d(OriginalImage1d, ImgHeight1, ImgWidth1, BytesPerPixel1);
	BackgroundImage3d = Img1dTo3d(BackgroundImage1d, ImgHeight2, ImgWidth2, BytesPerPixel2);

	/* initialize histogram variable and store data to it from image */
	unsigned long HistData[256][3];
	Histogram3d(HistData, OriginalImage3d, ImgHeight1, ImgWidth1, BytesPerPixel1);
	FileWriteHist3d("input_hist.txt", HistData);

	/* 1-a-3 Background removal logic to capture only the foreground and replace with beach */
	for (int i = 0; i < ImgHeight1; i++)
	{
		for (int j = 0; j < ImgWidth1; j++)
		{
			if (OriginalImage3d[i][j][0] <150 && OriginalImage3d[i][j][1] <150 && OriginalImage3d[i][j][2] > 150)
			{
				ForegroundImage3d[i][j][0] = BackgroundImage3d[400 + i][1100 + j][0];
				ForegroundImage3d[i][j][1] = BackgroundImage3d[400 + i][1100 + j][1];
				ForegroundImage3d[i][j][2] = BackgroundImage3d[400 + i][1100 + j][2];
			}
			else
			{
				ForegroundImage3d[i][j][0] = OriginalImage3d[i][j][0];
				ForegroundImage3d[i][j][1] = OriginalImage3d[i][j][1];
				ForegroundImage3d[i][j][2] = OriginalImage3d[i][j][2];
			}
		}
	}
	cout << "Foreground extraction done successfully !!!" << endl;

	/* 1-a-3 Image compositing logic */
	for (int i = 0; i < ImgHeight2; i++)
	{
		for (int j = 0; j < ImgWidth2; j++)
		{
			if (j >= 1100 && j < 1400 && i >= 400 && i <700)
			{
				CompositeImage3d[i][j][0] = ForegroundImage3d[i - 400][j - 1100][0];
				CompositeImage3d[i][j][1] = ForegroundImage3d[i - 400][j - 1100][1];
				CompositeImage3d[i][j][2] = ForegroundImage3d[i - 400][j - 1100][2];
			}
			else
			{
				CompositeImage3d[i][j][0] = BackgroundImage3d[i][j][0];
				CompositeImage3d[i][j][1] = BackgroundImage3d[i][j][1];
				CompositeImage3d[i][j][2] = BackgroundImage3d[i][j][2];
			}
		}
	}
	cout << "Composite image created successfully !!!" << endl;

	/* Convert Image 3D to 1D so that it can be written to file */
	ForegroundImage1d = Img3dTo1d(ForegroundImage3d, ImgHeight1, ImgWidth1, BytesPerPixel1);
	CompositeImage1d = Img3dTo1d(CompositeImage3d, ImgHeight2, ImgWidth2, BytesPerPixel2);

	/* Write Image to file */
	FileWrite(argv[2], ForegroundImage1d, ImgHeight1*ImgWidth1*BytesPerPixel1);
	FileWrite(argv[7], CompositeImage1d, ImgHeight2*ImgWidth2*BytesPerPixel2);

	/* Deallocate OriginalImage from heap */
	delete[] OriginalImage1d;

	/* Deallocate ForegroundImage from heap */
	delete[] ForegroundImage1d;

	/* Deallocate BackgroundImage from heap */
	delete[] BackgroundImage1d;

	/* Deallocate CompositeImage from heap */
	delete[] CompositeImage1d;

	/* deallocate OriginalImage from heap */
	DeallocHeap3d(OriginalImage3d, ImgHeight1, ImgWidth1);

	/* deallocate ForegroundImage from heap */
	DeallocHeap3d(ForegroundImage3d, ImgHeight1, ImgWidth1);

	/* deallocate BackgroundImage from heap */
	DeallocHeap3d(BackgroundImage3d, ImgHeight2, ImgWidth2);
	
	/* deallocate CompositeImage from heap */
	DeallocHeap3d(CompositeImage3d, ImgHeight2, ImgWidth2);

	return 0;
}

