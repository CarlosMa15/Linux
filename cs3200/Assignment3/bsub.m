% (2 points) A (function) file called bsub.m that contains a
% single function called bsub. This function does backward substitution.
% This function takes in two arguments: an upper-triangular matrix U, and
% a single column vector y. It outputs a column vector x that is the
% solution to Ux = y.
% Given:U, an N × N upper-triangular matrix.
% Given:y, a right hand side vector.
% Generate:x, the solution to U*x = y.
function [x] = bsub(U,y)
    [~,N] = size(U);
    
    x = zeros(N,1);
    
    x(N) =y(N)/U(N, N);
    
    for i = N-1: -1:1
        x(i) = (y(i)-U(i, i+ 1 :N)*x(i+ 1 :N))/U(i, i);
    end
    
    return
end