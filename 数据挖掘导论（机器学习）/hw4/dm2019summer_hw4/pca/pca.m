function [eigvector, eigvalue] = pca(data)
%PCA	Principal Component Analysis
%
%             Input:
%               data       - Data matrix. Each row vector of fea is a data point.
%
%             Output:
%               eigvector - Each column is an embedding function, for a new
%                           data point (row vector) x,  y = x*eigvector
%                           will be the embedding result of x.
%               eigvalue  - The sorted eigvalue of PCA eigen-problem.
%

% YOUR CODE HERE
cov_martix = cov(data);
size(cov_martix)
[eigvector, eigvalue] = eig(cov_martix, 'vector');
% sort
[eigvalue, index] = sort(eigvalue, 1, 'descend');
% sort the vector according to value (index)
eigvector = eigvector(:, index);
end