function [w, b] = perceptron(X, y, maxIterations)
    
% Implements the perceptron algorithm
% (http://en.wikipedia.org/wiki/Perceptron)
%   
%   Parameters:
%       X - training samples (DxN matrix)
%       y - training labels (1xN vector, contains either 1 or 2)
%
%   Returns:
%       w - weights vector (Dx1 vector)
%       b - bias (offset) term (1 double)
 
    
Z = [X; ones(1,size(X,2))];

for i = 1:size(y,2)
    if y(i)==2
        Z(:,i) = -Z(:,i);
    end
end
    
v = zeros(1, size(Z,1));

while(maxIterations>0)
    
    for i = 1:size(Z,2)
        if v*Z(:,i) > 0
            if i == size(Z,2)
                w = v(1,1:end-1)';
                b = v(1,end);
                return;
            end
            continue;
        end
        
        v = v + Z(:,i)';
        break;
    end
    
    maxIterations = maxIterations - 1;
end

w = NaN;
b = NaN;

end
