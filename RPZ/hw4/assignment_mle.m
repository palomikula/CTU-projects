% Assigment: MAXIMUM LIKELIHOOD ESTIMATION

% initialize stpr toolbox 
%run('../../../../solutions/3rdparty/stprtool/stprpath.m')

%% Part 1 of the task

stepCardinality = 10;
maxCardinality = 500;
cardinalities = 10:stepCardinality:maxCardinality;
n = size(cardinalities,2);

var_mu_rec = zeros(1,n);
var_sigma_rec = zeros(1,n);

for i = 1:n    
    % Compute the variance of the estimations for a fix cardinality
    [var_mu_rec(i) var_sigma_rec(i)] = mle_variance(cardinalities(i));    
end

figure, plot(cardinalities',[var_mu_rec' var_sigma_rec'],'lineWidth',3), grid on;
legend('Mean','Standard deviation');
xlabel 'Cardinality of traning set'
ylabel 'Variance'

%% Part 2 of the task
%% Load training and test data
load data_33rpz_cv04.mat

%% Select the training set
trnSet = trn_2000;

%% Computing features vectors (trainning)
x = compute_measurement_lr_cont(trnSet.images);

%% Estimate prior probabilities
prior_A = estimate_prior(1,trnSet.labels);
prior_C = estimate_prior(2,trnSet.labels);

%% Splitting the trainning data into into classes
x_A = x(trnSet.labels == 1);
x_C = x(trnSet.labels == 2);

%% Computing Gaussian models of Maximal Likelihood
[DA.Mean DA.Sigma] = mle_normal(x_A);
[DC.Mean DC.Sigma] = mle_normal(x_C);

DA.Prior = prior_A;
DC.Prior = prior_C;

DA2000 = DA;
DC2000 = DC;

%% Plotting L VS sigma
sigmas = 200:1000;
[L maximizerSigma maxL] = loglikelihood_sigma(x_A,DA,sigmas);

% Plotting the likelihood as a function of Sigma
figure,plot(sigmas,L),grid on, title 'Likelihood varing sigma class A';
xlabel('\sigma');
ylabel('L(\sigma)');
hold on;
plot(maximizerSigma, maxL,'r+','markersize',15);
line([DA.Sigma DA.Sigma],[min(L) maxL],'Color','g');
hold off;

%% Ploting the aproximated density functions
limit = 4000;
numBins = 20;
dom = -limit : limit;
figure;
% Compute histograms
[hist_A binCenters_A] = hist(x_A,numBins);
[hist_C binCenters_C] = hist(x_C,numBins);
% Normalize histograms
hist_A = hist_A/(sum(hist_A)*(binCenters_A(2)-binCenters_A(1)));
hist_C = hist_C/(sum(hist_C)*(binCenters_C(2)-binCenters_C(1)));

% Plot histograms
figure;
hold on;
bar(binCenters_A, hist_A,'y');
plot(dom',[normpdf(dom,DA20.Mean,DA20.Sigma)'],'b','linewidth',1);
plot(dom',[normpdf(dom,DA200.Mean,DA200.Sigma)'],'g','linewidth',1);
plot(dom',[normpdf(dom,DA2000.Mean,DA2000.Sigma)'],'r','linewidth',1);
grid on, title 'Densities functions class A', legend 'hist trn2000' 'trn20' 'trn200' 'trn2000' 
hold off;

figure;
hold on;
bar(binCenters_C, hist_C,'y');
plot(dom',[normpdf(dom,DC20.Mean,DC20.Sigma)'],'b','linewidth',1);
plot(dom',[normpdf(dom,DC200.Mean,DC200.Sigma)'],'g','linewidth',1);
plot(dom',[normpdf(dom,DC2000.Mean,DC2000.Sigma)'],'r','linewidth',1);
grid on, title 'Densities functions class C', legend 'hist trn2000' 'trn20' 'trn200' 'trn2000'
hold off;

% Overlap the above plot with estimated distributions

...

%% Estimating the optimal bayesian strategy

...

% Computing features vectors (test data)
xtst = compute_measurement_lr_cont(tst.images);

% Classify images
q_x = find_strategy_2normal(DA2000,DC2000);
estimated = classify_2normal(tst.images,q_x);

% Classification error
ClassError = sum(abs(estimated - tst.labels))/size(estimated,2);

% Displaying images

show_classification(tst.images, estimated, 'AC')

