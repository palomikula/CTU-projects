function maxLL = likelihood(x, D, sigma)
sz = size(x,2);
maxLL = -sz*log(sigma)- sz/2 * log(2*pi) - 1/(2*sigma^2)*sum((x - D.Mean).^2);