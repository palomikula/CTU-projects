function classify = adaboost_classify(strong_class, X)
% classify = adaboost_classify(strong_class, X)
%
% Applies the strong classifier to the data X and returns the
% classification labels
%
%   Parameters:
%       strong_class - the structure returned by adaboost()
%
%       X [K x N] - training samples, K is the number of weak classifiers
%            and N the number of data points
%
%   Returns:
%       classify [1 x N] - the labels of the input data X as predicted by
%            the strong classifier strong_class
%

s = zeros(1,size(X,2));
wc = strong_class.wc;
for i = 1:numel(strong_class.wc)
    s = s + (strong_class.alpha(i)*(sign(wc(i).parity * (X(wc(i).idx,:) - wc(i).theta ))));
end
classify = sign(s);