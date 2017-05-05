clc;
clear;
close all;

% open the files
fid=fopen('forest_2_hist.txt');
fid1=fopen('gaussian_pdf_hist.txt');
fid2=fopen('forest_2_histmatch.txt');

% scan the file for text and store it in a variable
s=textscan(fid,'%f %f %f %f','headerlines',0);
s1=textscan(fid1,'%f %f %f %f','headerlines',0);
s2=textscan(fid2,'%f %f %f %f','headerlines',0);

% close the files
fclose(fid);
fclose(fid1);
fclose(fid2);

% assign variables for histogram plotting
NBins_inp=s{1};
InpR=s{2};
InpG=s{3};
InpB=s{4};

NBins_gauss=s1{1};
GaussR=s1{2};
GaussG=s1{3};
GaussB=s1{4};

NBins_out=s2{1};
OutR=s2{2};
OutG=s2{3};
OutB=s2{4};

% plot the histogram
figure('Name','Red');
subplot(1,3,1);
stem(NBins_inp,InpR);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram (Red) component input');

subplot(1,3,2);
stem(NBins_gauss,GaussR);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Gaussian PDF plot');

subplot(1,3,3);
stem(NBins_out,OutR);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram matched (Red) component');

figure('Name','Green');
subplot(1,3,1);
stem(NBins_inp,InpG);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram (Green) component input');

subplot(1,3,2);
stem(NBins_gauss,GaussG);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Gaussian PDF plot');

subplot(1,3,3);
stem(NBins_out,OutG);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram matched (Green) component');

figure('Name','Blue');
subplot(1,3,1);
stem(NBins_inp,InpB);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram (Blue) component input');

subplot(1,3,2);
stem(NBins_gauss,GaussB);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Gaussian PDF plot');

subplot(1,3,3);
stem(NBins_out,OutB);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram matched (Blue) component');