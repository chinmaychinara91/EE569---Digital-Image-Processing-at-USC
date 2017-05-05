// ------------------------------------------------------------------------------------
// EE569 Homework Assignment #1
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

using namespace std;

/* different functions used */
double FindPsnr(unsigned char**, unsigned char**, int, int);
double FindSum(unsigned char**, int);
double FindMedian(unsigned char**, int);
unsigned char** LowPassFilter(unsigned char**, int, int, int, int);
unsigned char** MedianFilter(unsigned char**, int, int, int);
unsigned char** GaussianFilter(unsigned char**, int, int, int);
unsigned char** OutlierFilter(unsigned char**, int, int, int);

/* Find Psnr of Filtered Image to Original Noise Free Image */
double FindPsnr(unsigned char** OrigImg2d, unsigned char** FiltImg2d, int ImgHeight, int ImgWidth)
{
	int Max = 255;
	int Sum = 0;
	double Mse = 0.0;
	double Psnr;
	for (int i = 0;i < ImgHeight;i++)
	{
		for (int j = 0;j < ImgWidth;j++)
		{
			Sum = Sum + (pow((FiltImg2d[i][j] - OrigImg2d[i][j]), 2));
		}
	}
	Mse = (1 / (1.0 * ImgHeight * ImgWidth)) * Sum;
	Psnr = 10 * log10((pow(Max, 2)) / (1.0 * Mse));
	return Psnr;
}

/* Find sum of data in an array */
double FindSum(unsigned char** ImgWindow, int WindowSize)
{
	double Sum = 0.0;
	double Mean = 0.0;
	for (int i = 0;i < WindowSize;i++)
	{
		for (int j = 0;j < WindowSize;j++)
		{
			Sum = Sum + ImgWindow[i][j];
		}
	}
	return Sum;
}

/* Find median of data in an array */
double FindMedian(unsigned char** ImgWindow, int WindowDim)
{
	double temp = 0.0;
	double Median = 0.0;
	for (int i = 0;i < WindowDim;i++)
	{
		for (int j = 0;j < WindowDim;j++)
		{
			for (int k = 0;k < WindowDim;k++)
			{
				for (int l = 0;l < WindowDim;l++)
				{
					if (ImgWindow[i][j] > ImgWindow[k][l])
					{
						temp = ImgWindow[i][j];
						ImgWindow[i][j] = ImgWindow[k][l];
						ImgWindow[k][l] = temp;
					}
				}
				temp = 0;
			}
		}
	}
	Median = ImgWindow[(WindowDim - 1) / 2][(WindowDim - 1) / 2];
	return Median;
}

/* Low Pass Filter logic */
unsigned char** LowPassFilter(unsigned char** ImgNoisy, int ImgHeight, int ImgWidth, int WindowDim, int MaskType)
{
	int NewImgHeight = ImgHeight + 2 * ((WindowDim - 1) / 2);
	int NewImgWidth = ImgWidth + 2 * ((WindowDim - 1) / 2);

	unsigned char** ImgMeanFiltered = NULL;
	ImgMeanFiltered = AllocHeap2d(ImgMeanFiltered, ImgHeight, ImgWidth); 

	unsigned char** ImgWindow = NULL;
	ImgWindow = AllocHeap2d(ImgWindow, WindowDim, WindowDim);

	unsigned char** ImgNoisyPadded = NULL;
	ImgNoisyPadded = AllocHeap2d(ImgNoisyPadded, NewImgHeight, NewImgWidth);

	int** Mask = new int *[WindowDim]();
	for (int i = 0;i < WindowDim;i++)
	{
		Mask[i] = new int[WindowDim]();
		for (int j = 0;j < WindowDim;j++)
		{
			Mask[i][j] = 0;
		}
	}
	
	double Sum = 0.0;
	if (MaskType == 1)
	{
		cout << "Type-1 Mask for LPF Filtering" << endl;
		for (int i = 0;i < WindowDim;i++)
		{
			for (int j = 0;j < WindowDim;j++)
			{
				Mask[i][j] = 1;
				cout << Mask[i][j] << "\t";
				Sum = Sum + Mask[i][j];
			}
			cout << endl;
		}
		cout << "Sum of all elements in mask = " << Sum << endl;
		cout << endl;
	}

	else if (MaskType == 2)
	{
		cout << "Type-2 Mask for LPF Filtering" << endl;
		for (int i = 0;i < WindowDim;i++)
		{
			for (int j = 0;j < WindowDim;j++)
			{
				if (i == ((WindowDim - 1) / 2) && j == ((WindowDim - 1) / 2))
					Mask[i][j] = 2;
				else
					Mask[i][j] = 1;
				cout << Mask[i][j] << "\t";
				Sum = Sum + Mask[i][j];
			}
			cout << endl;
		}
		cout << "Sum of all elements in mask = " << Sum << endl;
		cout << endl;
	}
	
	else
	{
		cout << "Wrong Mask type entered for LPF Filtering !!!" << endl;
		cout << "Switching to default Type-1 !!!" << endl;
		for (int i = 0;i < WindowDim;i++)
		{
			for (int j = 0;j < WindowDim;j++)
			{
				Mask[i][j] = 1;
				cout << Mask[i][j] << "\t";
				Sum = Sum + Mask[i][j];
			}
			cout << endl;
		}
		cout << "Sum of all elements in mask = " << Sum << endl;
	}

	/* copy noisy image data to new padded image matrix by pixel replication method */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{	
			/* the actual image */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)];
			
			/* padding top side */
			if(i < ((WindowDim - 1) / 2) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][j - ((WindowDim - 1) / 2)];
			/* padding left side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j<((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][0];
			/* padding bottom side */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight-1][j - ((WindowDim - 1) / 2)];	
			/* padding right side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][ImgWidth - 1];

			/* padding top left corner */
			if (i < ((WindowDim - 1) / 2) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[0][0];
			/* padding top right corner */
			if (i<((WindowDim - 1) / 2) && j>=(NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][ImgWidth - 1];
			/* padding bottom left corner */
			if (i >=(NewImgHeight - ((WindowDim - 1) / 2)) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][0];
			/* padding bottom right corner */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j>=(NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][ImgWidth - 1];
		}
	}
	
	/* the filtering logic */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
			{
				int HeightUpperLimit = i + ((WindowDim - 1) / 2);
				int HeightLowerLimit = i - ((WindowDim - 1) / 2);
				int WidthUpperLimit = j + ((WindowDim - 1) / 2);
				int WidthLowerLimit = j - ((WindowDim - 1) / 2);
				int p = 0, q = 0;
				for (int h = HeightLowerLimit;h <= HeightUpperLimit;h++)
				{
					for (int w = WidthLowerLimit;w <= WidthUpperLimit;w++)
					{
						ImgWindow[p][q] =(unsigned char)((int)ImgNoisyPadded[h][w] * Mask[p][q]);
						q++;
					}
					p++;
					q = 0;
				}
				ImgMeanFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = (1/(1.0*Sum))*FindSum(ImgWindow, WindowDim);
			}
		}
	}

	return ImgMeanFiltered;
}

/* Median Filter logic */
unsigned char** MedianFilter(unsigned char** ImgNoisy, int ImgHeight, int ImgWidth, int WindowDim)
{
	int NewImgHeight = ImgHeight + 2 * ((WindowDim - 1) / 2);
	int NewImgWidth = ImgWidth + 2 * ((WindowDim - 1) / 2);

	unsigned char** ImgMedianFiltered = NULL;
	ImgMedianFiltered = AllocHeap2d(ImgMedianFiltered, ImgHeight, ImgWidth);

	unsigned char** ImgWindow = NULL;
	ImgWindow = AllocHeap2d(ImgWindow, WindowDim, WindowDim);

	unsigned char** ImgNoisyPadded = NULL;
	ImgNoisyPadded = AllocHeap2d(ImgNoisyPadded, NewImgHeight, NewImgWidth);

	/* copy noisy image data to new padded image matrix by pixel replication method */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{
			/* the actual image */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)];

			/* padding top side */
			if (i < ((WindowDim - 1) / 2) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][j - ((WindowDim - 1) / 2)];
			/* padding left side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][0];
			/* padding bottom side */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][j - ((WindowDim - 1) / 2)];
			/* padding right side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][ImgWidth - 1];

			/* padding top left corner */
			if (i < ((WindowDim - 1) / 2) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[0][0];
			/* padding top right corner */
			if (i < ((WindowDim - 1) / 2) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][ImgWidth - 1];
			/* padding bottom left corner */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][0];
			/* padding bottom right corner */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][ImgWidth - 1];
		}
	}

	/* the filtering logic */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
			{
				int HeightUpperLimit = i + ((WindowDim - 1) / 2);
				int HeightLowerLimit = i - ((WindowDim - 1) / 2);
				int WidthUpperLimit = j + ((WindowDim - 1) / 2);
				int WidthLowerLimit = j - ((WindowDim - 1) / 2);
				int p = 0, q = 0;
				for (int h = HeightLowerLimit;h <= HeightUpperLimit;h++)
				{
					for (int w = WidthLowerLimit;w <= WidthUpperLimit;w++)
					{
						ImgWindow[p][q] = ImgNoisyPadded[h][w];
						q++;
					}
					p++;
					q = 0;
				}
				ImgMedianFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = FindMedian(ImgWindow, WindowDim);
			}
		}
	}

	return ImgMedianFiltered;
}

/* Gaussian Filter logic */
unsigned char** GaussianFilter(unsigned char** ImgNoisy, int ImgHeight, int ImgWidth, int WindowDim)
{
	int NewImgHeight = ImgHeight + 2 * ((WindowDim - 1) / 2);
	int NewImgWidth = ImgWidth + 2 * ((WindowDim - 1) / 2);

	unsigned char** ImgGaussFiltered = NULL;
	ImgGaussFiltered = AllocHeap2d(ImgGaussFiltered, ImgHeight, ImgWidth);

	unsigned char** ImgWindow = NULL;
	ImgWindow = AllocHeap2d(ImgWindow, WindowDim, WindowDim);

	unsigned char** ImgNoisyPadded = NULL;
	ImgNoisyPadded = AllocHeap2d(ImgNoisyPadded, NewImgHeight, NewImgWidth);

	double** GaussMask = new double *[WindowDim]();
	for (int i = 0;i < WindowDim;i++)
	{
		GaussMask[i] = new double[WindowDim]();
		for (int j = 0;j < WindowDim;j++)
		{
			GaussMask[i][j] = 0;
		}
	}

	double Sum = 0.0;
	double Sigma = 1.0;
	cout << "Gaussian Mask for Gaussian Filtering" << endl;
	cout << "Gaussian mask not normalized" << endl;
	for (int i = 0;i < WindowDim;i++)
	{
		for (int j = 0;j < WindowDim;j++)
		{
			int H = j - ((WindowDim - 1) / 2);
			int W = i - ((WindowDim - 1) / 2);

			GaussMask[i][j] = (1 / (2 * 3.14*Sigma*Sigma)) * exp(-(pow(H, 2) + pow(W, 2)) / (2 * pow(Sigma, 2)));
			cout << GaussMask[i][j] << "\t";
			Sum = Sum + GaussMask[i][j];
		}
		cout << endl;
	}
	cout << "Sum of all elements in mask before normalization = " << Sum << endl;
	cout << "Normalized gaussian mask" << endl;
	double Sum1 = 0.0;
	for (int i = 0;i < WindowDim;i++)
	{
		for (int j = 0;j < WindowDim;j++)
		{
			GaussMask[i][j] = (1 / Sum) * GaussMask[i][j];
			cout << GaussMask[i][j] << "\t";
			Sum1 = Sum1 + GaussMask[i][j];
		}
		cout << endl;
	}
	cout << "Sum of all elements in mask after normalization = " << Sum1 << endl;
	cout << endl;

	/* copy noisy image data to new padded image matrix by pixel replication method */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{
			/* the actual image */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)];

			/* padding top side */
			if (i < ((WindowDim - 1) / 2) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][j - ((WindowDim - 1) / 2)];
			/* padding left side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j<((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][0];
			/* padding bottom side */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][j - ((WindowDim - 1) / 2)];
			/* padding right side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][ImgWidth - 1];

			/* padding top left corner */
			if (i < ((WindowDim - 1) / 2) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[0][0];
			/* padding top right corner */
			if (i<((WindowDim - 1) / 2) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][ImgWidth - 1];
			/* padding bottom left corner */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][0];
			/* padding bottom right corner */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][ImgWidth - 1];
		}
	}

	/* the filtering logic */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
			{
				int HeightUpperLimit = i + ((WindowDim - 1) / 2);
				int HeightLowerLimit = i - ((WindowDim - 1) / 2);
				int WidthUpperLimit = j + ((WindowDim - 1) / 2);
				int WidthLowerLimit = j - ((WindowDim - 1) / 2);
				int p = 0, q = 0;
				for (int h = HeightLowerLimit;h <= HeightUpperLimit;h++)
				{
					for (int w = WidthLowerLimit;w <= WidthUpperLimit;w++)
					{
						ImgWindow[p][q] = (unsigned char)((int)ImgNoisyPadded[h][w] * GaussMask[p][q]);
						q++;
					}
					p++;
					q = 0;
				}
				ImgGaussFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = FindSum(ImgWindow, WindowDim);
			}
		}
	}

	return ImgGaussFiltered;
}

/* Outlier FIlter logic */
unsigned char** OutlierFilter(unsigned char** ImgNoisy, int ImgHeight, int ImgWidth, int WindowDim)
{
	int NewImgHeight = ImgHeight + 2 * ((WindowDim - 1) / 2);
	int NewImgWidth = ImgWidth + 2 * ((WindowDim - 1) / 2);

	unsigned char** ImgOutlierFiltered = NULL;
	ImgOutlierFiltered = AllocHeap2d(ImgOutlierFiltered, ImgHeight, ImgWidth);

	unsigned char** ImgWindow = NULL;
	ImgWindow = AllocHeap2d(ImgWindow, WindowDim, WindowDim);

	unsigned char** ImgNoisyPadded = NULL;
	ImgNoisyPadded = AllocHeap2d(ImgNoisyPadded, NewImgHeight, NewImgWidth);

	int** Mask = new int *[WindowDim]();
	for (int i = 0;i < WindowDim;i++)
	{
		Mask[i] = new int[WindowDim]();
		for (int j = 0;j < WindowDim;j++)
		{
			Mask[i][j] = 0;
		}
	}

	double Sum = 0.0;
	cout << "Mask for Outlier Filtering" << endl;
	for (int i = 0;i < WindowDim;i++)
	{
		for (int j = 0;j < WindowDim;j++)
		{
			if (i == ((WindowDim - 1) / 2) && j == ((WindowDim - 1) / 2))
				Mask[i][j] = 0;
			else
				Mask[i][j] = 1;
			cout << Mask[i][j] << "\t";
			Sum = Sum + Mask[i][j];
		}
		cout << endl;
	}
	cout << "Sum of all elements in mask = " << Sum << endl;
	cout << endl;

	/* copy noisy image data to new padded image matrix by pixel replication method */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{
			/* the actual image */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)];

			/* padding top side */
			if (i < ((WindowDim - 1) / 2) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][j - ((WindowDim - 1) / 2)];
			/* padding left side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j<((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][0];
			/* padding bottom side */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][j - ((WindowDim - 1) / 2)];
			/* padding right side */
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((WindowDim - 1) / 2)][ImgWidth - 1];

			/* padding top left corner */
			if (i < ((WindowDim - 1) / 2) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[0][0];
			/* padding top right corner */
			if (i<((WindowDim - 1) / 2) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][ImgWidth - 1];
			/* padding bottom left corner */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j < ((WindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][0];
			/* padding bottom right corner */
			if (i >= (NewImgHeight - ((WindowDim - 1) / 2)) && j >= (NewImgWidth - ((WindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][ImgWidth - 1];
		}
	}

	/* the filtering logic */
	for (int i = 0;i < NewImgHeight;i++)
	{
		for (int j = 0;j < NewImgWidth;j++)
		{
			if (i >= ((WindowDim - 1) / 2) && i < (NewImgHeight - ((WindowDim - 1) / 2)) && j >= ((WindowDim - 1) / 2) && j < (NewImgWidth - ((WindowDim - 1) / 2)))
			{
				int HeightUpperLimit = i + ((WindowDim - 1) / 2);
				int HeightLowerLimit = i - ((WindowDim - 1) / 2);
				int WidthUpperLimit = j + ((WindowDim - 1) / 2);
				int WidthLowerLimit = j - ((WindowDim - 1) / 2);
				int p = 0, q = 0;
				for (int h = HeightLowerLimit;h <= HeightUpperLimit;h++)
				{
					for (int w = WidthLowerLimit;w <= WidthUpperLimit;w++)
					{
						ImgWindow[p][q] = (unsigned char)((int)ImgNoisyPadded[h][w] * Mask[p][q]);
						q++;
					}
					p++;
					q = 0;
				}
				double temp = 0.0;
				temp = (1 / (1.0*Sum))*FindSum(ImgWindow, WindowDim);
				if (ImgNoisyPadded[i][j] - temp > 190)
					ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = temp;
				else
					ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = ImgNoisyPadded[i][j];

				if (ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] > 255)
					ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = 255;
				if (ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] < 0)
					ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = 0;
			}
		}
	}

	return ImgOutlierFiltered;
}
