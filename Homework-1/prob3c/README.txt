-----------------------------------------------------------------------------------------------------------------------------------------------------------------
EE569 Homework Assignment #1 Prob3c
Date: February 5, 2017
Name : Chinmay Chinara
ID : 2527-2374-52
email : chinara@usc.edu
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------
STEPS TO DEBUG CODE:

1. Open the BM3D folder

2. There are 3 scripts of which BM3D.m is as per the author and two scripts combineRGB.m and convertPNG.m is implemented by me.

3. Use the ‘convertPNG.m’ script to first to read the raw file and convert it to PNG and saving the image file along with the R, G, B components extracted all into different files. 

4. In ‘BM3D.m’ at line numbers 92 to 98. Select the correct R, G, B component file name as saved in step-3 and give a new name to the BM3D denoised file that needs to be saved to the 'name_out' variable at line 93.

5. Use the 'combineRGB.m' script to combine all the png images in step-4 above to get the final clean/denoised image.

6. Another way I tried is by using CBM3D.m directly.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------