function worst_risks = worst_risk_cont(D1, D2, D1_priors)
% worst_risks = worst_risk_cont(D1, D2, D1_priors)
%
%   Compute worst possible risks of a bayesian strategies.
%
%   Parameters:
%       D1, D2 - normal distributions
%       D1_priors - <1 x n> vector of D1 priors to be used
%
%   Returns:
%       worst_risks - <1 x n> worst risk of bayesian strategies
%                     for D1, D2 with different priors D1_priors


%   Hint: for all D1_priors calculate bayesian strategy and 
%   corresponding maximal risk.

worst_risks = zeros(size(D1_priors));

for i = 1:size(D1_priors,2)
    q = find_strategy_2normal(D1, D1_priors(i), D2, 1-D1_priors(i));
    worst_risks(i) = max(risk_fix_q_cont(D1, D2, D1_priors, q));
end
