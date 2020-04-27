clear;
clc;
load A.dat
[U, S, V] = svd(A);

figure
plot(U(:,1),U(:,2),'o');

n = size(A,1);
K = 1:10;
ANormTenPercent = norm(A,2) * .10;
fprintf("fio A 10 Percent = %f\n",ANormTenPercent);

for i = 1 : size(K,2)
    k = K(i);
    Uk = U(:,1:k);
    Sk = S(1:k,1:k);
    Vk = V(:,1:k);
    Ak = Uk*Sk*Vk';
    L2norm = norm(A-Ak,2);
    fprintf("k = %d, L2Norm = %f",k,L2norm);
    if L2norm > ANormTenPercent
        fprintf(" Nope\n");
    else
        fprintf(" Yep\n");
    end
end

In = eye(n);
div = 1 / n;
oneMatrix = ones(n)';
Cn = In - (div * transpose(oneMatrix));
An = Cn*A;
[U, S, V] = svd(An);

figure
plot(U(:,1),U(:,2),'o');

x = zeros(40*40*5000,2);
c = 1;
for k = 1:5000
for i = 1:40
    for j = 1:40
        x(c,1) = dot(An(k,i),V(1,j));
        x(c,2) = dot(An(k,i),V(2,j));
        c = c + 1;
    end
end
end

figure
plot(x(:,1),x(:,2),'o');