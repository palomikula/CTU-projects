function [ classif ] = classif_kernel_svm( X, model )
%CLASSIF_KERNEL_SVM performs classification on X by trained SVM classifier stored in model
% 
% Input:
% X         [n x m (double)] matrix containing feature points in columns
% model     [1 x 1 (struct)] structure with fields (.b, .fun, .sv, .y, .alpha, .options see my_kernel_svm.m for details)
% 
% Output:
% classif   [1 x m (double)] vector with labels for X obtained by classification 
% 

    classif = ones(1,size(X, 2));
    f = (model.alpha'.*model.y)*getKernel(model.sv,X,model.options) + model.b;
    classif(f<0) = -1;

end

