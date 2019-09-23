function w = linear_regression(X, y)
%LINEAR_REGRESSION Linear Regression.
%
%   INPUT:  X: training sample features, P-by-N matrix.
%           y: training sample labels, 1-by-N row vector.
%
%   OUTPUT: w:    learned perceptron parameters, (P+1)-by-1 column vector.
%

% YOUR CODE HERE
X_addon = [ones(1,size(X,2));X];
w = (y * X_addon' * (X_addon * X_addon')^-1)';
end
