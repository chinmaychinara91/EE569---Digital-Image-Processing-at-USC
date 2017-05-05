clc;
clear;
close all;

% open the files
fid=fopen('tulip_mix_tf_pixelmap_mb.txt');

% scan the file for text and store it in a variable
s=textscan(fid,'%f %f','headerlines',0);

% close the files
fclose(fid);

% assign variables for data storage
Input=s{1};
Output=s{2};

% plot the transfer function
plot(Input, Output);
xlabel('Input (F)');
ylabel('Output (G)');
title('Transfer function showing pixel mapping for tulip-mix by Method-B');