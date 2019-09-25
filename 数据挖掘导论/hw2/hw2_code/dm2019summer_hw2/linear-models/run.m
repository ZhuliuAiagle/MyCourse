% You can use this skeleton or write your own.
% You are __STRONGLY__ suggest to run this script section-by-section using Ctrl+Enter.
% See http://www.mathworks.cn/cn/help/matlab/matlab_prog/run-sections-of-programs.html for more details.
% NOTE: Be becareful that the bias term is in the first element of weight, that is y = sign(w' * [ones[1, N]; X])


%% Part1: Preceptron
nRep = 1000; % number of replicates
nTrain = 100; % number of training data

% added varibles
nTest = 100; % number of testing data
avgIter = 0; % average number of interation
train_error = 0;
test_error = 0;

dim = 2; %default dim, but acutally we get the actual dim from X

for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    dim = size(X, 1); % get the dim for generate X_test
    [w_g, iter] = perceptron(X, y);
    % Compute training, testing error
    X_addon = [ones(1, nTrain); X]; % addon a line of 1 to suit w
    train_error = train_error + sum(sum(w_g .* X_addon, 1).*y < 0);
    X_test_raw = rand(2, nTest)*(2) - 1; %[-1,1]
    X_test = [ones(1, nTest);X_test_raw];
    y_test = sign(w_f'* X_test);
    test_error = test_error + sum(sum(w_g .* X_test, 1) .* y_test < 0);
    % Sum up number of iterations
    avgIter = avgIter + iter; % times of any perceptron iter
end


train_error = train_error / ( nRep * nTrain);
test_error = test_error / (nRep * nTest);
avgIter = avgIter / nRep;

fprintf('==================================\n');
fprintf('Preceptron:\n');
fprintf('E_train is %f, E_test is %f.\n',train_error, test_error);
fprintf('Average number of iterations is %d.\n', round(avgIter));
plotdata(X, y, w_f, w_g, 'Pecertron');

%% Part2: Preceptron: Non-linearly separable case: cause endless loop
nRep = 1000; % number of replicates
nTrain = 100; % number of training data

% added varibles
nTest = 100; % number of testing data
avgIter = 0; % average number of interation
train_error = 0;
test_error = 0;

dim = 2; %default dim, but acutally we get the actual dim from X

for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain,'noisy');
    dim = size(X, 1); % get the dim for generate X_test
    [w_g, iter] = perceptron(X, y);
    % Compute training, testing error
    X_addon = [ones(1, nTrain); X]; % addon a line of 1 to suit w
    train_error = train_error + sum(sum(w_g .* X_addon, 1).*y < 0);
    X_test_raw = rand(2, nTest)*(2) - 1; %[-1,1]
    X_test = [ones(1, nTest);X_test_raw];
    y_test = sign(w_f'* X_test);
    %TEST: ADD SOME NOISE
     idx = randsample(nTest,nTest/10);
     y_test(idx) = -y_test(idx);
    test_error = test_error + sum(sum(w_g .* X_test, 1) .* y_test < 0);
    % Sum up number of iterations
    avgIter = avgIter + iter; % times of any perceptron iter
end


train_error = train_error / ( nRep * nTrain);
test_error = test_error / (nRep * nTest);
avgIter = avgIter / nRep;

fprintf('==================================\n');
fprintf('Preceptron:\n');
fprintf('E_train is %f, E_test is %f.\n',train_error, test_error);
fprintf('Average number of iterations is %d.\n', round(avgIter));
plotdata(X, y, w_f, w_g, 'Pecertron-noisy');


%% Part3: Linear Regression
nRep = 1000; % number of replicates
nTrain = 100; % number of training data

%added varibles
nTest = 100; % number of testing data
train_error = 0;
test_error = 0;

for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    dim = size(X, 1);
    w_g = linear_regression(X, y);
    
    % Compute training, testing error
     X_addon = [ones(1, nTrain); X]; % addon a line of 1 to suit w
     train_error = train_error + sum((w_g' * X_addon).*y < 0);
     X_test = [ones(1, nTest); rand(dim, nTest)*2-1];
     y_test = sign(w_f'* X_test);
     
     test_error = test_error + sum(sum(w_g .* X_test, 1) .* y_test < 0);
     % Sum up number of iterations
end

train_error = train_error / (nRep * nTrain);
test_error = test_error / (nRep * nTest);


fprintf('==================================\n');
fprintf('Linear Regression:\n');
fprintf('E_train is %f, E_test is %f.\n',train_error, test_error);
plotdata(X, y, w_f, w_g, 'Linear Regression');

%% Part4: Linear Regression: noisy
nRep = 1000; % number of replicates
nTrain = 100; % number of training data

%added varibles
nTest = 100; % number of testing data
train_error = 0;
test_error = 0;

for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain,'noisy');
    dim = size(X, 1);
    w_g = linear_regression(X, y);
    
    % Compute training, testing error
     X_addon = [ones(1, nTrain); X]; % addon a line of 1 to suit w
     train_error = train_error + sum((w_g' * X_addon).*y < 0); % transform to logic array
     X_test = [ones(1, nTest);  rand(dim, nTest)*2-1];
     y_test = sign(w_f'* X_test);
     %TEST: ADD SOME NOISE
     idx = randsample(nTest,nTest/10);
     y_test(idx) = -y_test(idx);
     test_error = test_error + sum(sum(w_g .* X_test, 1) .* y_test < 0);
     % Sum up number of iterations
end

train_error = train_error / (nRep * nTrain);
test_error = test_error / (nRep * nTest);

fprintf('==================================\n');
fprintf('Linear Regression(noisy):\n');
fprintf('E_train is %f, E_test is %f.\n',train_error, test_error);
plotdata(X, y, w_f, w_g, 'Linear Regression:noisy');


%% Part5: Linear Regression: poly_fit

load('poly_train', 'X', 'y');
load('poly_test', 'X_test', 'y_test');
w = linear_regression(X, y);
% Compute training, testing error
train_error = sum(sum(w .* [ones(1, size(X, 2)); X], 1).* y < 0);
train_error = train_error / size(X, 2);
test_error = sum(sum(w .* [ones(1, size(X_test, 2)); X_test], 1) .* y_test < 0);
test_error = test_error / size(X_test,2);
fprintf('==================================\n');
fprintf('Linear Regression(poly_fit):\n');
fprintf('@ NO TRANSFORM: E_train is %f, E_test is %f.\n', train_error, test_error);

% add a transform
% do transformation 
X_t = [X; X(1,:) .* X(2,:); X(1,:).^2; X(2,:).^2]; 
X_test_t = [X_test; X_test(1, :) .* X_test(2, :); X_test(1, :).^2; X_test(2, :).^2]; 
%actual code
w = linear_regression(X_t, y);
train_error = sum(sum(w .* [ones(1, size(X_t, 2)); X_t], 1).* y < 0);
train_error = train_error / size(X_t, 2);
test_error = sum(sum(w .* [ones(1, size(X_test_t, 2)); X_test_t], 1) .* y_test < 0);
test_error = test_error / size(X_test_t,2);
fprintf('@ TRANSFORM: E_train is %f, E_test is %f.\n', train_error, test_error);


%% Part6: Logistic Regression
% Since logistic regression outputs 0/1, we should adjust the label y.
nRep = 100; % number of replicates
nTrain = 100; % number of training data

%added varibles
nTest = 100; % number of testing data
train_error = 0;
test_error = 0;

for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    w_g = logistic(X, y);
    dim = size(X, 1);
    % Compute training, testing error
    X_addon = [ones(1, nTrain); X]; % addon a line of 1 to suit w
     train_error = train_error + sum((w_g' * X_addon).*y < 0); % transform to logic array
     X_test = [ones(1, nTest);  rand(dim, nTest)*2-1];
     y_test = sign(w_f'* X_test);
     test_error = test_error + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end

train_error = train_error / (nRep * nTrain);
test_error = test_error / (nRep * nTest);

fprintf('==================================\n');
fprintf('Logistic Regression:\n');
fprintf('E_train is %f, E_test is %f.\n',train_error, test_error);
plotdata(X, y, w_f, w_g, 'Logistic Regression');

%% Part7: Logistic Regression: noisy
% Since logistic regression outputs 0/1, we should adjust the label y.
nRep = 100; % number of replicates
nTrain = 100; % number of training data
nTest = 10000; % number of training data

train_error = 0;
test_error = 0;

for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain, 'noisy');
    w_g = logistic(X, y);
    dim = size(X, 1);
    % Compute training, testing error
    X_addon = [ones(1, nTrain); X]; % addon a line of 1 to suit w
     train_error = train_error + sum((w_g' * X_addon).*y < 0); % transform to logic array
     X_test = [ones(1, nTest); rand(dim, nTest)*(2) - 1];
     y_test = sign(w_f'* X_test);
     %TEST: ADD SOME NOISE
     idx = randsample(nTest,nTest/10);
     y_test(idx) = -y_test(idx);
     test_error = test_error + sum(sum(w_g .* X_test, 1) .* y_test < 0);
end
train_error = train_error / (nRep * nTrain);
test_error = test_error / (nRep * nTest);

fprintf('==================================\n');
fprintf('Logistic Regression(noisy):\n');
fprintf('E_train is %f, E_test is %f.\n',train_error, test_error);
plotdata(X, y, w_f, w_g, 'Logistic Regression: noisy');

%% Part8: SVM
nRep = 1000; % number of replicates
nTrain = 100; % number of training data

%added varibles
nTest = 100; % number of testing data
num_sv = 0;
train_error = 0;
test_error = 0;

for i = 1:nRep
    [X, y, w_f] = mkdata(nTrain);
    [w_g, num_sc] = svm(X, y);  
    % Compute training, testing error
    train_error = train_error + sum(sum(w_g .* [ones(1, nTrain); X], 1).* y < 0);
    X_test = [ones(1, nTest); rand(2, nTest)*(2)-1];
    y_test = sign(w_f'* X_test);
    test_error = test_error + sum(sum(w_g .* X_test, 1) .* y_test < 0);
    % Sum up number of support vectors
     num_sv = num_sv + num_sc;
end
train_error = train_error / (nRep * nTrain);
test_error = test_error / (nRep * nTest);
num_sv = num_sv / nRep;
fprintf('==================================\n');
fprintf('Support Vector Machine:\n');
fprintf('E_train is %f, E_test is %f.\n', train_error, test_error);
fprintf('Average number of supported vectors is %d.\n', round(num_sv));

plotdata(X, y, w_f, w_g, 'SVM');
