clear;
clc;

load X.dat
load Y.dat

% Part A
A = inv(transpose(X) * X)*transpose(X)*Y;

ErrorA = norm(Y - X*A,2)

s = [1,5,10,15,20,25,30];

for index = 1:7
    AS = inv(transpose(X)*X + s(index).^2*eye(15))*transpose(X)*Y;
    ErrorAS = norm(Y - X*AS,2);
end

Z1 = zeros(8,1);
Z2 = zeros(8,1);
Z3 = zeros(8,1);

% % Part B, A
% X1 = X(1:66,:);
% Y1 = Y(1:66);
% 
% A = inv(transpose(X1) * X1)*transpose(X1)*Y1;
% 
% ErrorA = norm(Y(67:100) - X(67:100,:)*A,2);
% 
% Z1(1) = ErrorA;
% 
% for index = 1:7
%     AS = inv(transpose(X1)*X1 + s(index).^2*eye(15))*transpose(X1)*Y1;
%     ErrorAS = norm(Y(67:100) - X(67:100,:)*AS,2);
%     Z1(index + 1) = ErrorAS;
% end
% 
% % Part B, B
% X1 = X(34:100,:);
% Y1 = Y(34:100);
% 
% A = inv(transpose(X1) * X1)*transpose(X1)*Y1;
% 
% ErrorA = norm(Y(1:33) - X(1:33,:)*A,2);
% 
% Z2(1) = ErrorA;
% 
% for index = 1:7
%     AS = inv(transpose(X1)*X1 + s(index).^2*eye(15))*transpose(X1)*Y1;
%     ErrorAS = norm(Y(1:33) - X(1:33,:)*AS,2);
%     Z2(index + 1) = ErrorAS;
% end
% 
% % Part B, C
% X1 = [X(1:33,:); X(67:100,:)];
% Y1 = [Y(1:33); Y(67:100)];
% 
% A = inv(transpose(X1) * X1)*transpose(X1)*Y1;
% 
% ErrorA = norm(Y(34:66) - X(34:66,:)*A,2);
% 
% Z3(1) = ErrorA;
% 
% for index = 1:7
%     AS = inv(transpose(X1)*X1 + s(index).^2*eye(15))*transpose(X1)*Y1;
%     ErrorAS = norm(Y(34:66) - X(34:66,:)*AS,2);
%     Z3(index + 1) = ErrorAS;
% end
% 
% %Part C
% for i = 1:8
%     (Z1(i) + Z2(i) + Z3(i)) / 3
% end