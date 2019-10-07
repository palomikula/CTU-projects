function R = bayes_risk_2normal(distribution1, prior1, distribution2, prior2, q)
% R = bayes_risk_2normal(distribution1, distribution2, q)
%
%   Compute bayesian risk of a strategy q for 2 normal distributions and zero-one loss function.
%
%   Parameters:
%       distribution1 - parameters of the normal dist. distribution1.Mean, distribution1.Sigma
%       distribution2 - the same as distribution1
%       q - strategy
%               q.t1 q.t2 - two descision thresholds 
%               q.decision - 3 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
%                            shape <1 x 3>
%
%   Return:
%       R - bayesian risk, scalar


distributions = [distribution1 distribution2];
priors = [prior1 prior2];
partial_risks = [normcdf([-Inf q.t1],distributions(q.decision(1)).Mean,distributions(q.decision(1)).Sigma); ...
                normcdf([q.t1 q.t2],distributions(q.decision(2)).Mean,distributions(q.decision(2)).Sigma); ...
                normcdf([q.t2 Inf],distributions(q.decision(3)).Mean,distributions(q.decision(3)).Sigma)];
R = 1 - priors(q.decision(:))*(partial_risks(:,2) - partial_risks(:,1));
    
