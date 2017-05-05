-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob1b3
Date: February 5, 2017
Name : Chinmay Chinara
ID : 2527-2374-52
email : chinara@usc.edu
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
SYSTEM CONFIGURATION IN WHICH THIS CODE RAN:

OS: Windows 10 64-bit

Software: Visual Studio 2015 Community (Visual C++)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
STEPS TO DEBUG CODE:

1. Open "Developer Command Prompt for VS2015".

2. Go to the directory that has the .cpp and .h files using "cd" command.

3. To compile code and generate .obj type as below:
   "cl /EHsc <program_name>.cpp"

4. To run type the following in command prompt after doing a successful compilation in step-3:

<program_name> <in_RGB>.raw <out_gray>.raw <out_sepia>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>

where;
program_name: 	the name of the .exe file generated
in_RGB: 	the name of the .raw file along with path of the RGB file to be taken as input for processing
out_gray: 	the name of the .raw file along with path to which the gray image is saved
out_sepia: 	the name of the .raw file along with path to which the sepia filtered image is saved
ImgWidth: 	Width of the image" << endl;
ImgHeight: 	Height of the image" << endl;		
BytesPerPixel: 	bytes per pixel (for RGB = 3)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------