function q = find_strategy_discrete(distribution1, distribution2, W)
% q = find_strategy_discrete(distribution1, distribution2, W)
%
%   Find bayesian strategy for 2 discrete distributions.
%   
%   Parameters:
%       distribution1.Prob      pXk(x|A) given as a <1 × n> vector
%       distribution1.Prior 	prior probability pK(A)
%       distribution2.Prob      ...
%       distribution2.Prior 	...
%       W - cost function matrix, <states x decisions>
%                                (nr. of states is fixed to 2)
%
%   Return: 
%       q - optimal strategy <1 x n>


post = [distribution1.Prob.*distribution1.Prior ; distribution2.Prob.*distribution2.Prior];
args = post'*W;
q = ceil((args(:,1)-args(:,2))')+1;
