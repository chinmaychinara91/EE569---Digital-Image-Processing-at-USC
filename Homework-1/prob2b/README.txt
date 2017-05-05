-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob2b
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

<program_name> <inp_image>.raw <inp_hist>.txt <out_image>.raw <out_hist>.txt <ImgWidth> <ImgHeight> <BytesPerPixel> <method_name>

where;
program_name: 		the name of the .exe file generated
inp_image: 		the name of the .raw file along with path to be taken as input for processing
inp_hist: 		the name of the .txt file along with path to to which input image histogram data is saved
out_image: 		the name of the .raw file along with path to which the processesed image is saved
out_hist: 		the name of the .txt file along with path to to which output image histogram data is saved
ImgWidth: 		Width of the image
ImgHeight: 		Height of the image		
BytesPerPixel: 		bytes per pixel (for RGB = 3 and for GRAY = 1)	
method_name: 		ma = Method-A, mb = Method-B		
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob2b_rgb_hsv_rgb
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

<program_name> <inp_image>.raw <inp_hist>.txt <out_image>.raw <out_hist>.txt <ImgWidth> <ImgHeight> <BytesPerPixel>

where;
program_name: 		the name of the .exe file generated
inp_image: 		the name of the .raw file along with path to be taken as input for processing
inp_hist: 		the name of the .txt file along with path to to which input image histogram data is saved
out_image: 		the name of the .raw file along with path to which the processesed image is saved
out_hist: 		the name of the .txt file along with path to to which output image histogram data is saved
ImgWidth: 		Width of the image
ImgHeight: 		Height of the image		
BytesPerPixel: 		bytes per pixel (for RGB = 3 and for GRAY = 1)	
-----------------------------------------------------------------------------------------------------------------------------------------------------------------