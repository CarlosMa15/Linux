%% Clearing everything for a fresh start
clear; 
clc;

%% Define the unknown function to interpolate
f = @(x) sin(x) + 0.1 * rand(length(x),1);

%% Setup the points at which we want to evaluate the interpolant
xe = linspace(-1,1,10000)'; %generate evenly-spaced points
ye = f(xe); %evaluate function

%% Numbers of data points.
%% Modify Code Here the N
Nd = 3; % 3, 10, 40

%% Creating variables
%% Modify Code Here the l
l = 2; % 1, 2, Nd - 1
N = l + 1;

%% Start plotting stuff
figure
plot(xe,ye,'black');
%pause %waits for input

%% Generate evenly-spaced points in [-1,1].
x = linspace(-1,1,Nd)';

%% Get the values of y
y = f(x);

%% Getting the vandermonde matrix
V = zeros(Nd,N);
for vit = 1:N
    V(:,vit) = x.^(vit-1);
end

%% Aquiring Q1 and R1 which have independent columns/rows for us to use
[Q,R] = qr(V);
Q1 = Q(:,1:N);
R1 = R(1:N,1:N);

%% Compute coefficients by QR decomposition.
a = R1 \ (Q1' * y);

%% Evaluate polynomial interpolant by building eval matrix and
%% multiplying with coefficients.
Ve = zeros(length(xe),N);
for vit = 1:N
    Ve(:,vit) = xe.^(vit-1);
end
ye_num = Ve*a; %Do the matrix-vector product to predict values

%% Plot stuff
hold on;
plot(xe,ye_num,'-');
drawnow