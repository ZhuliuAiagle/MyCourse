function [out_sensitivity] = relu_backprop(in_sensitivity, in)
%The backpropagation process of relu
%   input paramter:
%       in_sensitivity  : the sensitivity from the upper layer, shape: 
%                       : [number of images, number of outputs in feedforward]
%       in              : the input in feedforward process, shape: same as in_sensitivity
%   
%   output paramter:
%       out_sensitivity : the sensitivity to the lower layer, shape: same as in_sensitivity

% TODO
% reLU
relu  =  max(in, zeros(size(in)));
% Derivation, 0 or 1, according to ReLU function
relu_der = relu ./ in;
% update sensitivity
out_sensitivity = relu_der .* in_sensitivity; 
end

