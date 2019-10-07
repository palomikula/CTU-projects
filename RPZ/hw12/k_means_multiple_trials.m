function [c, means, sq_dists] = k_means_multiple_trials(x, k, n_trials, max_iter, show)
% [c, means, sq_dists] = k_means_multiple_trials(x, k, n_trials, max_iter, show)
%
% Performs several trials of the k-means clustering algorithm in order to
% avoid local minima. Result of the trial with the lowest "within-cluster
% sum of squares" is selected as the best one and returned.
%
% Input:
%   x         .. Feature vectors, of size [dim,number_of_vectors], where dim
%                is arbitrary feature vector dimension.
%
%   k         .. Required number of clusters (single number).
%
%   n_trials  .. Number of trials.
%
%   max_iter  .. Stopping criterion: max. number of iterations (single number)
%                for each of the trials. Set it to Inf if you wish to deactivate
%                this criterion. 
%
%   show      .. Boolean switch to turn on/off visualization of partial results.
%
% Output (= information about the best clustering from all the trials):
%   c         .. Cluster index for each feature vector, of size
%                [1, number_of_vectors], containing only values from 1 to k,
%                i.e. c(i) is the index of a cluster which the vector x(:,i)
%                belongs to.
%
%   means     .. Cluster centers, of size [dim,k], i.e. means(:,i) is the
%                center of the i-th cluster.
%
%   sq_dists   .. Squared distance to the nearest mean for each feature vector,
%                 of size [1, number_of_vectors].

if nargin < 5
    show = false;
end

c = Inf;
means = Inf;
sq_dists = Inf;
 
% Multiple trial of the k-means clustering algorithm
for i_trial = 1:n_trials
   [tmp_c, tmp_means, tmp_sq_dists] = k_means(x, k, max_iter);
   if sum(tmp_sq_dists) < sum(sq_dists)
       c = tmp_c;
       means = tmp_means;
       sq_dists = tmp_sq_dists;
   end
   
% Plotting partial results
    if show
        fprintf('Iteration: %d\n', i_iter);
        show_clusters(x, c, means);
        disp('Press any key or mouse button in the figure...')    
        waitforbuttonpress;        
    end
     
end

