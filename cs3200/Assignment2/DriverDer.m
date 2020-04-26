clear;
clc;

a = 10;
b = 100;
N = 1000;
syms x;

% function change both to be the same
f = @(x) x.^2;
ff = x.^2;

%Sample data
x = linspace(a,b,N);
y = f(x);

% Calcuate derivatives from your approx
dy = FirstDer(x,y);
% real_dy = df(x);
real_dy = diff(ff);
xx = linspace(a,b,N)';

% semilogy(xx,dy);

yy = eval(subs(real_dy,xx));

e2 = zeros(length(y),1);
for it=1:length(y)
   e2(it,1) = norm(dy(it) - yy(it))/norm(yy(it));
end
figure;

loglog(xx,e2);