-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob1a3
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

<program_name> <OriginalImage>.raw <ForegroundImage>.raw <ImgWidth1> <ImgHeight1> <BytesPerPixel1> <NewBackgroundImage>.raw <CompositeImage>.raw <ImgWidth2> <ImgHeight2> <BytesPerPixel2>

where;
program_name: 		the name of the .exe file generated
OriginalImage: 		the name of the .raw file along with path to be taken as input for processing
ForegroundImage: 	the name of the .raw file along with path to which the background removed image is saved
ImgWidth1: 		Width of the smaller image
ImgHeight1: 		Height of the smaller image
BytesPerPixel1: 	bytes per pixel of smaller image (for RGB = 3 and for GRAY = 1)		
NewBackgroundImage: 	the name of the .raw file along with path to be taken as input for background of composite image
CompositeImage: 	the name of the .raw file along with path to which the composite image is saved
ImgWidth2: 		Width of the larger image
ImgHeight2: 		Height of the larger image		
BytesPerPixel2: 	bytes per pixel of larger image (for RGB = 3 and for GRAY = 1)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------