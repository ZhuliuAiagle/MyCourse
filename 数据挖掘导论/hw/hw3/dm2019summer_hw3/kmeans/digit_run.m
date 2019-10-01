%% digit run
largest_dist=-inf;
smallest_dist=inf;
load('digit_data', 'X');
list = [10,20 ,50];

for i = list
    [idx, ctrs, iter_ctrs] = kmeans(X, i);
    show_digit(ctrs);
end