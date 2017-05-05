clc;
clear;
close all;

% open the files
fid=fopen('input_hist.txt');

% scan the file for text and store it in a variable
s=textscan(fid,'%f %f %f %f','headerlines',0);

% close the files
fclose(fid);
% assign variables for histogram plotting
NBins_inp=s{1};
InpR=s{2};
InpG=s{3};
InpB=s{4};

% plot the histogram
figure('Name','Red');
stem(NBins_inp,InpR);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram (Red) component input');

figure('Name','Green');
stem(NBins_inp,InpG);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram (Green) component input');

figure('Name','Blue');
stem(NBins_inp,InpB);
xlabel('Grayscale values');
ylabel('Number of pixels');
title('Histogram (Blue) component input');