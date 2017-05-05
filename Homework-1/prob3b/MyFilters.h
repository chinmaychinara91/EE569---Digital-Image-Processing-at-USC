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

/* different functions used */
double FindPsnr(unsigned char**, unsigned char**, int, int);

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

	else if (MaskType == 3)
	{
		int b = 2;
		cout << "Type-3 Mask for LPF Filtering" << endl;
		for (int i = 0;i < WindowDim;i++)
		{
			for (int j = 0;j < WindowDim;j++)
			{
				if (i == ((WindowDim - 1) / 2) && j == ((WindowDim - 1) / 2))
					Mask[i][j] = pow(b, 2);
				else if ((i == ((WindowDim - 1) / 2) && j != ((WindowDim - 1) / 2)) || (i != ((WindowDim - 1) / 2) && j == ((WindowDim - 1) / 2)))
					Mask[i][j] = b;
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

			GaussMask[i][j] = (1 / (2 * 3.14*Sigma)) * exp(-(pow(H, 2) + pow(W, 2)) / (2 * pow(Sigma, 2)));
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
				if (ImgNoisyPadded[i][j] - temp > 110)
					ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = temp;
				else
					ImgOutlierFiltered[i - ((WindowDim - 1) / 2)][j - ((WindowDim - 1) / 2)] = ImgNoisyPadded[i][j];
			}
		}
	}

	return ImgOutlierFiltered;
}

unsigned char** NlmFilter(unsigned char** ImgNoisy, int ImgHeight, int ImgWidth, int SimilarityWindowDim, int SearchWindowDim, double Sigma, double h)
{
	int NewImgHeight = ImgHeight + 2 * ((SimilarityWindowDim - 1) / 2);
	int NewImgWidth = ImgWidth + 2 * ((SimilarityWindowDim - 1) / 2);

	unsigned char** ImgNlmFiltered = NULL;
	ImgNlmFiltered = AllocHeap2d(ImgNlmFiltered, ImgHeight, ImgWidth);

	unsigned char** ImgSimilarityWindow1 = NULL;
	ImgSimilarityWindow1 = AllocHeap2d(ImgSimilarityWindow1, SimilarityWindowDim, SimilarityWindowDim);

	unsigned char** ImgSimilarityWindow2 = NULL;
	ImgSimilarityWindow2 = AllocHeap2d(ImgSimilarityWindow2, SimilarityWindowDim, SimilarityWindowDim);

	unsigned char** ImgNoisyPadded = NULL;
	ImgNoisyPadded = AllocHeap2d(ImgNoisyPadded, NewImgHeight, NewImgWidth);

	double** GaussMask = new double *[SimilarityWindowDim]();
	for (int i = 0;i < SimilarityWindowDim;i++)
	{
		GaussMask[i] = new double[SimilarityWindowDim]();
		for (int j = 0;j < SimilarityWindowDim;j++)
		{
			GaussMask[i][j] = 0;
		}
	}

	/* Generate Gausian mask and normalize it */
	double Sum = 0.0;
	cout << "Gaussian mask for NLM Filtering" << endl;
	cout << "Gaussian mask not normalized" << endl;
	for (int i = 0;i < SimilarityWindowDim;i++)
	{
		for (int j = 0;j < SimilarityWindowDim;j++)
		{
			int H = j - ((SimilarityWindowDim - 1) / 2);
			int W = i - ((SimilarityWindowDim - 1) / 2);

			GaussMask[i][j] = (1 / (2 * 3.14 *Sigma * Sigma)) * exp(-(pow(H, 2) + pow(W, 2)) / (2 * pow(Sigma, 2)));
			cout << GaussMask[i][j] << "\t";
			Sum = Sum + GaussMask[i][j];
		}
		cout << endl;
	}
	cout << "Sum of all elements in mask before normalization = " << Sum << endl;
	cout << "Normalized gaussian mask" << endl;
	double Sum1 = 0.0;
	for (int i = 0;i < SimilarityWindowDim;i++)
	{
		for (int j = 0;j < SimilarityWindowDim;j++)
		{
			GaussMask[i][j] = (1/Sum) * GaussMask[i][j];
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
			if (i >= ((SimilarityWindowDim - 1) / 2) && i < (NewImgHeight - ((SimilarityWindowDim - 1) / 2)) && j >= ((SimilarityWindowDim - 1) / 2) && j < (NewImgWidth - ((SimilarityWindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((SimilarityWindowDim - 1) / 2)][j - ((SimilarityWindowDim - 1) / 2)];

			/* padding top side */
			if (i < ((SimilarityWindowDim - 1) / 2) && j >= ((SimilarityWindowDim - 1) / 2) && j < (NewImgWidth - ((SimilarityWindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][j - ((SimilarityWindowDim - 1) / 2)];
			/* padding left side */
			if (i >= ((SimilarityWindowDim - 1) / 2) && i < (NewImgHeight - ((SimilarityWindowDim - 1) / 2)) && j < ((SimilarityWindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((SimilarityWindowDim - 1) / 2)][0];
			/* padding bottom side */
			if (i >= (NewImgHeight - ((SimilarityWindowDim - 1) / 2)) && j >= ((SimilarityWindowDim - 1) / 2) && j < (NewImgWidth - ((SimilarityWindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][j - ((SimilarityWindowDim - 1) / 2)];
			/* padding right side */
			if (i >= ((SimilarityWindowDim - 1) / 2) && i < (NewImgHeight - ((SimilarityWindowDim - 1) / 2)) && j >= (NewImgWidth - ((SimilarityWindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[i - ((SimilarityWindowDim - 1) / 2)][ImgWidth - 1];

			/* padding top left corner */
			if (i < ((SimilarityWindowDim - 1) / 2) && j < ((SimilarityWindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[0][0];
			/* padding top right corner */
			if (i < ((SimilarityWindowDim - 1) / 2) && j >= (NewImgWidth - ((SimilarityWindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[0][ImgWidth - 1];
			/* padding bottom left corner */
			if (i >= (NewImgHeight - ((SimilarityWindowDim - 1) / 2)) && j < ((SimilarityWindowDim - 1) / 2))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][0];
			/* padding bottom right corner */
			if (i >= (NewImgHeight - ((SimilarityWindowDim - 1) / 2)) && j >= (NewImgWidth - ((SimilarityWindowDim - 1) / 2)))
				ImgNoisyPadded[i][j] = ImgNoisy[ImgHeight - 1][ImgWidth - 1];
		}
	}

	/* the filtering logic */
	for (int i = 0;i < ImgHeight;i++)
	{
		for (int j = 0;j < ImgWidth;j++)
		{
			int Pad = ((SimilarityWindowDim - 1) / 2);
			int iPad = i + Pad;
			int jPad = j + Pad;
			
			/* Define similarity window 1 */
			int HeightLowerLimit1 = iPad - Pad;
			int HeightUpperLimit1 = iPad + Pad;
			int WidthLowerLimit1 = jPad - Pad;
			int WidthUpperLimit1 = jPad + Pad;

			int p1 = 0, q1 = 0;
			for (int h1 = HeightLowerLimit1;h1 <= HeightUpperLimit1;h1++)
			{
				for (int w1 = WidthLowerLimit1;w1 <= WidthUpperLimit1;w1++)
				{
					ImgSimilarityWindow1[p1][q1] = ImgNoisyPadded[h1][w1];
					q1++;
				}
				p1++;
				q1 = 0;
			}

			/* initialize variables */
			double W = 0.0, WMax = 0.0, Xk = 0.0, Z = 0.0, SumG = 0.0;

			/* Define a search window within the actual image */
			int SHeightLowerLimit = max(iPad - SearchWindowDim, Pad +1);
			int SHeightUpperLimit = min(iPad + SearchWindowDim, ImgHeight + Pad);
			int SWidthLowerLimit = max(jPad - SearchWindowDim, Pad + 1);
			int SWidthUpperLimit = min(jPad + SearchWindowDim , ImgWidth + Pad);

			//For Debug purpose:
			//cout << SHeightLowerLimit << "\t" << SHeightUpperLimit << "\t" << SWidthLowerLimit << "\t" << SWidthUpperLimit << endl;

			for (int k = SHeightLowerLimit; k <= SHeightUpperLimit;k++)
			{
				for (int l = SWidthLowerLimit; l <= SWidthUpperLimit;l++)
				{
					/* ignore if the coordinate matches with the coordinates of the current window */
					if (k == iPad && l == jPad)
						break;

					/* Define similarity window 2 */
					int HeightLowerLimit2 = k - Pad;
					int HeightUpperLimit2 = k + Pad;
					int WidthLowerLimit2 = l - Pad;
					int WidthUpperLimit2 = l + Pad;

					int p2 = 0, q2 = 0;
					for (int h2 = HeightLowerLimit2;h2 < HeightUpperLimit2;h2++)
					{
						for (int w2 = WidthLowerLimit2;w2 < WidthUpperLimit2;w2++)
						{
							ImgSimilarityWindow2[p2][q2] = ImgNoisyPadded[h2][w2];
							q2++;
						}
						p2++;
						q2 = 0;
					}

					SumG = 0.0;

					/* Compute sum from Gaussians */
					for (int m = 0;m < SimilarityWindowDim;m++)
					{
						for (int n = 0;n < SimilarityWindowDim;n++)
						{
							SumG = SumG + (GaussMask[m][n] * pow((ImgSimilarityWindow1[m][n] - ImgSimilarityWindow2[m][n]), 2));
						}
					}

					/* Compute W */
					W = exp(-SumG / (h*h));
					if (W > WMax)
						WMax = W;

					/* Keep calculating normalizing factor cumulatively */
					Z = Z + W;

					/* Add Xk cumulatively */
					Xk = Xk + W *  ImgNoisyPadded[k][l];
				}
			}/* end of 21x21 loop */

				/* Update Xk */
			Xk = Xk + WMax * ImgNoisyPadded[iPad][jPad];

			/* Update Z */
			Z = Z + WMax;

			/* Final filtered image */
			if (Z > 0.0)
			{
				ImgNlmFiltered[i][j] = (Xk / Z);
			}
			else
			{
				ImgNlmFiltered[i][j] = ImgNoisy[i][j];
			}
		}
	}

	return ImgNlmFiltered;
}