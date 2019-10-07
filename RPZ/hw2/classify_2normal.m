function label = classify_2normal(imgs, q)
% label = classify_2normal(imgs, q)
%
%   Classify images using continuous measurement and strategy q.
%
%   Parameters:
%       images - test set images, <h x w x n>
%       q - strategy
%               q.t1 q.t2 - two descision thresholds 
%               q.decision - 3 decisions for intervals (-inf, t1>, (t1, t2>, (t2, inf)
%                            shape <1 x 3>
%
%   Return:
%       label - image labels, <1 x n>


img_values = compute_measurement_lr_cont(imgs);
label = q.decision(uint8(q.t1>img_values) + 2*uint8(q.t1<img_values & img_values<q.t2) + 3*uint8(img_values>q.t2));
