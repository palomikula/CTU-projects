function centers = k_meanspp(x, k)
% KMEANSPP - perform k-means++ initialization for k-means clustering.
%
% Input:
%   X - [DIMS x N] N input vectors of dimenionality DIMS
%   K - [int] number of k-means centers
%
% Output:
%   CENTERS - [DIMS x K] K proposed centers for k-means initialization

N = size(x, 2);
weights = ones(1,N)./N;
ind = random_sample(weights);
centers(:,1) = x(:, ind);
dists = zeros(1,N);

while size(centers,2) < k
    for i = 1:N
        d = sqrt(sum((x(:,i) - centers).^2,1));  
        dists(i) = min(d);  
    end 

    dist = (dists.^2) ./ sum(dists.^2);
    idx = random_sample(dist);
    centers(:,end+1) = x(:,idx);
end
