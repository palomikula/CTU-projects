% RPZ assigment: Logistic regression


%% Init
run('../statistical-pattern-recognition-toolbox-master/stprpath.m');

%% Classification of letters A and C
%--------------------------------------------------------------------------
% Load training data and compute features
load ('data_33rpz_logreg.mat');

% Prepare the training data
[trainX, params] = compute_measurements(trn.images);
trainX = [ones(1,size(trainX, 2));trainX];

% Training - gradient descent of the logistic loss function
% Start at a fixed point:
w_init = [-7; -8];
% or start at a random point:
% w_init = 20 * (rand(size(trainX, 1), 1) - 0.5);
epsilon = 1e-2;
[w, wt, Et] = logistic_loss_gradient_descent(trainX, trn.labels, w_init, epsilon);

% Plot the progress of the gradient descent
% plotEt
figure();
plot(Et);
title('Logistic regression error');
xlabel('Iteration');
ylabel('Loss function');    



figure
plot_gradient_descent(trainX, trn.labels, @logistic_loss, w, wt, Et);

% plot aposteriori probabilities
figure();
thr = get_threshold(w);
fplot(@(x) 1/(1+exp([1 x]*w)), [-4 4], 'b');
hold on;
fplot(@(x) 1/(1+exp(-[1 x]*w)), [-4 4], 'r');
cur_ylim = get(gca, 'ylim');
plot([thr thr], [cur_ylim(1) cur_ylim(2)], 'black');


down = trainX(2,(trn.labels == 1));
up = trainX(2,(trn.labels == -1));
scatter(down', zeros(size(down')),'*' , 'b');
scatter(up', ones(size(up')),'*' , 'r');
legend('p(A|x)', 'p(C|x)');

hold off;

% Prepare the test data
tmp = compute_measurements(tst.images,params);
testX = [ones(1,size(tmp, 2));tmp];

% Classify letter test data and calculate classification error
classifiedLabels = classify_images(testX, w);

testError = sum(tst.labels ~= classifiedLabels) / size(tst.labels,2);
fprintf('Letter classification error: %.2f%%\n', testError * 100);

% Visualize classification results
show_classification(tst.images, classifiedLabels, 'AC');


%% Classification of MNIST digits
%--------------------------------------------------------------------------
% Load training data
load('mnist_01_trn');

% prepare the training data
Xtrain = X;
%y = ???;

% Training - gradient descent of the logistic loss function
w_init = rand(size(X, 1), 1);
epsilon = 1e-2;
[w, ~, Et] = logistic_loss_gradient_descent(Xtrain, y, w_init, epsilon);

% Plot the progress of the gradient descent
% plotEt
figure();
plot(Et);
title('Logistic regression error');
xlabel('Iteration');
ylabel('Loss function');
    
% Load test data
load('mnist_01_tst');
Xtest = X;

% prepare the training data

% Classify MNIST test data and calculate classification error
classifiedLabels = classify_images(Xtest, w);
testError = sum(y ~= classifiedLabels) / size(y,2);
fprintf('MNIST digit classification error: %.2f%%\n', testError * 100);

% Visualize classification results
show_mnist_classification(Xtest(1:end, :), classifiedLabels);
