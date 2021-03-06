function [q risk] = minmax_strategy_discrete(D1, D2)
% [q risk] = minmax_strategy_cont(D1, D2)
%
%   Find minmax strategy.
%
%   Parameters:
%       D1, D2 - 2 discrete distributions
%                D1.Prob - <1x21> vector of conditional probs
%                prior not needed
%
%   Returns:
%       q - strategy, <1x21> vector of 1 and 2 (see find_strategy_discrete)
%       risk - bayes risk of the minimax strategy q (in discrete
%       case, use the worse case risk of the optimal strategy - do
%       you know why?)

W = [0 1; 1 0];
p_1 = 0:0.01:1;
[risk, idx] = min(worst_risk_discrete(D1, D2, p_1));
idx=idx;
q = find_strategy_discrete(D1, p_1(idx), D2, 1-p_1(idx), W);




