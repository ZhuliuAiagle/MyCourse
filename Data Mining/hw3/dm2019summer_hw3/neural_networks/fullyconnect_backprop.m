function [weight_grad, bias_grad, out_sensitivity] = fullyconnect_backprop(in_sensitivity,  in, weight)
%The backpropagation process of fullyconnect
%   input parameter:
%       in_sensitivity  : the sensitivity from the upper layer, shape: 
%                       : [number of images, number of outputs in feedforward]
%       in              : the input in feedforward process, shape: 
%                       : [number of images, number of inputs in feedforward]
%       weight          : the weight matrix of this layer, shape: 
%                       : [number of inputs in feedforward, number of outputs in feedforward]
%
%   output parameter:
%       weight_grad     : the gradient of the weights, shape: 
%                       : [number of inputs in feedforward, number of outputs in feedforward]
%       out_sensitivity : the sensitivity to the lower layer, shape: 
%                       : [number of images, number of inputs in feedforward]
%
% Note : remember to divide by number of images in the calculation of gradients.

% TODO
gr = 0.01;
weight_grad = in' * in_sensitivity  .* gr;
bias_grad = ( (ones(1, size(in, 1)) * in_sensitivity)  .* gr)';
out_sensitivity = in_sensitivity * weight';
end

