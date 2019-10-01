load('TDT2_data', 'fea', 'gnd');

% YOUR CODE HERE

nClass = length(unique(gnd));
kac = 0;
sac = 0;
nTotal = 50;
kmihat = 0;
smihat = 0;


% may take quite a long time
for i = 1:50
    i
    label = litekmeans(fea,nClass,'Replicates',10);
    label = bestMap(gnd,label);
    kac = kac + length(find(gnd == label))/length(gnd);
    kmihat = kmihat + MutualInfo(gnd,label);
    options = [];
    options.NeighborMode = 'KNN';
    options.WeightMode = 'Binary';
    options.k = 8;
    W = constructW(fea,options);
    label = spectral(W, 2);
    label = bestMap(gnd,label);
    sac = sac + length(find(gnd == label)) / length(gnd);
    smihat = smihat + MutualInfo(gnd,label);
end

fprintf("Kmeans:\n");
fprintf("Accuracy=%s, NMI=%s\n",num2str(kac/nTotal),num2str(kmihat/nTotal));
fprintf("Spetral clustering: \n");
fprintf("Accuracy=%s, NMI=%s\n",num2str(sac/nTotal),num2str(smihat/nTotal));
