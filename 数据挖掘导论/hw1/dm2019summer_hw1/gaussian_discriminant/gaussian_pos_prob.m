function p = gaussian_pos_prob(X, Mu, Sigma, Phi)
%GAUSSIAN_POS_PROB Posterior probability of GDA.
%   p = GAUSSIAN_POS_PROB(X, Mu, Sigma) compute the posterior probability
%   of given N data points X using Gaussian Discriminant Analysis where the
%   K gaussian distributions are specified by Mu, Sigma and Phi.
%
%   Inputs:
%       'X'     - M-by-N matrix, N data points of dimension M.
%       'Mu'    - M-by-K matrix, mean of K Gaussian distributions.
%       'Sigma' - M-by-M-by-K matrix (yes, a 3D matrix), variance matrix of
%                   K Gaussian distributions.
%       'Phi'   - 1-by-K matrix, prior of K Gaussian distributions.
%
%   Outputs:
%       'p'     - N-by-K matrix, posterior probability of N data points
%                   with in K Gaussian distributions.

N = size(X, 2);
K = length(Phi);
P = zeros(N, K);
M = size(X, 1);

% Your code HERE

for i = 1:N
    for j = 1:K
        x = X(:,i);
        mu = Mu(:,j);
        sigma = Sigma(:,:,j);
       P(i, j) = Phi(j) * exp((x-mu)' * sigma^-1 * (x-mu)/-2);
       P(i, j) = P(i,j)./ ((2 * pi)^(M/2) * sqrt(det(sigma)));
    end
end
P = P ./ sum(P,2);
P
p = P;