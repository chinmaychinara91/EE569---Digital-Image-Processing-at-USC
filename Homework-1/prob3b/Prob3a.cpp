// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob3a
// Date: February 5, 2017
// Name : Chinmay Chinara
// ID : 2527-2374-52
// email : chinara@usc.edu
// ------------------------------------------------------------------------------------

#include <iostream>
#include "MyImgHeaders.h"
#include "MyFilters.h"
#include <math.h>
#include <stdio.h>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	/* condition to ensure that correct parameters are passed in the command line */
	if (argc < 9 || ((strcmp(argv[7],"lpf")==0) && argv[9] == NULL))	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <noisy_image>.raw <out_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel> <filtering_method> <window_size> <mask type for lpf filtering method>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "noisy_image: the name of the .raw file along with path to be taken as input for noisy image" << endl;
		cout << "out_image: the name of the .raw file along with path for storing output clean image" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3)" << endl << endl;		cout << "filtering_methods are as follows: " << endl;		cout << "lpf = Low Pass Filter" << endl;		cout << "median = Median Filter" << endl;		cout << "gauss = Gaussian Filter" << endl;		cout << "outlier = Outlier Filter" << endl;		cout << "medianmedian = Cascaded Median + Median" << endl;		cout << "outliermedian =  Cascaded Outlier + Median" << endl;		cout << "outlierlpf =  Cascaded Outlier + LPF" << endl;		cout << "outliermedianlpf =  Cascaded Outlier + Median + LPF" << endl;		cout << "outlierlpfmedian =  Cascaded Outlier + LPF + Median" << endl;		cout << "outliergauss =  Cascaded Outlier + Gaussian" << endl << endl;		cout << "window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the window around a pixel of the image to be considered for processing" << endl;		cout << "mask type for lpf filtering method: 1 = Type-1, 2 = Type-2" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[4]); /* number of rows in the image */	int ImgHeight = atoi(argv[5]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[6]); /* bytes per pixel (RGB = 3, GRAY = 1) */
	int WindowDim = atoi(argv[8]);

	/* Allocate OriginalImage1d in heap */
	unsigned char* OriginalImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();
	/* Allocate OriginalImageR in heap */
	unsigned char** OriginalImageR = NULL;
	OriginalImageR = AllocHeap2d(OriginalImageR, ImgHeight, ImgWidth);
	/* Allocate OriginalImageG in heap */
	unsigned char** OriginalImageG = NULL;
	OriginalImageG = AllocHeap2d(OriginalImageG, ImgHeight, ImgWidth);
	/* Allocate OriginalImageB in heap */
	unsigned char** OriginalImageB = NULL;
	OriginalImageB = AllocHeap2d(OriginalImageB, ImgHeight, ImgWidth);
	/* Allocate OriginalImage3d in heap */
	unsigned char*** OriginalImage3d = NULL;
	OriginalImage3d = AllocHeap3d(OriginalImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate NoiseImage1d in heap */
	unsigned char* NoiseImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();
	/* Allocate NoiseImageR in heap */
	unsigned char** NoiseImageR = NULL;
	NoiseImageR = AllocHeap2d(NoiseImageR, ImgHeight, ImgWidth);
	/* Allocate NoiseImageG in heap */
	unsigned char** NoiseImageG = NULL;
	NoiseImageG = AllocHeap2d(NoiseImageG, ImgHeight, ImgWidth);
	/* Allocate NoiseImageB in heap */
	unsigned char** NoiseImageB = NULL;
	NoiseImageB = AllocHeap2d(NoiseImageB, ImgHeight, ImgWidth);
	/* Allocate NoiseImage3d in heap */
	unsigned char*** NoiseImage3d = NULL;
	NoiseImage3d = AllocHeap3d(NoiseImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Allocate CleanImage1d in heap */
	unsigned char* CleanImage1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();
	/* Allocate CleanImageR in heap */
	unsigned char** CleanImageR = NULL;
	CleanImageR = AllocHeap2d(CleanImageR, ImgHeight, ImgWidth);
	/* Allocate CleanImageG in heap */
	unsigned char** CleanImageG = NULL;
	CleanImageG = AllocHeap2d(CleanImageG, ImgHeight, ImgWidth);
	/* Allocate CleanImageB in heap */
	unsigned char** CleanImageB = NULL;
	CleanImageB = AllocHeap2d(CleanImageB, ImgHeight, ImgWidth);
	/* Allocate CleanImage3d in heap */
	unsigned char*** CleanImage3d = NULL;
	CleanImage3d = AllocHeap3d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Read images from file */
	FileRead(argv[1], OriginalImage1d, ImgHeight*ImgWidth*BytesPerPixel);
	FileRead(argv[2], NoiseImage1d, ImgHeight*ImgWidth*BytesPerPixel);

	/* Convert images 1D to 3D so that image traversal becomes easier */
	OriginalImage3d = Img1dTo3d(OriginalImage1d, ImgHeight, ImgWidth, BytesPerPixel);
	NoiseImage3d = Img1dTo3d(NoiseImage1d, ImgHeight, ImgWidth, BytesPerPixel);

	/* Extract R,G,B Channels from images */
	OriginalImageR = ExtractChannel(OriginalImage3d, ImgHeight, ImgWidth, 'R');
	OriginalImageG = ExtractChannel(OriginalImage3d, ImgHeight, ImgWidth, 'G');
	OriginalImageB = ExtractChannel(OriginalImage3d, ImgHeight, ImgWidth, 'B');
	NoiseImageR = ExtractChannel(NoiseImage3d, ImgHeight, ImgWidth, 'R');
	NoiseImageG = ExtractChannel(NoiseImage3d, ImgHeight, ImgWidth, 'G');
	NoiseImageB = ExtractChannel(NoiseImage3d, ImgHeight, ImgWidth, 'B');

	/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
	CleanImage1d = Img2dTo1d(NoiseImageR, ImgHeight, ImgWidth);
	FileWrite("sailboat_noisy_red.raw", CleanImage1d, ImgHeight*ImgWidth);
	CleanImage1d = Img2dTo1d(NoiseImageG, ImgHeight, ImgWidth);
	FileWrite("sailboat_noisy_green.raw", CleanImage1d, ImgHeight*ImgWidth);
	CleanImage1d = Img2dTo1d(NoiseImageB, ImgHeight, ImgWidth);
	FileWrite("sailboat_noisy_blue.raw", CleanImage1d, ImgHeight*ImgWidth);

	/* Histogram of noisy data */
	unsigned long HistNoise[256][3];
	Histogram3d(HistNoise, NoiseImage3d, ImgHeight, ImgWidth, BytesPerPixel);
	FileWriteHist3d("sailboat_noisy_hist.txt", HistNoise);
	
	/* PSNR before noise removal */
	double PsnrR = FindPsnr(OriginalImageR, NoiseImageR, ImgHeight, ImgWidth);
	double PsnrG = FindPsnr(OriginalImageG, NoiseImageG, ImgHeight, ImgWidth);
	double PsnrB = FindPsnr(OriginalImageB, NoiseImageB, ImgHeight, ImgWidth);
	cout << "PSNR_R (in dB) with noise: " << PsnrR << endl;
	cout << "PSNR_G (in dB) with noise: " << PsnrG << endl;
	cout << "PSNR_B (in dB) with noise: " << PsnrB << endl;

	/* Noise removal logic */

	/*------------------------------------------------------------------------------------ 
	Type-1
	R Channel --> Low Pass Filter Type-1
	G Channel --> Low Pass Filter Type-1
	B Channel --> Low Pass Filter Type-1

	Type-2
	R Channel --> Low Pass Filter Type-2
	G Channel --> Low Pass Filter Type-2
	B Channel --> Low Pass Filter Type-2
	------------------------------------------------------------------------------------*/
	if (strcmp(argv[7], "lpf") == 0)
	{
		int MaskType = atoi(argv[9]);

		CleanImageR = LowPassFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageG = LowPassFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageB = LowPassFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim, MaskType);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_lpf.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that clean images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_lpf.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_lpf.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_lpf.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	R Channel --> Median Filter
	G Channel --> Median Filter
	B Channel --> Median Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "median") == 0)
	{
		CleanImageR = MedianFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = MedianFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = MedianFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_median.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write MirrorImage to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that clean images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_median.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_median.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_median.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	R Channel --> Gaussian Filter
	G Channel --> Gaussian Filter
	B Channel --> Gaussian Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "gauss") == 0)
	{
		CleanImageR = GaussianFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = GaussianFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = GaussianFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_gauss.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_gauss.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_gauss.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_gauss.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	R Channel --> Outlier Filter
	G Channel --> Outlier Filter
	B Channel --> Outlier Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "outlier") == 0)
	{
		CleanImageR = OutlierFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = OutlierFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = OutlierFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_outlier.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_outlier.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_outlier.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_outlier.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	Cascade --> Median + Median

	R Channel --> Median Filter
	G Channel --> Median Filter
	B Channel --> Median Filter

	R Channel --> Median Filter
	G Channel --> Median Filter
	B Channel --> Median Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "medianmedian") == 0)
	{
		CleanImageR = MedianFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = MedianFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = MedianFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		CleanImageR = MedianFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = MedianFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = MedianFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_mix.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	Cascade --> Outlier + Median

	R Channel --> Outlier Filter
	G Channel --> Outlier Filter
	B Channel --> Outlier Filter

	R Channel --> Median Filter
	G Channel --> Median Filter
	B Channel --> Median Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "outliermedian") == 0)
	{
		CleanImageR = OutlierFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = OutlierFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = OutlierFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		CleanImageR = MedianFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = MedianFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = MedianFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_mix.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	Cascade --> Outlier + LPF

	R Channel --> Outlier Filter
	G Channel --> Outlier Filter
	B Channel --> Outlier Filter

	R Channel --> LPF Filter
	G Channel --> LPF Filter
	B Channel --> LPF Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "outlierlpf") == 0)
	{
		int MaskType = atoi(argv[9]);

		CleanImageR = OutlierFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = OutlierFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = OutlierFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		CleanImageR = LowPassFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageG = LowPassFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageB = LowPassFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim, MaskType);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_mix.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	Cascade --> Outlier + Median + LPF

	R Channel --> Outlier Filter
	G Channel --> Outlier Filter
	B Channel --> Outlier Filter

	R Channel --> Median Filter
	G Channel --> Median Filter
	B Channel --> Median Filter

	R Channel --> LPF Filter
	G Channel --> LPF Filter
	B Channel --> LPF Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "outliermedianlpf") == 0)
	{
		int MaskType = atoi(argv[9]);

		CleanImageR = OutlierFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = OutlierFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = OutlierFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		CleanImageR = MedianFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = MedianFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = MedianFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim);

		CleanImageR = LowPassFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageG = LowPassFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageB = LowPassFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim, MaskType);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_mix.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	Cascade --> Outlier + LPF + Median

	R Channel --> Outlier Filter
	G Channel --> Outlier Filter
	B Channel --> Outlier Filter

	R Channel --> LPF Filter
	G Channel --> LPF Filter
	B Channel --> LPF Filter

	R Channel --> Median Filter
	G Channel --> Median Filter
	B Channel --> Median Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "outlierlpfmedian") == 0)
	{
		int MaskType = atoi(argv[9]);

		CleanImageR = OutlierFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = OutlierFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = OutlierFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		CleanImageR = LowPassFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageG = LowPassFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim, MaskType);
		CleanImageB = LowPassFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim, MaskType);

		CleanImageR = MedianFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = MedianFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = MedianFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_mix.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	/*------------------------------------------------------------------------------------
	Cascade --> Outlier + Gaussian

	R Channel --> Outlier Filter
	G Channel --> Outlier Filter
	B Channel --> Outlier Filter

	R Channel --> Gaussian Filter
	G Channel --> Gaussian Filter
	B Channel --> Gaussian Filter
	------------------------------------------------------------------------------------*/
	else if (strcmp(argv[7], "outliergauss") == 0)
	{
		CleanImageR = OutlierFilter(NoiseImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = OutlierFilter(NoiseImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = OutlierFilter(NoiseImageB, ImgHeight, ImgWidth, WindowDim);

		CleanImageR = GaussianFilter(CleanImageR, ImgHeight, ImgWidth, WindowDim);
		CleanImageG = GaussianFilter(CleanImageG, ImgHeight, ImgWidth, WindowDim);
		CleanImageB = GaussianFilter(CleanImageB, ImgHeight, ImgWidth, WindowDim);

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "PSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Cnnvert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("sailboat_clean_hist_mix.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write clean image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that noisy images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_red_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_green_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("sailboat_clean_blue_mix.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	else
	{
		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <noisy_image>.raw <out_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel> <filtering_method> <window_size> <mask type for lpf filtering method>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		delete[] OriginalImage1d;
		delete[] NoiseImage1d;
		delete[] CleanImage1d;
		DeallocHeap2d(OriginalImageR, ImgHeight);
		DeallocHeap2d(OriginalImageG, ImgHeight);
		DeallocHeap2d(OriginalImageB, ImgHeight);
		DeallocHeap2d(NoiseImageR, ImgHeight);
		DeallocHeap2d(NoiseImageG, ImgHeight);
		DeallocHeap2d(NoiseImageB, ImgHeight);
		DeallocHeap2d(CleanImageR, ImgHeight);
		DeallocHeap2d(CleanImageG, ImgHeight);
		DeallocHeap2d(CleanImageB, ImgHeight);
		DeallocHeap3d(OriginalImage3d, ImgHeight, ImgWidth);
		DeallocHeap3d(NoiseImage3d, ImgHeight, ImgWidth);
		DeallocHeap3d(CleanImage3d, ImgHeight, ImgWidth);
		return 0;
	}
	/* Noise removal logic ends here*/

	delete[] OriginalImage1d;
	delete[] NoiseImage1d;
	delete[] CleanImage1d;
	DeallocHeap2d(OriginalImageR, ImgHeight);
	DeallocHeap2d(OriginalImageG, ImgHeight);
	DeallocHeap2d(OriginalImageB, ImgHeight);
	DeallocHeap2d(NoiseImageR, ImgHeight);
	DeallocHeap2d(NoiseImageG, ImgHeight);
	DeallocHeap2d(NoiseImageB, ImgHeight);
	DeallocHeap2d(CleanImageR, ImgHeight);
	DeallocHeap2d(CleanImageG, ImgHeight);
	DeallocHeap2d(CleanImageB, ImgHeight);
	DeallocHeap3d(OriginalImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(NoiseImage3d, ImgHeight, ImgWidth);
	DeallocHeap3d(CleanImage3d, ImgHeight, ImgWidth);

	return 0;
}