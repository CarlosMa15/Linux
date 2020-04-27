clear;
clc;

load M.dat
load W.dat

M; % Matrix

W; % Vector

r = W' % Row

X = r * M; % Row

max = X(1);
index = 1;
for i = 1 : size(X,2)
    element = abs(X(i));
    
    if element > max
        max = element;
        index = i;
    end
end

index

column = M(:,index); % column

r = r - column'

X = r * M; % Row

max = X(1);
index = 1;
for i = 1 : size(X,2)
    element = abs(X(i));
    
    if element > max
        max = element;
        index = i;
    end
end

index

column = M(:,index); % column

r = r - column'

X = r * M; % Row

max = X(1);
index = 1;
for i = 1 : size(X,2)
    element = abs(X(i));
    
    if element > max
        max = element;
        index = i;
    end
end

index

column = M(:,index); % column

r = r - column'

X = r * M; % Row

max = X(1);
index = 1;
for i = 1 : size(X,2)
    element = abs(X(i));
    
    if element > max
        max = element;
        index = i;
    end
end

index

column = M(:,index); % column

r = r - column'

X = r * M; % Row

max = X(1);
index = 1;
for i = 1 : size(X,2)
    element = abs(X(i));
    
    if element > max
        max = element;
        index = i;
    end
end

index

column = M(:,index); % column

r = r - column'