%% Ridge Regression
load('digit_train', 'X', 'y');
% Do feature normalization:row wise
std1 = std(X,0,2);
std1(std1 == 0) = 1;
X = (X - mean(X,2)) ./std1;
% Do feature normalization
% ...

% Do LOOCV
lambdas = [1e-3, 1e-2, 1e-1, 0, 1, 1e1, 1e2, 1e3];
E = zeros(size(lambdas));
ws = zeros(size(X, 1) + 1, size(lambdas, 2));
for i = 1:length(lambdas)
    E_val = 0;
    for j = 1:size(X, 2)
        X_rest = X(:, [1:j-1,j+1:end]); Y_rest = y([1:j-1, j+1:end]); % take point j out of X
        w = ridge(X_rest, Y_rest, lambdas(i));
        E_val = E_val + (w' * [1; X(:,j)] * y(j) < 0);
    end
    E(i) = E_val;
    w = ridge(X, y, lambdas(i));
    ws(:,i) = w;
end

% Compute training error

load('digit_test', 'X_test', 'y_test');
[~, best_idx] = min(E);
% Do feature normalization
% Do feature normalization:row wise
std1 = std(X_test,0,2);
std1(std1 == 0) = 1;
X_test = (X_test - mean(X_test,2)) ./ std1;
E
% Do the sum of ws
wss = ws .* ws;
s_wss = sum(wss)  % show the sum of ws
% Compute test error
train_error = sum(sum(ws(:, best_idx) .* [ones(1, size(X, 2)); X], 1).* y < 0) ;
train_error = train_error / size(X, 2);
test_error = sum(sum(ws(:, best_idx) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0);
test_error = test_error / size(X_test, 2);
lambda = lambdas(best_idx);
fprintf('lambda is %f, E_train is %f, E_test is %f.\n', lambda, train_error, test_error);
no_regularzation = find(lambdas == 0);
train_error = sum(sum(ws(:, no_regularzation) .* [ones(1, size(X, 2)); X], 1).* y < 0) ;
train_error = train_error / size(X, 2);
test_error = sum(sum(ws(:, no_regularzation) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0);
test_error = test_error / size(X_test, 2);
fprintf('Without regularization, E_train is %f, E_test is %f.\n', train_error, test_error);

%% Logistic
load('digit_train', 'X', 'y');
% Do feature normalization:row wise
std1 = std(X,0,2);
std1(std1 == 0) = 1;
X = (X - mean(X,2)) ./std1;
% Do feature normalization
% ...

% Do LOOCV
lambdas = [1e-3, 1e-2, 1e-1, 0, 1, 1e1, 1e2, 1e3];
E = zeros(size(lambdas));
ws = zeros(size(X, 1) + 1, size(lambdas, 2));
for i = 1:length(lambdas)
    E_val = 0;
    for j = 1:size(X, 2)
        X_rest = X(:, [1:j-1,j+1:end]); Y_rest = y([1:j-1, j+1:end]); % take point j out of X
        w = logistic_r(X_rest, Y_rest, lambdas(i));
        E_val = E_val + (w' * [1; X(:,j)] * y(j) < 0);
    end
    E(i) = E_val;
    ws(:,i) = w;
end

% Compute training error

load('digit_test', 'X_test', 'y_test');
[~, best_idx] = min(E);
E
% Do feature normalization:row wise
std1 = std(X_test,0,2);
std1(std1 == 0) = 1;
X_test = (X_test - mean(X_test,2)) ./ std1;
% Compute test error
train_error = sum(sum(ws(:, best_idx) .* [ones(1, size(X, 2)); X], 1).* y < 0);
train_error = train_error / size(X, 2);
test_error = sum(sum(ws(:, best_idx) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0);
test_error = test_error / size(X_test, 2);
lambda = lambdas(best_idx);
fprintf('lambda is %f, E_train is %f, E_test is %f.\n', lambda, train_error, test_error);
no_regularzation = find(lambdas == 0);
train_error = sum(sum(ws(:, no_regularzation) .* [ones(1, size(X, 2)); X], 1).* y < 0) ;
train_error = train_error / size(X, 2);
test_error = sum(sum(ws(:, no_regularzation) .* [ones(1, size(X_test, 2)); X_test], 1).* y_test < 0);
test_error = test_error / size(X_test, 2);
fprintf('Without regularization, E_train is %f, E_test is %f.\n', train_error, test_error);