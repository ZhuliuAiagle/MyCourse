%% SD
load('kmeans_data', 'X');

% ini
ld =-inf;
sd =inf;
ctrs = [];
this_d = 0;

for iter = 1:1000
    % get new idx, ctrs and iter_ctrs
    [idx, new_ctrs, iter_ctrs] = kmeans(X, 2);
    size_ctrs = size(iter_ctrs, 3);
    %calculate distance
    for i = 2:size_ctrs
        temp = (iter_ctrs(:,:,i-1) - iter_ctrs(:,:,i)).^2;
        this_d = this_d + temp;
    end
    % larger than largest
    if(this_d > ld)
        l_ctrs = new_ctrs;l_iter_ctrs = iter_ctrs;
        l_idx = idx;ld = this_d;
    end
    % smaller than smallest
    if(this_d < sd)
        s_ctrs = new_ctrs;s_iter_ctrs = iter_ctrs;
        sd = this_d;s_idx = idx;
    end
    % update
    ctrs = new_ctrs;
    % update the distance of this iteration
    this_d = 0;
end
% plot
kmeans_plot(X, l_idx, l_ctrs, l_iter_ctrs);
kmeans_plot(X, s_idx, s_ctrs, s_iter_ctrs);