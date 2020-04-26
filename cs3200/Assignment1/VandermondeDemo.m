%% Clearing everything
clear;  %% clears the left side memory stuff
clc;    %% clears the command window
%% Define the unknown function to interpolate
% f = @(x) exp(x).*sin(x);
f = @(x) 1./(1+25*x.^2);
%% Setup different numbers of interpolation nodes/data sites.
Nd = [2,4,8,16,32,64,128,256];
%% Setup the points at which we want to evaluate the interpolant
xe = linspace(-1,1,1000)'; % change here
% generates a transpose row vector xe of 1000 points linearly spaced between and including -1 and 1
ye = f(xe); %evaluate function
%% Preallocate space for numerical solution
ye_num = zeros(length(ye),length(Nd));
%% Start plotting stuff
figure
plot(xe,ye,'black');
pause %waits for input
%% Loop goes over different numbers of interpolation nodes
for it=1:length(Nd)
    x = linspace(-1,1,Nd(it))'; % generate evenly-spaced points in [-1,1].
    y = f(x);  
    %% Form Vandermonde matrix "intelligently"
    V = zeros(length(x),length(x));
    for vit = 1:Nd(it)
        V(:,vit) = x.^(vit-1);
    end
    %% Compute coefficients by solving linear system
    a = V\y;
    %% Condition Number Vandermonde Matrix
    A = inv(V); % V^-1
    B = det(A); % ||V^-1||
    C = det(V); % ||V^-1||
    D = C * B;  % ||V||
    % disp(D);
    % cond(V);
    %% Evaluate polynomial interpolant by building eval matrix and
    %% multiplying with coefficients.
    Ve = zeros(length(xe),length(x));
    for vit = 1:Nd(it)
        Ve(:,vit) = xe.^(vit-1);
    end
    ye_num(:,it) = Ve*a; %Do the matrix-vector product to predict values
    %% Plot stuff
    hold on;
    plot(xe,ye_num(:,it),'o');
    drawnow
    pause
end
%% Now plot relative errors
figure
e2 = zeros(length(Nd),1);
for it=1:length(Nd)
    e2(it,1) = norm(ye_num(:,it)-ye)./norm(ye);
end
semilogy(Nd,e2);
pause %waits for input
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Start plotting stuff
figure
plot(xe,ye,'black');
pause %waits for input
%% Loop goes over different numbers of interpolation nodes
for it=1:length(Nd)
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
    a = V\y;%% Condition Number Vandermonde Matrix
    A = inv(V); % V^-1
    B = det(A); % ||V^-1||
    C = det(V); % ||V||
    D = C * B;  % ||V^-1|| ||V||
    % disp(D);
    %% Evaluate polynomial interpolant by building eval matrix and
    %% multiplying with coefficients.
    Ve = zeros(length(xe),length(x));
    for vit = 1:Nd(it)
        Ve(:,vit) = xe.^(vit-1);
    end
    ye_num(:,it) = Ve*a; %Do the matrix-vector product to predict values
    %% Plot stuff
    hold on;
    plot(xe,ye_num(:,it),'o');
    drawnow
    pause
end
%% Now plot relative errors
figure
e2 = zeros(length(Nd),1);
for it=1:length(Nd)
    e2(it,1) = norm(ye_num(:,it)-ye)./norm(ye);
end
semilogy(Nd,e2);
pause %waits for input
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Nd = [9,50];
ye_num = zeros(length(ye),length(Nd));
%% Start plotting stuff
figure
plot(xe,ye,'black');
pause %waits for input
%% Loop goes over different numbers of interpolation nodes
for it=1:length(Nd)
    x = linspace(-1,1,Nd(it))'; % generate evenly-spaced points in [-1,1]. 
    y = f(x);
    %% Form Vandermonde matrix "intelligently"
    V = zeros(length(x),length(x));
    for vit = 1:Nd(it)
        V(:,vit) = x.^(vit-1);
    end
    %% Evaluate polynomial interpolant by buildin%% Compute coefficients by solving linear system
    a = V\y;
    %% multiplying with coefficients.
    Ve = zeros(length(xe),length(x));
    for vit = 1:Nd(it)
        Ve(:,vit) = xe.^(vit-1);
    end
    ye_num(:,it) = Ve*a; %Do the matrix-vector product to predict values
    %% Plot stuff
    hold on;
    plot(xe,ye_num(:,it),'o');
    drawnow
    pause
end
%% Now plot relative errors
figure
e2 = zeros(length(Nd),1);
for it=1:length(Nd)
    e2(it,1) = norm(ye_num(:,it)-ye)./norm(ye);
end
semilogy(Nd,e2);