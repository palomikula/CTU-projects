function im_q = quantize_colors(im, k)
% im_q = quantize_colors(im, k)
%
% Image color quantization using the k-means clustering. The pixel colors
% are at first clustered into k clusters. Color of each pixel is then set
% to the mean color of the cluster to which it belongs to.
%
% Input:
%   im        .. Image whose pixel colors are to be quantized.
%
%   k         .. Required number of quantized colors.
%
% Output:
%   im_q      .. Image with quantized colors.


% Convert the image from RGB to L*a*b* color space
cform = makecform('srgb2lab');
im = applycform(im, cform);
indices = randsample(size(im,1) * size(im,2), 1000);
 
pixels = double(reshape(im,size(im,1) * size(im,2),3)');

[~,means,~] = k_means(pixels(:,indices),k,Inf);
for i = 1:size(pixels,2)
    for k = 1:size(means,2)
        pixel = double(pixels(:,i));
        y(k) = norm(pixel - means(:,k));
    end
    [~,idx] = min(y);
    pixels(:,i) = means(:,idx);
end

im_q = uint8(reshape(pixels', size(im)));
 
 
% Convert the image from L*a*b* back to RGB
cform = makecform('lab2srgb');
im_q = applycform(im_q, cform);
