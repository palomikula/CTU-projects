function K = classif_quadrat_perc(tst, model)

% Classifies test samples using the quadratic discriminative function 
%
%   Parameters:
%       tst - samples for classification in original space (2xN matrix)
%       model - structure with the trained perceptron classifier
%       (parameters of the descriminative function)
%           model.w - weights vector (5x1 vector)
%           model.b - bias term (1 double)
%
%   Returns:
%       K - classification result (1xN vector, contains either 1 or 2)



K = ones(1, size(tst,2));
K(model.w' * lift_dimension(tst) + model.b < 0) = 2;

end
