clear;
clc;

N = 30;
nrhs = 16;

% Random Matrix
A = rand(N,N);

% Right Hand Side Matrix
B = rand(N,nrhs);

% Solution Matrix
X = zeros(N, nrhs);

% Using a loop, you will fill this solution matrix column by column.
% Forinstance,X(:,j)=bsub(U,fsub(L,P∗ B(:,j))) solves for one column of
% unknowns using one of the right hand sides from the matrix B. Outside
% this loop (before the loop), you will call [L, U, P ] = lup(A) exactly once.
[L, U, P ] = lup(A);
for j = 1: nrhs
    X(:,j)= bsub(U,fsub(L,P* B(:,j)));
end

% Display Matrix
% You will then also use Matlab’s built-in solvers (the backslash operator)
% to compute X2, another solution to the linear system. This can be done
% via X2 = A\B (no loops). You must now print the relative error of your
% solution X against the Matlab solution X2.
X
X2 = A\B

% Display Error
% This error should be O(10^14) − O(10^16).
TheRequestError = norm(X - X2)./norm(X2)