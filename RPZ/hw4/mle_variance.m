function [var_mean var_sigma] = mle_variance(cardinality)
% [var_mean var_sigma] = mle_variance(cardinality)
%
%   Estimates variance of estimated parameters of a normal distribution 
%   in 100 trials.
%
%   Parameters:
%       cardinality - size of the generated dataset (e.g. 1000)
%   Returns
%       var_mean - variance of the estimated means in 100 trials
%       var_sigma - variance of the estimated standard deviation in 100 trials


    numTrials = 100;
    muRec = zeros(1,numTrials);
    sigmaRec = zeros(1,numTrials);
    
    data = randn(numTrials, cardinality);
    
    for i = 1:numTrials
        [muRec(1,i) sigmaRec(1,i)] = mle_normal(data(i,:));
    end
    
    [~,sigma_mean] = mle_normal(muRec);
    [~,sigma_sigma] = mle_normal(sigmaRec);
    var_mean = sigma_mean^2;
    var_sigma = sigma_sigma^2;
    
    
    
    
    


