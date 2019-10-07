function [strong_class, wc_error, upper_bound] = adaboost(X, y, num_steps)
% [strong_class, wc_error, upper_bound] = adaboost(X, y, num_steps)
%
% Trains an AdaBoost classifier
%
%   Parameters:
%       X [K x N] - training samples, KxN matrix, where K is the number of 
%            weak classifiers and N the number of data points
%
%       y [1 x N] - training samples labels (1xN vector, contains -1 or 1)
%
%       num_steps - number of iterations
%
%   Returns:
%       strong_class - a structure containing the found strong classifier
%           .wc [1 x num_steps] - an array containing the weak classifiers
%                  (their structure is described in findbestweak())
%           .alpha [1 x num_steps] - weak classifier coefficients
%
%       wc_error [1 x num_steps] - error of the best weak classifier in
%             each iteration
%
%       upper_bound [ 1 x num_steps] - upper bound on the training error
%

%% initialisation
N = length(y);

% prepare empty arrays for results
strong_class.wc = [];
strong_class.alpha = zeros(1, num_steps);

%% AdaBoost
D = 0.5*ones(1,N) / length(y(y==1));
D(y==-1) = 0.5/length(y(y==-1));
wc_error = zeros(1,num_steps);
upper_bound = ones(1,num_steps);
alphas = ones(N,1);

for t = 1:num_steps
    disp(['Step ' num2str(t)]);
    [wc, error] = findbestweak(X,y,D);
    if (error >= 0.5) 
        break;
    end
    alphas(t) = (0.5)*log((1-error)/error); 
    
    D = D.*exp(-alphas(t)*y.*sign(wc.parity.*(X(wc.idx,:)-wc.theta)));    
    upper_bound(t) = sum(D);
    D = D/upper_bound(t);
    strong_class.wc(t).idx = wc.idx;
    strong_class.wc(t).parity = wc.parity;
    strong_class.wc(t).theta = wc.theta;
    strong_class.alpha(t) = alphas(t);
    wc_error(t) = error;
    
end
upper_bound = cumprod(upper_bound);

