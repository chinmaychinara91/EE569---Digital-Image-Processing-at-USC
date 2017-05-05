-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob1a2
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

<program_name> <input_mirror_image>.raw <output_image200*200>.raw <output_image400*400>.raw <output_image600*600>.raw <ImgWidth> <ImgHeight> <BytesPerPixel>

where;
program_name: 		the name of the .exe file generated
input_mirror_image: 	the name of the mirror .raw file along with path to be taken as input for processing
output_image200*200: 	the name of the 200*200 .raw file along with path to which the processesed image is saved
output_image400*400: 	the name of the 400*400 .raw file along with path to which the processesed image is saved
output_image600*600: 	the name of the 600*600 .raw file along with path to which the processesed image is saved
ImgWidth: 		Width of the input image
ImgHeight: 		Height of the input image	
BytesPerPixel: 		bytes per pixel (for RGB = 3)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------