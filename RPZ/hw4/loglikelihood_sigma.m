function [L maximizerSigma maxL] = loglikelihood_sigma(x, D, sigmas)
% [L maximizerSigma maxL] = loglikelihood_sigma(x, D, sigmas)
%
%   Compute log ligelihoods and maximum ll of a normal
%   distribution with fixed mean and variable standard deviation.
%
%   Parameters:
%       x - input features <1 x n>
%       D - D.Mean the normal distribution mean
%       sigmas - <1 x m> vector of standard deviations
%
%   Returns:
%       L - <1 x m> vector of log likelihoods
%       maximizerSigma - sigma for the maximal log likelihood
%       maxL - maximal log likelihood
  
% no need of for loops!!!
	sz = size(x,2);
    
    L = -sz*log(sigmas)- sz/2 * log(2*pi) - 1./(2*sigmas.^2)*sum((x - D.Mean).^2)
    [maximizerSigma, maxL] = fminbnd(@(sigma) -likelihood(x, D, sigma), 0, 1000000)
    maxL = -maxL;
    

