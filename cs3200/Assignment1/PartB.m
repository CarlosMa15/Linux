clear; 
clc;

b = rand(1,30);

%% Loop goes over different numbers of interpolation nodes
for it = 1:30
    x = linspace(-1,1,it)';
    V = zeros(length(x),length(x));
    for vit = 1:it
        V(:,vit) = x.^(vit-1);
    end
    b(it) = cond(V);    
end

a = (1:30);

%% Start plotting stuff
figure
semilogy(a,b);
hold on
pause %waits for input

c = rand(1,30);

%% Loop goes over different numbers of interpolation nodes
for it = 1:30
    x = rand(1,it);
    for itt = 1:it
        x(itt) = cos((itt - 1) * pi/((it + 1) - 1));
    end
    x = x'; 
    V = zeros(length(x),length(x));
    for vit = 1:it
        V(:,vit) = x.^(vit-1);
    end
    c(it) = cond(V);    
end

semilogy(a,c);