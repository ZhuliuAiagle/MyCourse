% You can use this skeleton or write your own.
% You are __STRONGLY__ suggest to run this script section-by-section using Ctrl+Enter.
% See http://www.mathworks.cn/cn/help/matlab/matlab_prog/run-sections-of-programs.html for more details.

%%load data
load('data');
x_all = cat(2, x1_train, x1_test, x2_train, x2_test);
range = [min(x_all), max(x_all)];
train_x = get_x_distribution(x1_train, x2_train, range);
test_x = get_x_distribution(x1_test, x2_test, range); 

x1_all = cat(2, x1_train, x1_test);
x2_all = cat(2,x2_train, x2_test);
all_x = get_x_distribution(x1_all, x2_all, range);
 

%% Part1 likelihood: 
l = likelihood(train_x);


bar(range(1):range(2), l');
xlabel('x');
ylabel('P(x|\omega)');
axis([range(1) - 1, range(2) + 1, 0, 0.5]);

%TODO

testError = 0;
for i=1:size(l,2)
    if l(1,i) > l(2,i)
        testError = testError + test_x(2,i);
    elseif l(1,i) < l(2,i)
        testError = testError + test_x(1,i);
    end
end

fprintf("Error = %d.\n", testError);
%compute the number of all the misclassified x using maximum likelihood decision rule

%% Part2 posterior:
p = posterior(train_x);

bar(range(1):range(2), p');
xlabel('x');
ylabel('P(\omega|x)');
axis([range(1) - 1, range(2) + 1, 0, 1.2]);

%TODO

testError_2 = 0;
for i=1:size(l,2)
    if p(1,i) > p(2,i)
        testError_2 = testError_2 + test_x(2,i);
    elseif p(1,i) < p(2,i)
        testError_2 = testError_2 + test_x(1,i);
    end
end

fprintf("Error = %d.\n", testError_2);

%compute the number of all the misclassified x using optimal bayes decision rule

%% Part3 risk:
risk = [0, 1; 2, 0];
%TODO
p_all = posterior(all_x); 
sum_1 = sum(all_x, 1);
p_x = sum_1 ./ sum(sum_1);
double_p_x = cat(1, p_x, p_x);
p_all_x = p_all .* p_x;
bayesRisk = risk * p_all_x; % risk (change the order of the roll)

minBayesRisk = sum(min(bayesRisk));
fprintf("Min Risk = %f.\n", minBayesRisk);
%get the minimal risk using optimal bayes decision rule and risk weights
