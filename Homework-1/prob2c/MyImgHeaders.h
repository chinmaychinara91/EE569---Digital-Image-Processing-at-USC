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
unsigned char** AllocHeap2d(unsigned char**, int, int);
unsigned char*** AllocHeap3d(unsigned char***, int, int, int);
void DeallocHeap2d(unsigned char**, int);
void DeallocHeap3d(unsigned char***, int, int);
unsigned char*** Img1dTo3d(unsigned char*, int, int, int);
unsigned char* Img3dTo1d(unsigned char***, int, int, int);
unsigned char** Img1dTo2d(unsigned char*, int, int);
unsigned char* Img2dTo1d(unsigned char**, int, int);
void FileRead(char*, unsigned char*, long int);
void FileWrite(char*, unsigned char*, long int);
void Histogram2d(unsigned long, unsigned char**, int, int);
void Histogram3d(unsigned long, unsigned char***, int, int, int);
void FileWriteHist2d(char*, unsigned long);
void FileWriteHist3d(char*, unsigned long);
unsigned char** ExtractChannel(unsigned char*** , int, int, char);
unsigned char*** CombineChannel(unsigned char**, unsigned char**, unsigned char**, int, int, int);

/* Allocate 2D memory in heap*/
unsigned char** AllocHeap2d(unsigned char** Img2d, int ImgHeight, int ImgWidth)
{
	Img2d = new unsigned char *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img2d[i] = new unsigned char[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img2d[i][j] = 0;
		}
	}
	return Img2d;
}

/* Allocate 3d memory in heap*/
unsigned char*** AllocHeap3d(unsigned char*** Img3d, int ImgHeight, int ImgWidth, int BytesPerPixel)
{
	Img3d = new unsigned char **[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img3d[i] = new unsigned char *[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img3d[i][j] = new unsigned char[BytesPerPixel]();
			for (int k = 0; k < BytesPerPixel;k++)
			{
				Img3d[i][j][k] = 0;
			}
		}
	}
	return Img3d;
}

/* Deallocate 2D memory in heap*/
void DeallocHeap2d(unsigned char** Img2d, int ImgHeight)
{
	for (int i = 0; i < ImgHeight; i++)
	{
		delete[] Img2d[i];
	}
	delete[] Img2d;
}

/* Deallocate 3D memory in heap*/
void DeallocHeap3d(unsigned char*** Img3d, int ImgHeight, int ImgWidth)
{
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
			delete[] Img3d[i][j];
		delete[] Img3d[i];
	}
	delete[] Img3d;
}

/* Convert 1D image data to 2D */
unsigned char** Img1dTo2d(unsigned char* Img1d, int ImgHeight, int ImgWidth)
{
	unsigned char** Img2d = new unsigned char *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img2d[i] = new unsigned char [ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img2d[i][j] = 0;
		}
	}

	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Img2d[i][j] = Img1d[i * ImgWidth + j];
		}
	}
	return(Img2d);
}

/* Convert 1D image data to 3D */
unsigned char*** Img1dTo3d(unsigned char* Img1d, int ImgHeight, int ImgWidth, int BytesPerPixel)
{
	unsigned char*** Img3d =  new unsigned char **[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img3d[i] = new unsigned char *[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img3d[i][j] = new unsigned char[BytesPerPixel]();
			for (int k = 0; k < BytesPerPixel;k++)
			{
				Img3d[i][j][k] = 0;
			}
		}
	}

	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			for (int k = 0; k < BytesPerPixel; k++)
			{
				Img3d[i][j][k] = Img1d[i * ImgWidth * BytesPerPixel + j * BytesPerPixel + k];
			}
		}
	}
	return(Img3d);
}

/* Convert 3D image data to 1D */
unsigned char* Img3dTo1d(unsigned char*** Img3d, int ImgHeight, int ImgWidth, int BytesPerPixel)
{
	unsigned char* Img1d = new unsigned char[ImgHeight*ImgWidth*BytesPerPixel]();
	int m = 0;
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Img1d[m] = Img3d[i][j][0];
			Img1d[m + 1] = Img3d[i][j][1];
			Img1d[m + 2] = Img3d[i][j][2];
			m = m + 3;
		}
	}
	return(Img1d);
}

/* Convert 2D image data to 1D */
unsigned char* Img2dTo1d(unsigned char** Img2d, int ImgHeight, int ImgWidth)
{
	unsigned char* Img1d = new unsigned char[ImgHeight*ImgWidth]();
	for (int i = 0; i < ImgHeight; i++)
	{
		for (int j = 0; j < ImgWidth; j++)
		{
			Img1d[i * ImgWidth + j] = Img2d[i][j];
		}
	}
	return(Img1d);
}

/* read images from file */
void FileRead(char* FileName, unsigned char* Data, long int DataSize)
{
	FILE* file;
	errno_t err;

	if ((err = fopen_s(&file, FileName, "rb")) != 0)	{		cout << "Cannot open file " << FileName << endl;		exit(1);	}	fread(Data, sizeof(unsigned char), DataSize, file);	fclose(file);
	cout << "File " << FileName << " read successfully !!!" << endl;
}

/* write images to file */
void FileWrite(char* FileName, unsigned char* Data, long int DataSize)
{
	FILE* file;
	errno_t err;

	if ((err = fopen_s(&file, FileName, "wb")) != 0)	{		cout << "Cannot open file " << FileName << endl;		exit(1);	}	fwrite(Data, sizeof(unsigned char), DataSize, file);	fclose(file);
	cout << "File " << FileName << " written successfully !!!" << endl;
}

/* calculate Histogram3d Data*/
void Histogram3d(unsigned long HistData[][3], unsigned char ***Img3d, int ImgHeight, int ImgWidth, int BytesPerPixel)
{	
	for (int i = 0;i < 256;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			HistData[i][j] = 0;
		}
	}

	for (int r = 0;r < 256;r++)
	{
		for (int i = 0;i < ImgHeight; i++)
		{
			for (int j = 0;j < ImgWidth; j++)
			{
				for (int k = 0; k < BytesPerPixel; k++)
				{
					if (Img3d[i][j][k] == r)
					{
						HistData[r][k]++;
					}
				}
			}
		}
	}
}

/* calculate Histogram2d Data*/
void Histogram2d(unsigned long HistData[256], unsigned char **Img2d, int ImgHeight, int ImgWidth)
{
	for (int i = 0;i < 256;i++)
	{
		HistData[i] = 0;
	}

	for (int r = 0;r < 256;r++)
	{
		for (int i = 0;i < ImgHeight; i++)
		{
			for (int j = 0;j < ImgWidth; j++)
			{
				if (Img2d[i][j] == r)
				{
					HistData[r]++;
				}
			}
		}
	}
}

/* write histogram3d data to file */
void FileWriteHist3d(char* FileName, unsigned long HistData[][3])
{
	FILE *file;
	file = fopen(FileName, "w");
	if (file != NULL)
	{
		for (int i = 0; i < 256;i++)
		{
			fprintf(file, "%d\t%ld\t%ld\t%ld\n", i, HistData[i][0], HistData[i][1], HistData[i][2]);
		}
		fclose(file);
		cout << "File " << FileName << " written successfully !!!" << endl;
	}
	else
	{
		cout << "Cannot open file " << FileName << endl;
	}
}

/* write histogram2d data to file */
void FileWriteHist2d(char* FileName, unsigned long HistData[256])
{
	FILE *file;
	file = fopen(FileName, "w");
	if (file != NULL)
	{
		for (int i = 0; i < 256;i++)
		{
			fprintf(file, "%d\t%ld\n", i, HistData[i]);
		}
		fclose(file);
		cout << "File " << FileName << " written successfully !!!" << endl;
	}
	else
	{
		cout << "Cannot open file " << FileName << endl;
	}
}

/* Convert 3D image to 2D by extracting R,G,B components */
unsigned char** ExtractChannel(unsigned char*** Img3d, int ImgHeight, int ImgWidth, char Channel)
{
	unsigned char** ImgR = new unsigned char *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		ImgR[i] = new unsigned char[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			ImgR[i][j] = 0;
		}
	}

	unsigned char** ImgG = new unsigned char *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		ImgG[i] = new unsigned char[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			ImgG[i][j] = 0;
		}
	}

	unsigned char** ImgB = new unsigned char *[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		ImgB[i] = new unsigned char[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			ImgB[i][j] = 0;
		}
	}

	for (int i = 0;i < ImgHeight;i++)
	{
		for (int j = 0;j < ImgWidth;j++)
		{
			ImgR[i][j] = Img3d[i][j][0];
			ImgG[i][j] = Img3d[i][j][1];
			ImgB[i][j] = Img3d[i][j][2];
		}
	}

	if (Channel == 'R' || Channel == 'r')
		return ImgR;
	else if (Channel == 'G' || Channel == 'g')
		return ImgG;
	else if (Channel == 'B' || Channel == 'b')
		return ImgB;
	else
		return NULL;
}

/* Convert 2D image to 3D by combining R,G,B components */
unsigned char*** CombineChannel(unsigned char** ImgR, unsigned char** ImgG, unsigned char** ImgB, int ImgHeight, int ImgWidth, int BytesPerPixel)
{
	unsigned char*** Img3d = new unsigned char **[ImgHeight]();
	for (int i = 0;i < ImgHeight;i++)
	{
		Img3d[i] = new unsigned char *[ImgWidth]();
		for (int j = 0;j < ImgWidth;j++)
		{
			Img3d[i][j] = new unsigned char[BytesPerPixel]();
			for (int k = 0;k < BytesPerPixel;k++)
			{
				Img3d[i][j][k] = 0;
			}
		}
	}

	for (int i = 0;i < ImgHeight;i++)
	{
		for (int j = 0;j < ImgWidth;j++)
		{
			Img3d[i][j][0] = ImgR[i][j];
			Img3d[i][j][1] = ImgG[i][j];
			Img3d[i][j][2] = ImgB[i][j];
		}
	}

	return Img3d;
}