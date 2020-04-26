clear;
clc;

% Set up the program
N = 60;
x = 1:N;
time = zeros(N,1);

% Random Matrix
A = rand(N,N);

% Doing the fast calculations
for nrhs = 1: N

% Right Hand Side Matrix
B = rand(N,nrhs);

% Solution Matrix
X = zeros(N, nrhs);

tic;
[L, U, P ] = lup(A);

for j = 1: nrhs
    % [L, U, P ] = lup(A);
    X(:,j)= bsub(U,fsub(L,P* B(:,j)));
end
time(nrhs) = toc;
end

% Ploting the fast times
loglog(x,time);
xlim([0 N])
hold;

% Doing the slow calculations
for nrhs = 1: N

% Right Hand Side Matrix
B = rand(N,nrhs);

% Solution Matrix
X = zeros(N, nrhs);

tic;
for j = 1: nrhs
    [L, U, P] = lup(A);
    X(:,j)= bsub(U,fsub(L,P* B(:,j)));
end
time(nrhs) = toc;
end

% plotting the slow times
loglog(x,time);