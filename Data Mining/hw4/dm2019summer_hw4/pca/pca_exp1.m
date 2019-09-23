load('ORL_data', 'fea_Train', 'gnd_Train', 'fea_Test', 'gnd_Test');

% YOUR CODE HERE

% 1. Feature preprocessing

% 2. Run PCA
[eigvector,eigvalue] = pca(fea_Train);
% 3. Visualize eigenface
show_face(eigvector');
% 4. Project data on to low dimensional space
% ======
dimension = 128; % can be modified
% ======
trans = eigvector(:, 1:dimension);
lowDim_Train = fea_Train * trans;
lowDim_Test = fea_Test * trans;
% 5. Run KNN in low dimensional space
gnd = knn(lowDim_Test', lowDim_Train', gnd_Train', 1);
accuracy = sum(gnd'== gnd_Test) / length(gnd_Test);
fprintf("Test Error(dim = %d): %f\n",dimension,1 - accuracy);
% 6. Recover face images form low dimensional space, visualize them
fea = lowDim_Test * trans';
figure; show_face(fea);title(dimension);