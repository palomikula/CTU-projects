function R = bayes_risk_discrete(distribution1, distribution2, W, q)
% R = bayes_risk_discrete(distribution1, distribution2, W, q)
%
%   Compute bayesian risk for a discrete strategy q
%
%   Parameters:
%           distribution1.Prob      pXk(x|A) given as a <1 × n> vector
%           distribution1.Prior 	prior probability pK(A)
%           W                       cost function matrix
%                                   dims: <states x decisions>
%                                   (nr. of states and decisions is fixed to 2)
%           q                       strategy - <1 × n> vector, values 1 or 2
%
%   Return:
%           R - bayesian risk, <1 x 1>

post = [distribution1.Prob.*distribution1.Prior ; distribution2.Prob.*distribution2.Prior];
losses = W([1 2],q);
R = sum(sum(post.*losses));

