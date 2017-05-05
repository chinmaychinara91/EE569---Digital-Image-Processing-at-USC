clc;
clear;
close all;

% open the files
fid=fopen('bedroom_mb_hist.txt');
fid1=fopen('bedroom_mb_eq_hist.txt');

% scan the file for text and store it in a variable
s=textscan(fid,'%f %f %f %f','headerlines',0);
s1=textscan(fid1,'%f %f %f %f','headerlines',0);

% close the files
fclose(fid);
fclose(fid1);

% assign variables for histogram plotting
NBins_inp=s{1};
InpR=s{2};
InpG=s{3};
InpB=s{4};
NBins_out=s1{1};
OutR=s1{2};
OutG=s1{3};
OutB=s1{4};

% plot the histogram
figure('Name','Red');
subplot(1,2,1);
stem(NBins_inp,InpR);
xlabel('Pixel intensity');
ylabel('Number of pixels');
title('Histogram (Red) component input');

subplot(1,2,2);
stem(NBins_out,OutR);
xlabel('Pixel intensity');
ylabel('Number of pixels');
title('Histogram equalized (Red) component method B');

figure('Name','Green');
subplot(1,2,1);
stem(NBins_inp,InpG);
xlabel('Pixel intensity');
ylabel('Number of pixels');
title('Histogram (Green) component input');

subplot(1,2,2);
stem(NBins_out,OutG);
xlabel('Pixel intensity');
ylabel('Number of pixels');
title('Histogram equalized (Green) component method B');

figure('Name','Blue');
subplot(1,2,1);
stem(NBins_inp,InpB);
xlabel('Pixel intensity');
ylabel('Number of pixels');
title('Histogram (Blue) component input');

subplot(1,2,2);
stem(NBins_out,OutB);
xlabel('Pixel intensity');
ylabel('Number of pixels');
title('Histogram equalized (Blue) component method B');