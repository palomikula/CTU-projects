function idx = random_sample(weights)
% RANDOM_SAMPLE picks randomly a sample based on the sample weights.
%
% Suppose weights / sum(weights) is a discrete probability distribution. 
% RANDOM_SAMPLE picks a sample from the distribution and returns its index.
%
% Input:
%   weights - array of sample weights
%
% Output:
%   idx - index of chosen sample

% use rand() for random number generation in open interval (0, 1)

rnd = rand()*sum(weights);
for i = 1:length(weights)
    if rnd<=sum(weights(1:i))
        idx = i;
        break;
    end
end

