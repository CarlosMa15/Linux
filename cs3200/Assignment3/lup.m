% (5 points) A (function) file that contains a single function called plu.
% The file must also be called plu.m. The function plu takes a single matrix
% A as an input, and outputs L, U, and P in that order. L is the unit
% lower-triangular matrix, U is the upper triangular matrix, and P is the
% pivoting matrix so that PA = LU.
% Given:A, an N × N matrix.
% Generate:L, a unit lower triangular matrix,U, an upper triangular matrix,
% and P, a pivoting matrix, so thatP A = LU.
function [L,U,P] = lup(A)
    N = size(A,1);
    U = A;
    L = eye(N, N);
    P = eye(N, N);

    for i = 1 : N - 1
        
        % Find m ≥ ithat results in maximum|U(m, i)|(index of 
        % largest columnelement in absolute value).
        
        [~,m] = max(abs(A(i:N,i)));
        m = m + i -1;
        
        % Swap
        temp = U(i,i:N);
        U(i,i:N) = U(m,i:N);
        U(m,i:N) = temp;
        
        temp = L(i,1:i-1);
        L(i,1:i-1) = L(m,1:i-1);
        L(m,1:i-1) = temp;
        
        temp = P(m,:);
        P(m,:) = P(i,:);
        P(i,:) = temp;
        
        for j = i + 1 : N
            L(j,i) = U(j,i) / U(i,i);
            U(j,i:N) = U(j,i:N) - L(j,i)*U(i,i:N);
        end
    end
end