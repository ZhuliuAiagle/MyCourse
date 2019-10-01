%% Prereq
%ham_train contains the occurrences of each word in ham emails. 1-by-N vector
ham_train = csvread('ham_train.csv');
%spam_train contains the occurrences of each word in spam emails. 1-by-N vector
spam_train = csvread('spam_train.csv');
%N is the size of vocabulary.
N = size(ham_train, 2);
%There 9034 ham emails and 3372 spam emails in the training samples
num_ham_train = 9034;
num_spam_train = 3372;
%Do smoothing
x = [ham_train;spam_train] + 1;

%ham_test contains the occurences of each word in each ham test email. P-by-N vector, with P is number of ham test emails.
load ham_test.txt;
ham_test_tight = spconvert(ham_test);
ham_test = sparse(size(ham_test_tight, 1), size(ham_train, 2));
ham_test(:, 1:size(ham_test_tight, 2)) = ham_test_tight;
%spam_test contains the occurences of each word in each spam test email. Q-by-N vector, with Q is number of spam test emails.
load spam_test.txt;
spam_test_tight = spconvert(spam_test);
spam_test = sparse(size(spam_test_tight, 1), size(spam_train, 2));
spam_test(:, 1:size(spam_test_tight, 2)) = spam_test_tight;

%TODO
%Implement a ham/spam email classifier, and calculate the accuracy of your classifier

%% Caculate the top 10 words on ratio(1)
l = likelihood(x);
[values, index] = sort(l(2,:)./l(1,:),'descend'); % sort
top_10 = index(1:10);
top_10_values = values(1:10);

%% learn(2,4)
% ----
tp = 0; 
fp = 0; 
fn = 0; 
tn = 0;
spam_test_count = size(spam_test, 1);
ham_test_count = size(ham_test, 1);
l_ham = log(num_ham_train / (num_ham_train + num_spam_train));
l_spam = log(num_spam_train / (num_ham_train + num_spam_train));
% ---
for i=1:spam_test_count
    if spam_test(i,:) * log(l(2,:)') + l_spam >  l_ham + spam_test(i,:)*log(l(1,:)')
        tp = tp + 1;
    else
        fn = fn + 1;
    end
end
for i=1:num_ham_test
    if ham_test(i,:) * log(l(1,:)') + l_ham  >  l_spam + ham_test(i,:)*log(l(2,:)')
        tn = tn + 1;
    else
        fp = fp + 1;
    end
end

%% result(2,4)
accuracy = (tp + tn) / (num_ham_test + spam_test_count);
precision = tp / (tp + fp);
recall = tp / (tp + fn);
fprintf("accuracy= %f.\n",accuracy);
fprintf("precision= %f.\n",precision);
fprintf("recall= %f.\n",recall);