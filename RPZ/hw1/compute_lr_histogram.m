function lr_histogram = compute_lr_histogram(letter_char, Alphabet, images, labels, num_bins)
% lr_histogram = compute_lr_histogram(letter_char, Alphabet, images, labels, num_bins)
%
%   Calculates feature histogram.
%
%   Parameters:
%       letter_char is a character representing the letter whose feature 
%                   histogram we want to compute, e.g. 'C'
%       Alphabet - string of characters
%       images - images in 3d matrix of shape <h x w x n>
%       labels - labels of images, indices to Alphabet list, <1 x n>
%       num_bins - number of histogram bins
%   
%   Return: 
%       lr_histogram - counts of values in the corresponding bins, vector <1 x num_bins>
%
%   class support: integer classes
    letter_indices = labels == find(Alphabet==letter_char);
    letter_images = double(images(:,:,letter_indices));
    l_values = sum(sum(letter_images(:,1:5,:),1),2);
    r_values = sum(sum(letter_images(:,6:10,:),1),2);
    features = l_values - r_values;
    lr_histogram = hist(reshape(features,1,100),num_bins);
    
end
