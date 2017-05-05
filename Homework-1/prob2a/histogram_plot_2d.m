clc;
clear;
close all;

% open the files
fid=fopen('tulip_mix_hist.txt');
fid1=fopen('tulip_mix_histeqma.txt');

% scan the file for text and store it in a variable
s=textscan(fid,'%f %f','headerlines',0);
s1=textscan(fid1,'%f %f','headerlines',0);

% close teh files
fclose(fid);
fclose(fid1);

% assign variables for histigram plotting
NBins_inp=s{1};
InpPixel=s{2};
NBins_out=s1{1};
OutPixel=s1{2};

% plot the histogram
subplot(1,2,1);
stem(NBins_inp,InpPixel);
xlabel('Bins');
ylabel('Number of pixels');
title('Histogram tulip-mix');

subplot(1,2,2);
stem(NBins_inp,OutPixel);
xlabel('Bins');
ylabel('Number of pixels');
title('Histogram equalised tulip-mix method B');