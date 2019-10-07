%% Asignment: Support Vector Machines 2

clc;
close all;
clearvars;

%% Init STPR Toolbox

% Change this:
%run('../statistical-pattern-recognition-toolbox-master/stprpath.m');

%% %% Tasks, part 1 - Kernel SVM

%% %% Load data

load('flower.mat');
% load('tmp/data_33rpz_svm_toy.mat');

y(y == 2) = -1;

C = 10000;
options = c2s({'verb', 1, 'tmax', inf, 'kernel', 'polynomial', 'd', 4});

model = my_kernel_svm(X, y, C, options);

% visualization 
y(y == -1) = 2;
figure(1);
ppatterns(X, y);
pboundary(model);
title('Polynomial kernel, d = 4');

saveas(gcf, 'flower_polynomial.png');

%% %% Tasks, part 2 - Model selection on OCR data

% Train
clc;
clear all;

load('ocr_data_2D_trn.mat');

Cs = [0.0001 0.001 0.01 0.1];
ds = [5, 10, 15];

rand('seed', 42);
[itrn, itst] = crossval(size(X, 2), 4);
TstErrs = [];

min_err = inf;

for i = 1:size(Cs, 2)
    for j = 1:size(ds, 2)
        options = c2s({'verb', 1, 'tmax', inf, 'kernel', 'polynomial', 'd', ds(j)});
        TstErrs(end+1) = compute_kernel_TstErr(itrn, itst, X, y, Cs(i), options);
        if min_err > TstErrs(end)
            min_err = TstErrs(end);
            cs_idx = i;
            ds_idx = j;
        end
    end
end

[minTstErr, idx] = min(TstErrs);

C_opt = Cs(cs_idx);
d_opt = ds(ds_idx);

% Train with optimal C and d

y(y == 2) = -1;
options = c2s({'verb', 1, 'tmax', inf, 'kernel', 'polynomial', 'd', d_opt});
model = my_kernel_svm(X, y, C_opt, options);

classif = classif_kernel_svm(X,model);
trn_error = size(find(classif~=y));

% visualization 
y(y == -1) = 2;
figure;
ppatterns(X, y);
pboundary(model);
title(['Polynomial kernel, d = ' num2str(d_opt) ' TRN data']);

saveas(gcf, 'ocr_polynomial_kernel_trn.png');

% Test

load('ocr_data_2D_tst.mat');

y(y == 2) = -1;

classif = classif_kernel_svm(X,model);
error = size(find(classif~=y));

% visualization 
y(y == -1) = 2;
figure;
ppatterns(X, y);
pboundary(model);
title(['Polynomial kernel, d = ' num2str(d_opt) ' TST data']);

saveas(gcf, 'ocr_polynomial_kernel_tst.png');

classif(classif==-1) = 2;
show_classification(tst.images, classif, 'AC');
saveas(gcf, 'ocr_polynomial_kernel_classif.png');


%% %% Tasks, part 3 - Digit classification

% Train kernel SVM

clc;
clear all; 

load('mnist_01_trn.mat');

Cs = [0.01 0.1 1 10];
sigmas = [0.5, 0.9, 1.5];

rand('seed', 42);
[itrn, itst] = crossval(size(X, 2), 5);
TstErrs = [];

min_err = inf;

for i = 1:size(Cs, 2)
    for j = 1:size(sigmas, 2)
        options = c2s({'verb', 1, 'tmax', inf, 'kernel', 'rbf', 'sigma', sigmas(j)});
        TstErrs(end+1) = compute_kernel_TstErr(itrn, itst, X, y, Cs(i), options);
        if min_err > TstErrs(end)
            min_err = TstErrs(end);
            cs_idx = i;
            sigmas_idx = j;
        end
    end;
end;

[minTstErr, idx] = min(TstErrs);

C_opt = Cs(cs_idx);
sigma_opt = sigmas(sigmas_idx);

options = c2s({'verb', 1, 'tmax', inf, 'kernel', 'rbf', 'sigma', sigma_opt});
model = my_kernel_svm(X, y, C_opt, options);

classif = classif_kernel_svm(X,model);
trn_error = size(find(classif~=y),2);

% Evaluate on test data 

load('mnist_01_tst.mat');

yclass = classif_kernel_svm(X,model);

show_mnist_classification(X, yclass);

saveas(gcf, 'mnist_tst_classif.png');

error = size(find(yclass~=y),2);
