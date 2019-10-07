function [w_new, wt, Et] = logistic_loss_gradient_descent(X, y, w_init, epsilon)
% [w, wt, Et] = logistic_loss_gradient_descent(X, y, w_init, epsilon)
%
%   Performs gradient descent optimization of the logistic loss function.
%
%   Parameters:
%       X - d-dimensional observations of size [d, number_of_observations]
%       y - labels of the observations of size [1, number_of_observations]
%       w_init - initial weights of size [d, 1]
%       epsilon - parameter of termination condition: norm(w_new - w_prev) <= epsilon
%
%   Return:
%       w - resulting weights
%       wt - progress of weights (of size [d, number_of_accepted_candidates])
%       Et - progress of logistic loss (of size [1, number_of_accepted_candidates])

w_new = w_init;
w_prev = 100000000*ones(size(w_new));
E_prev = logistic_loss(X,y,w_new);
grad = logistic_loss_gradient(X,y,w_new);
Et = E_prev;
step = 1;
wt = w_new;

while norm(w_prev - w_new) > epsilon
   E_new = logistic_loss(X,y, w_new - step * grad);

   if E_new < E_prev

      w_prev = w_new;
      E_prev = E_new;
      w_new = w_new - step * grad;
      wt = [wt w_new];
      Et = [Et E_prev];
      grad = logistic_loss_gradient(X,y,w_prev - step * grad);
      step = step * 2;
      
   else
       step = step / 2;
   end
end