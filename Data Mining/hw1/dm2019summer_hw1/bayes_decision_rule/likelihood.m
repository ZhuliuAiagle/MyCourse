function l = likelihood(x)
%LIKELIHOOD Different Class Feature Liklihood 
%
%   INPUT:  x, features of different class, C-By-N vector
%           C is the number of classes, N is the number of different feature
%
%   OUTPUT: l,  likelihood of each feature(from smallest feature to biggest feature) given by each class, C-By-N matrix
%

[C, N] = size(x); % get the size of x
l = zeros(C, N);   % generate a C*N zero martix
%TODO
l = x ./ sum(x,2); % calculate the likelihood and return it
end
