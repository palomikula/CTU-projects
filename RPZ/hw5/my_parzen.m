function p = my_parzen(x, x_trn, h)
% p = my_parzen(x, x_trn, h)
%
%   Parzen window density estimation with normal kernel N(0, h^2).
%
%   Parameters:
%       x - vector of data points where the probability density functions
%           should be evaluated
%       x_trn - training data
%       h - kernel bandwidth
%
%   Returns:
%       p - estimated p(x|k) evaluated at values given by x

for i = 1:size(x,2)
    x_est = x(i) - x_trn;   
    p(i) = mean(normpdf(x_est, 0, h));
end
