-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob3a
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

<program_name> <inp_image>.raw <noisy_image>.raw <out_image>.raw <ImgWidth> <ImgHeight> <BytesPerPixel> <filtering_method> <window_size> <mask type for lpf filtering method>

where;
program_name: the name of the .exe file generated
inp_image: 	the name of the .raw file along with path to be taken as input for processing
noisy_image: 	the name of the .raw file along with path to be taken as input for noisy image
out_image: 	the name of the .raw file along with path for storing output clean image
ImgWidth: 	Width of the image
ImgHeight: 	Height of the image		
BytesPerPixel: 	bytes per pixel (for RGB = 3)

filtering_methods are as follows: 
lpf = Low Pass Filter
median = Median Filter
gauss = Gaussian Filter
outlier = Outlier Filter
medianmedian = Cascaded Median + Median
outliermedian = Cascaded Outlier + Median
outlierlpf = Cascaded Outlier + LPF
outliermedianlpf = Cascaded Outlier + Median + LPF
outlierlpfmedian = Cascaded Outlier + LPF + Median
outliergauss = Cascaded Outlier + Gaussian

window_size: (WHICH SHOULD BE OF ODD LENGTH) The size of the window around a pixel of the image to be considered for processing
mask type for lpf filtering method: 1 = Type-1, 2 = Type-2
-----------------------------------------------------------------------------------------------------------------------------------------------------------------