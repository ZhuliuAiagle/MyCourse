function idx = spectral(W, k)
%SPECTRUAL spectral clustering
%   Input:
%     W: Adjacency matrix, N-by-N matrix
%     k: number of clusters
%   Output:
%     idx: data point cluster labels, n-by-1 vector.

% YOUR CODE HERE


D = eye(size(W));
D = D .* sum(W,2);
[Y, ~] = eigs(D - W, D, 2, 'smallestabs');
idx = kmeans(Y(:,2), k);
%L = D - W;
%L = pinv(full((sqrt(D)))) * L *  pinv(full((sqrt(D))));
%[Y, ~] = eigs(L, k, 'smallestabs');
%idx = kmeans(Y, k);

end
