function digits = hack(img_name)
%HACK Recognize a CAPTCHA image
%   Inputs:
%       img_name: filename of image
%   Outputs:
%       digits: 1x5 matrix, 5 digits in the input CAPTCHA image.

load('hack_data');
% YOUR CODE HERE
image_for_test = extract_image(img_name);
% knn
digits = knn(image_for_test, X, Y, 2);
% new a figure, in order not to cover the old figure
figure;
%show
show_image(image_for_test);
% title
title(num2str(digits));
hold off;
end