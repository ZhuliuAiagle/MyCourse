img = imread('sample3.jpg');
fea = double(reshape(img, size(img, 1)*size(img, 2), 3));
% YOUR (TWO LINE) CODE HERE
[idx, ctrs, ~] = kmeans(fea, 64);
fea = ctrs(idx, :);
%
imshow(uint8(reshape(fea, size(img))));
