% (2 points) A (function) file called fsub.m that contains a single function
% called fsub. This function does forward substitution. This function takes
% in two arguments: a unit lower-triangular matrix L, and a single column
% vector d. It outputs a column vector y that is the solution to Ly = d.
% Given: L, an N × N unit lower-triangular matrix.
% Given: d = P*b, a right hand side vector.
% Generate: y, the solution to L*y = d.
function [y] = fsub(L,d)
    N = size(L,1);
    y = d;
    for i = 2:N
        y(i) = d(i) - L(i,1:i-1)*y(1:i-1);
    end
end