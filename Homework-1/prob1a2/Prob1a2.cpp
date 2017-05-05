// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob1a2
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
		cout << "<program_name> <input_mirror_image>.raw <output_image200*200>.raw <output_image400*400>.raw <output_image600*600>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "input_mirror_image: the name of the mirror .raw file along with path to be taken as input for processing" << endl;
		cout << "output_image200*200: the name of the 200*200 .raw file along with path to which the processesed image is saved" << endl;
		cout << "output_image400*400: the name of the 400*400 .raw file along with path to which the processesed image is saved" << endl;
		cout << "output_image600*600: the name of the 600*600 .raw file along with path to which the processesed image is saved" << endl;
		cout << "ImgWidth: Width of the input image" << endl;
		cout << "ImgHeight: Height of the input image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3)" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[5]); /* number of rows in the image */	int ImgHeight = atoi(argv[6]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[7]); /* bytes per pixel (RGB = 3, GRAY = 1) */

	/* Allocate MirrorImage1d in heap */
	unsigned char* MirrorImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();

	/* Allocate MirrorImage3d in heap */
	unsigned char*** MirrorImage3d = NULL;
	MirrorImage3d = AllocHeap3d(MirrorImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate ResizeImage1d in heap */
	unsigned char* ResizeImage1d200_200 = new unsigned char[200 * 200 * BytesPerPixel]();
	unsigned char* ResizeImage1d400_400 = new unsigned char[400 * 400 * BytesPerPixel]();
	unsigned char* ResizeImage1d600_600 = new unsigned char[600 * 600 * BytesPerPixel]();

	/* Allocate ResizeImage3d200_200 in heap */
	unsigned char*** ResizeImage3d200_200 = NULL;
	ResizeImage3d200_200 = AllocHeap3d(ResizeImage3d200_200, 200, 200, BytesPerPixel);

	/* Allocate ResizeImage3d400_400 in heap */
	unsigned char*** ResizeImage3d400_400 = NULL;
	ResizeImage3d400_400 = AllocHeap3d(ResizeImage3d400_400, 400, 400, BytesPerPixel);

	/* Allocate ResizeImage3d600_600 in heap */
	unsigned char*** ResizeImage3d600_600 = NULL;
	ResizeImage3d600_600 = AllocHeap3d(ResizeImage3d600_600, 600, 600, BytesPerPixel);

	/* Read MirrorImage1d from file */
	FileRead(argv[1], MirrorImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert MirrorImage 1D to 3D so that image traversal becomes easier */
	MirrorImage3d = Img1dTo3d(MirrorImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* 1-a-2 Image resizing to 200*200 by bilinear interpolation logic */
	for (int i = 0; i < 200; i++) 
	{
		/* mapping new coordinates for the pixels along the height of the image */
		double TranslateFactorHeightPixel = (1.0 * (ImgHeight-1) / 200) * i;
		
		/* set the new height as rounded value of the mapped co-ordinate above */
		int NewHeight = (int)floor(TranslateFactorHeightPixel);
		
		/* assuring that the boundary never overflows */
		if (NewHeight < 0)
		{
			NewHeight = 0;
		}
		if (NewHeight >= ImgHeight - 1)
		{ 
			NewHeight = ImgHeight - 1;
		}

		/* calulate by how much the pixels along the height of the image would translate */
		double DeltaHeight = TranslateFactorHeightPixel - NewHeight;

		for (int j = 0; j < 200; j++) 
		{
			/* mapping new coordinates for the pixels along the width of the image */
			double TranslateFactorWidthPixel = (1.0 * (ImgWidth-1) / 200) * j;
			
			/* set the new width as rounded value of the mapped co-ordinate above */
			int NewWidth = (int)floor(TranslateFactorWidthPixel);
			
			/* assuring that the boundary never overflows */
			if (NewWidth < 0)
			{ 
				NewWidth = 0;
			}
			if (NewWidth >= ImgWidth - 1)
			{ 
				NewWidth = ImgWidth - 1;
			}

			/* calulate by how much the pixels along the width of the image would translate */
			double DeltaWidth = TranslateFactorWidthPixel - NewWidth;

			/* use the bilinear interpolation method to calculate the resized image pixel values */
			ResizeImage3d200_200[i][j][0] = (unsigned char)
											((1 - DeltaHeight) * (1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][0] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][0] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][0] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][0]);

			ResizeImage3d200_200[i][j][1] = (unsigned char)
											((1 - DeltaHeight)*(1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][1] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][1] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][1] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][1]);

			ResizeImage3d200_200[i][j][2] = (unsigned char)
											((1 - DeltaHeight)*(1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][2] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][2] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][2] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][2]);
		}
	}
	cout << "Image resizing to 200*200 done successfully !!!" << endl;

	/* 1-a-2 Image resizing to 400*400 by bilinear interpolation logic*/
	for (int i = 0; i < 400; i++)
	{
		/* mapping new coordinates for the pixels along the height of the image */
		double TranslateFactorHeightPixel = (1.0 * (ImgHeight - 1) / 400) * i;
		
		/* set the new height as rounded value of the mapped co-ordinate above */
		int NewHeight = (int)floor(TranslateFactorHeightPixel);
		
		/* assuring that the boundary never overflows */
		if (NewHeight < 0)
		{
			NewHeight = 0;
		}
		if (NewHeight >= ImgHeight - 1)
		{
			NewHeight = ImgHeight - 1;
		}
		
		/* calulate by how much the pixels along the height of the image would translate */
		double DeltaHeight = TranslateFactorHeightPixel - NewHeight;

		for (int j = 0; j < 400; j++) 
		{
			/* mapping new coordinates for the pixels along the width of the image */
			double TranslateFactorWidthPixel = (1.0 * (ImgWidth - 1) / 400) * j;
			
			/* set the new width as rounded value of the mapped co-ordinate above */
			int NewWidth = (int)floor(TranslateFactorWidthPixel);
			
			/* assuring that the boundary never overflows */
			if (NewWidth < 0)
			{
				NewWidth = 0;
			}
			if (NewWidth >= ImgWidth - 1)
			{
				NewWidth = ImgWidth - 1;
			}
			
			/* calulate by how much the pixels along the width of the image would translate */
			double DeltaWidth = TranslateFactorWidthPixel - NewWidth;

			/* use the bilinear interpolation method to calulate the resized image pixel values */
			ResizeImage3d400_400[i][j][0] = (unsigned char)
											((1 - DeltaHeight) * (1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][0] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][0] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][0] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][0]);

			ResizeImage3d400_400[i][j][1] = (unsigned char)
											((1 - DeltaHeight)*(1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][1] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][1] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][1] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][1]);

			ResizeImage3d400_400[i][j][2] = (unsigned char)
											((1 - DeltaHeight)*(1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][2] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][2] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][2] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][2]);
		}
	}
	cout << "Image resizing to 400*400 done successfully !!!" << endl;

	/* 1-a-2 Image resizing to 600*600 by bilinear interpolation logic*/
	for (int i = 0; i < 600; i++)
	{
		/* mapping new coordinates for the pixels along the height of the image */
		double TranslateFactorHeightPixel = (1.0 * (ImgHeight-1) / 600) * i;
		
		/* set the new height as rounded value of the mapped co-ordinate above */
		int NewHeight = (int)floor(TranslateFactorHeightPixel);
		
		/* assuring that the boundary never overflows */
		if (NewHeight < 0)
		{
			NewHeight = 0;
		}
		if (NewHeight >= ImgHeight - 1)
		{
			NewHeight = ImgHeight - 1;
		}
		
		/* calulate by how much the pixels along the height of the image would translate */
		double DeltaHeight = TranslateFactorHeightPixel - NewHeight;

		for (int j = 0; j < 600; j++) 
		{
			/* mapping new coordinates for the pixels along the width of the image */
			double TranslateFactorWidthPixel = (1.0 * (ImgWidth-1) / 600) * j;
			
			/* set the new width as rounded value of the mapped co-ordinate above */
			int NewWidth = (int)floor(TranslateFactorWidthPixel);
			
			/* assuring that the boundary never overflows */
			if (NewWidth < 0)
			{
				NewWidth = 0;
			}
			if (NewWidth >= ImgWidth - 1)
			{
				NewWidth = ImgWidth - 1;
			}
			
			/* calulate by how much the pixels along the width of the image would translate */
			double DeltaWidth = TranslateFactorWidthPixel - NewWidth;

			/* use the bilinear interpolation method to calulate the resized image pixel values */
			ResizeImage3d600_600[i][j][0] = (unsigned char)
											((1 - DeltaHeight) * (1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][0] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][0] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][0] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][0]);

			ResizeImage3d600_600[i][j][1] = (unsigned char)
											((1 - DeltaHeight)*(1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][1] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][1] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][1] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][1]);

			ResizeImage3d600_600[i][j][2] = (unsigned char)
											((1 - DeltaHeight)*(1 - DeltaWidth) * MirrorImage3d[NewHeight][NewWidth][2] +
											DeltaHeight * (1 - DeltaWidth) * MirrorImage3d[NewHeight + 1][NewWidth][2] +
											(1 - DeltaHeight) * DeltaWidth * MirrorImage3d[NewHeight][NewWidth + 1][2] +
											DeltaHeight * DeltaWidth * MirrorImage3d[NewHeight + 1][NewWidth + 1][2]);
		}
	}
	cout << "Image resizing to 600*600 done successfully !!!" << endl;

	/* Convert ResizeImage 3D to 1D so that it can be written to file */
	ResizeImage1d200_200 = Img3dTo1d(ResizeImage3d200_200, 200, 200, BytesPerPixel);
	ResizeImage1d400_400 = Img3dTo1d(ResizeImage3d400_400, 400, 400, BytesPerPixel);
	ResizeImage1d600_600 = Img3dTo1d(ResizeImage3d600_600, 600, 600, BytesPerPixel);

	/* Write ResizeImage to file */
	FileWrite(argv[2], ResizeImage1d200_200, 200 * 200 * BytesPerPixel);
	FileWrite(argv[3], ResizeImage1d400_400, 400 * 400 * BytesPerPixel);
	FileWrite(argv[4], ResizeImage1d600_600, 600 * 600 * BytesPerPixel);

	/* Deallocate MirrorImage1d from heap */
	delete[] MirrorImage1d;

	/* Deallocate ResizeImage1d from heap */
	delete[] ResizeImage1d200_200;
	delete[] ResizeImage1d400_400;
	delete[] ResizeImage1d600_600;

	/* Deallocate MirrorImage3d from heap */
	DeallocHeap3d(MirrorImage3d, ImgHeight, ImgWidth);

	/* Deallocate ResizeImage3d from heap */
	DeallocHeap3d(ResizeImage3d200_200, 200, 200);
	DeallocHeap3d(ResizeImage3d400_400, 400, 400);
	DeallocHeap3d(ResizeImage3d600_600, 600, 600);

	return 0;
}