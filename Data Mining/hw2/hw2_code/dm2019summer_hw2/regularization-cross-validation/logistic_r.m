function w = logistic_r(X, y, lambda)
%LR Logistic Regression.
%
%   INPUT:  X:   training sample features, P-by-N matrix.
%           y:   training sample labels, 1-by-N row vector.
%           lambda: regularization parameter.
%
%   OUTPUT: w    : learned parameters, (P+1)-by-1 column vector.
%

% YOUR CODE HERE
w = rand(size(X, 1) + 1, 1);
rate = 0.1;
x = [ones(1, size(X, 2)); X];
eps = 0.01;  % wucha
iter = 0;
for i=1:100
    w_pre = w;
    step =  sum((-y .* x)./ ( 1 + exp(w_pre' *x.* y)) ,2);
    w = w - rate * (sum(-y ./ ( 1 + exp(w' * x .* y)) .* x,2) + 2 * lambda * w);
    iter = iter + 1;
    fanshu = sum(sum(abs(step)));
    if fanshu <= eps
        break
    end
end
end
