clear; 
clc;

%% Define the unknown function to interpolate
% f = @(x) exp(x).*sin(x);
f = @(x) 1./(1+25*x.^2);

%% Setup different numbers of interpolation nodes/data sites.
Nd = [2,4,8,16,32,64,128,256];

%% Setup the points at which we want to evaluate the interpolant
xe = linspace(-1,1,10000)'; %generate evenly-spaced points
ye = f(xe); %evaluate function

%% Preallocate space for numerical solution
ye_num = zeros(length(ye),length(Nd));

A = (1:8);
%% Loop goes over different numbers of interpolation nodes
for it=1:length(Nd)
    tic;
    x = rand(1,Nd(it));
    for itt = 1:Nd(it)
        x(itt) = cos((itt - 1) * pi/(Nd(it) - 1));
    end
    x = x';
    y = f(x);
    
    %% Form Vandermonde matrix "intelligently"
    V = zeros(length(x),length(x));
    for vit = 1:Nd(it)
        V(:,vit) = x.^(vit-1);
    end
    
    %% Compute coefficients by solving linear system
    a = V\y;
    
    %% Evaluate polynomial interpolant by building eval matrix and
    %% multiplying with coefficients.
    Ve = zeros(length(xe),length(x));
    for vit = 1:Nd(it)
        Ve(:,vit) = xe.^(vit-1);
    end
    ye_num(:,it) = Ve*a; %Do the matrix-vector product to predict values
    A(it) = toc;
end

%% Now plot relative errors
figure
e2 = zeros(length(Nd),1);
for it=1:length(Nd)
    e2(it,1) = norm(ye_num(:,it)-ye)./norm(ye);
end
semilogy(A,e2);