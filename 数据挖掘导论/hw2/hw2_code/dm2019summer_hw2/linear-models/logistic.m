function w = logistic(X, y)
%LR Logistic Regression.
%
%   INPUT:  X:   training sample features, P-by-N matrix.
%           y:   training sample labels, 1-by-N row vector.
%
%   OUTPUT: w    : learned parameters, (P+1)-by-1 column vector.
%

% YOUR CODE HERE
w = rand(size(X, 1) + 1, 1);
X_addon = [ones(1,size(X, 2));X];
descent_rate = 0.1;
eps = 0.1;  % wucha
iter = 0;
for i = 1:1000
    w_pre = w;
    step =  sum((-y .* X_addon)./ ( 1 + exp(w_pre' * X_addon .* y)) ,2);
    w = w - descent_rate * sum((-y .* X_addon)./ ( 1 + exp(w' * X_addon .* y)) ,2);
    fanshu = sum(sum(abs(step)));
    iter = iter + 1;
    if fanshu <= eps
        break
    end
end
end

