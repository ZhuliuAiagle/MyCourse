function [idx, ctrs, iter_ctrs] = kmeans(X, K)
%KMEANS K-Means clustering algorithm
%
%   Input: X - data point features, n-by-p maxtirx.
%          K - the number of clusters
%
%   OUTPUT: idx  - cluster label
%           ctrs - cluster centers, K-by-p matrix.
%           iter_ctrs - cluster centers of each iteration, K-by-p-by-iter
%                       3D matrix.

% YOUR CODE HERE
    % initialize
    ctrs = datasample(X, K, 1, 'Replace', false);
    iter_ctrs = ctrs;
    idx = zeros(1, size(X,1));
    while(true)
        % caculate p-distance
        [~, newIdx] = pdist2(ctrs, X, 'euclidean','Smallest', 1);
        % break
        if(newIdx == idx)
            break;
        end
        % calcuate means of satifying rows of X
        for i=1:K
            t = X(newIdx == i, :);
            ctrs(i,:) = mean(t);
        end
        % update iter_ctrs
        iter_ctrs = cat(3, iter_ctrs, ctrs);
        % update idx
        idx = newIdx;
    end
end
