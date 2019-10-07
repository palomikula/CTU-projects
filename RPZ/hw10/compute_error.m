function errors = compute_error(strong_class, X, y)
% errors = compute_error(strong_class, X, y)
%
% Computes the error on data X for *all lengths* of the given strong
% classifier
%
%   Parameters:
%       strong_class - the structure returned by adaboost()
%
%       X [K x N] - samples, K is the number of weak classifiers and N the
%            number of data points
%
%       y [1 x N] - sample labels (-1 or 1)
%
%   Returns:
%       errors [1 x T] - error of the strong classifier for all lenghts 1:T
%            of the strong classifier
%
N = numel(strong_class.wc);
errors = zeros(1,N);
for i = 1:N
    sc.wc = strong_class.wc(1:i);
    sc.alpha = strong_class.alpha(1:i);
    classify = adaboost_classify(sc, X);
    errors(i) = sum(classify ~= y) / size(X,2);

end
