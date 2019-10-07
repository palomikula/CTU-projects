function [ TstErr ] = compute_TstErr( itrn, itst, X, y, C )
%COMPUTE_TSTERR Computes mean risk computed over crossvalidation folds (train svm on train set and evaluate on test set)
%
% Input:
% itrn      [1 x f (cell)] cell with f folds for crossvalidation (indices to X of training data)
% itst      [1 x f (cell)] cell with f folds for crossvalidation (indices to X of test data)
% X         [n x m (double)] matrix containing feature points in columns
% y         [1 x m (double)] vector with labels (-1, 1) for feature points in X
% C         [1 x 1 (double)] number with regularization constant C
% 
% Output:
% TstErr    [1 x 1 (double)] mean test error 

errors = zeros(1,10);
for i = 1:10
   trn = X(:,itrn{i});
   ytrn = y(itrn{i});   
   [W, b, sv_idx] = my_svm(trn, ytrn, C);
   model.W = W; model.b = b; model.fun = 'linclass';
   tst = X(:,itst{i});
   ytst = y(itst{i});
    
   classif = classif_lin_svm(tst,model);
    
   errors(i) = size(find(classif~=ytst),2);
   
end
TstErr = mean(errors);

end

