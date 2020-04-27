clear;
clc;

load A.dat;

[n,d] = size(A);

% Part 2.A.First
AF10 = (norm(A, 'fro')^2) / 10;
fprintf("AF10 = %f\n", AF10);

% For Part 2.A
for l = 1:10
    B = FD(A,l);
    Error = norm(A'*A - B'*B, 2);
    fprintf("Error = %f, l = %d",Error, l);
    if Error > AF10
        fprintf(", Nope\n");
    else
        fprintf(", Yes\n");
    end
end

fprintf("\n\nAF = %f\n", (norm(A, 'fro')^2));
fprintf("AF3 = %f\n", (norm(A, 'fro')^2) / 3);

% Part 2.A.Third
[U, S, V] = svd(A);
k = 2;
Uk = U(:,1:k);
Sk = S(1:k,1:k);
Vk = V(:,1:k);
Ak = Uk*Sk*Vk';
AF10 = (norm(A-Ak, 'fro')^2) / 10;
fprintf("\n\nAF10 = %f\n", AF10);

% For Part 2.A
for l = 1:10
    B = FD(A,l);
    Error = norm(A'*A - B'*B, 2);
    fprintf("Error = %f, l = %d",Error, l);
    if Error > AF10
        fprintf(", Nope\n");
    else
        fprintf(", Yes\n");
    end
end

%%%%%%%%%%%%%%%%%% Problem 2.B %%%%%%%%%%%%%%%%%%%%%%%

t = 5;
T = zeros(1,t);
for k = 1:t
    for l = 1:1000
        S = randn(l,n);
        for i = 1:l
            for j = 1:n
                S(i,j) = (sqrt(l)/sqrt(n))*S(i,j);
            end
        end
        B = S*A;
        Error = norm(A'*A - B'*B, 2);
        if Error > AF10
            
        else
            T(1,k) = l;
            break;
        end
    end
end
maxResult = max(T);
fprintf("\n\nResult = %f",maxResult);