% imshow(hack_pca('1.gif'));

img_r = double(imread('1.gif'));
% YOUR CODE HERE
[x, y] = find(img_r<255);
a = [x,y];
[eigvector, ~] = pca([x y]);
eigvector
idx = round([x y] * eigvector);
b = idx;
idx(:,2) = -idx(:,2);
idx = idx - min(idx) + 2;
img = ones(max(idx(:,2)), max(idx(:,1))) * 255;
img(sub2ind(size(img), idx(:,2), idx(:,1))) = img_r(sub2ind(size(img_r), x, y));
img = uint8(img);
imshow(img);
