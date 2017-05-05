// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1 Prob3b_pepper
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
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
{
	/* condition to ensure that correct parameters are passed in the command line */
	if (argc != 12)	{		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <noisy_image>.raw <out_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel> <filtering_method> <similarity_window_size> <search_window_size> <Sigma> <h>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "noisy_image: the name of the .raw file along with path to be taken as input for noisy image" << endl;
		cout << "out_image: the name of the .raw file along with path for storing output clean image" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3)" << endl;		cout << "filtering_method: nlm = Non_Local Means" << endl;		cout << "similarity_window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the similarity window" << endl;		cout << "search_window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the search window" << endl;		cout << "Sigma: the standard deviation" << endl;		cout << "h: the filtering parameter" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		return 0;	}

	/* initialization of variables */
	int ImgWidth = atoi(argv[4]); /* number of rows in the image */	int ImgHeight = atoi(argv[5]); /* number of columns in the image */
	int BytesPerPixel = atoi(argv[6]); /* bytes per pixel (RGB = 3, GRAY = 1) */
	int SimilarityWindowDim = atoi(argv[8]);
	int SearchWindowDim = atoi(argv[9]);
	double Sigma = atof(argv[10]);
	double h = atof(argv[11]);

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
	FileWrite("pepper_noisy_red.raw", CleanImage1d, ImgHeight*ImgWidth);
	CleanImage1d = Img2dTo1d(NoiseImageG, ImgHeight, ImgWidth);
	FileWrite("pepper_noisy_green.raw", CleanImage1d, ImgHeight*ImgWidth);
	CleanImage1d = Img2dTo1d(NoiseImageB, ImgHeight, ImgWidth);
	FileWrite("pepper_noisy_blue.raw", CleanImage1d, ImgHeight*ImgWidth);
	
	/* Histogram of noisy data */
	unsigned long HistNoise[256][3];
	Histogram3d(HistNoise, NoiseImage3d, ImgHeight, ImgWidth, BytesPerPixel);
	FileWriteHist3d("pepper_noisy_hist.txt", HistNoise);

	/* PSNR before noise removal */
	double PsnrR = FindPsnr(OriginalImageR, NoiseImageR, ImgHeight, ImgWidth);
	double PsnrG = FindPsnr(OriginalImageG, NoiseImageG, ImgHeight, ImgWidth);
	double PsnrB = FindPsnr(OriginalImageB, NoiseImageB, ImgHeight, ImgWidth);
	cout << "PSNR_R (in dB) with noise: " << PsnrR << endl;
	cout << "PSNR_G (in dB) with noise: " << PsnrG << endl;
	cout << "PSNR_B (in dB) with noise: " << PsnrB << endl;

	/* Noise removal logic */

	/*------------------------------------------------------------------------------------
	Non-Local Means
	R Channel --> NLM
	G Channel --> NLM
	B Channel --> NLM
	------------------------------------------------------------------------------------*/
	if (strcmp(argv[7], "nlm") == 0)
	{
		cout << "\nNoise removal for R channel in progress..." << endl;
		int start_sR = clock();
		CleanImageR = NlmFilter(NoiseImageR, ImgHeight, ImgWidth, SimilarityWindowDim, SearchWindowDim, Sigma, h);
		int stop_sR = clock();
		cout << "Noise removal for R channel ended..." << endl;

		cout << "\nNoise removal for G channel in progress..." << endl;
		int start_sG = clock();
		CleanImageG = NlmFilter(NoiseImageG, ImgHeight, ImgWidth, SimilarityWindowDim, SearchWindowDim, Sigma, h);
		int stop_sG = clock();
		cout << "Noise removal for G channel ended..." << endl;

		cout << "\nNoise removal for B channel in progress..." << endl;
		int start_sB = clock();
		CleanImageB = NlmFilter(NoiseImageB, ImgHeight, ImgWidth, SimilarityWindowDim, SearchWindowDim, Sigma, h);
		int stop_sB = clock();
		cout << "Noise removal for B channel ended..." << endl;

		/* Display execution times for noise removal by NLM filtering for each channel */
		cout << "\nTime taken for R channel noise removal: " << (stop_sR - start_sR) / double(CLOCKS_PER_SEC) << " seconds" << endl;
		cout << "Time taken for G channel noise removal: " << (stop_sG - start_sG) / double(CLOCKS_PER_SEC) << " seconds" << endl;
		cout << "Time taken for B channel noise removal: " << (stop_sB - start_sB) / double(CLOCKS_PER_SEC) << " seconds" << endl;

		/* PSNR after noise removal */
		PsnrR = FindPsnr(OriginalImageR, CleanImageR, ImgHeight, ImgWidth);
		PsnrG = FindPsnr(OriginalImageG, CleanImageG, ImgHeight, ImgWidth);
		PsnrB = FindPsnr(OriginalImageB, CleanImageB, ImgHeight, ImgWidth);
		cout << "\nPSNR_R (in dB) after noise removal: " << PsnrR << endl;
		cout << "PSNR_G (in dB) after noise removal: " << PsnrG << endl;
		cout << "PSNR_B (in dB) after noise removal: " << PsnrB << endl;

		/* Convert 2D to 3D images */
		CleanImage3d = CombineChannel(CleanImageR, CleanImageG, CleanImageB, ImgHeight, ImgWidth, BytesPerPixel);

		/* Histogram of clean data */
		unsigned long HistClean[256][3];
		Histogram3d(HistClean, CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);
		FileWriteHist3d("pepper_clean_hist_nlm.txt", HistClean);

		/* Convert images 3D to 1D so that it can be written to file */
		CleanImage1d = Img3dTo1d(CleanImage3d, ImgHeight, ImgWidth, BytesPerPixel);

		/* Write Image to file */
		FileWrite(argv[3], CleanImage1d, ImgHeight*ImgWidth*BytesPerPixel);

		/* Convert images 2D to 1D so that clean images channel wise can be written to file */
		CleanImage1d = Img2dTo1d(CleanImageR, ImgHeight, ImgWidth);
		FileWrite("pepper_clean_red_nlm.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageG, ImgHeight, ImgWidth);
		FileWrite("pepper_clean_green_nlm.raw", CleanImage1d, ImgHeight*ImgWidth);
		CleanImage1d = Img2dTo1d(CleanImageB, ImgHeight, ImgWidth);
		FileWrite("pepper_clean_blue_nlm.raw", CleanImage1d, ImgHeight*ImgWidth);
	}

	else
	{
		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Syntax Error - Incorrect parameter usage" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;		cout << "Make sure the parameters passed are in the following format:" << endl;
		cout << "<program_name> <inp_image>.raw <noisy_image>.raw <out_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel> <filtering_method> <similarity_window_size> <search_window_size> <Sigma> <h>" << endl;
		cout << "-----------------------------------------------------------------------------------------------" << endl;
		cout << "Details of the above parameters:" << endl;
		cout << "program_name: the name of the .exe file generated" << endl;
		cout << "inp_image: the name of the .raw file along with path to be taken as input for processing" << endl;
		cout << "noisy_image: the name of the .raw file along with path to be taken as input for noisy image" << endl;
		cout << "out_image: the name of the .raw file along with path for storing output clean image" << endl;
		cout << "ImgWidth: Width of the image" << endl;
		cout << "ImgHeight: Height of the image" << endl;		cout << "BytesPerPixel: bytes per pixel (for RGB = 3)" << endl;		cout << "filtering_method: nlm = Non_Local Means" << endl;		cout << "similarity_window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the similarity window" << endl;		cout << "search_window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the search window" << endl;		cout << "Sigma: the standard deviation" << endl;		cout << "h: the filtering parameter" << endl;		cout << "-----------------------------------------------------------------------------------------------" << endl;
		
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

