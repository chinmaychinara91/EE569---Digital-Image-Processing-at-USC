-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob3b
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

<program_name> <inp_image>.raw <noisy_image>.raw <out_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel> <filtering_method> <similarity_window_size> <search_window_size> <Sigma> <h>
where;
program_name: 	the name of the .exe file generated
inp_image: 	the name of the .raw file along with path to be taken as input for processing
noisy_image: 	the name of the .raw file along with path to be taken as input for noisy image
out_image: 	the name of the .raw file along with path for storing output clean image
ImgWidth: 	Width of the image
ImgHeight: 	Height of the image		
BytesPerPixel: 	bytes per pixel (for RGB = 3)

filtering_methods are as follows: 
nlm:	Non-Local Means Filter

similarity_window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the window around a pixel of the image to be considered for processing
search_window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the search window
Sigma: the standard deviation
h: the filtering parameter

5. Each folder contains the matlab code to plot histograms from the .txt files containing histogram data after running the program as per step-4 above.

6. prob3b_peper.cpp has the code for running NLM for pepper image

7. prob3b_sailboat.cpp has the code for running NLM for sailboat image

8. prob3a.cpp has the code for running all Prob3a methiods used for sailboat image

9. The reason for having three separate files is because I generate some intermediate image files whose names have been hardcoded inside. 
e.g. For just displaying the R component as an image file I have hard coded the name to be pepper_red.raw for any input image given. I could have given it a command line parameter but then there would be too many arguments in the command line which I wanted to avoid. 
-----------------------------------------------------------------------------------------------------------------------------------------------------------------