function W = knn_graph(X, k, threshold)
%KNN_GRAPH Construct W using KNN graph
%   Input: X - data point features, n-by-p matrix.
%          k - number of nn.
%          threshold - distance threshold.
%
%   Output:W - adjacency matrix, n-by-n matrix.

% YOUR CODE HERE
D = EuDist2(X);
W = (D <= max(mink(D, k+1)));
W = W & (D <= threshold) ;
W = W - eye(size(D)); % clear the distance to itself
W = double(W | W');  % merge the two-direction matrix
end
