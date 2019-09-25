function [w, iter] = perceptron(X, y)
%PERCEPTRON Perceptron Learning Algorithm.
%
%   INPUT:  X: training sample features, P-by-N matrix.
%           y: training sample labels, 1-by-N row vector.
%
%   OUTPUT: w:    learned perceptron parameters, (P+1)-by-1 column vector.
%           iter: number of iterations
%

% YOUR CODE HERE
finish = 0;
iter = 0;
w = [1; rand(size(X, 1),1)];
while(finish == 0 && iter < 10000)
    iter = iter + 1;
    finish = 1;
    for i = 1:size(X,2)
        x = [1; X(:, i)];       % if right
        if(w' * x * y(i) < 0) % if not right
            w = w + x * y(i);   % gradient descent
            finish = 0;        % till all the w' * x * y(i) > 0
        end
    end
end
end
