function [mu sigma] = mle_normal(x)
% [mu sigma] = mle_normal(x)
%
%   Computes maximum likelihood estimate of mean and sigma of a normal
%   distribution.
%
%   Parameters:
%       x - input features <1 x n>
%
%   Returns:
%       mu - mean
%       sigma - standard deviation
mu = sum(x)/size(x,2);
sigma = sqrt((1/size(x,2))*sum((x-mu).^2));


