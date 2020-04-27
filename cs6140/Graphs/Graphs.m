%% Clearing everything for a fresh start
clear;
clc;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Part A

%% Load matrix M
load M.dat;

%% Random Walk
RandomWalk = zeros(1,10);
a = 1000;
for k = 1:a
     t = 100;
     C = 1;
     LC = zeros(1,10);
     LC(1,1) = 1;
     Location = 1;
     for i = 1: t
         A = M(:,C)* 10;
         R = randi(10);
         C = 1;
         while true
            R;
            C;
             if A(C,1) == 0 && R ~= 0
                 C = C + 1;
             elseif A(C,1) == 0 && R == 0
                     break
             else
                 A(C,1) = A(C,1) - 1;
                 R = R -1;
                 if R == 0
                     break
                 end
             end
         end
         C;
         if i ~= t
            LC(1,C) = LC(1,C) + 1;
         end

     end

     LC = LC / sum(LC);

     for l = 1:10
         RandomWalk(1,l) = RandomWalk(1,l) + LC(1,l);
     end
end

for l = 1:10
     RandomWalk(1,l) = RandomWalk(1,l) / a;
end

RandomWalk = RandomWalk';

%% Eigen-Analysis
[V,L,W] = eig(M);
V = V(:,1);
EigenAnalysis = V/sum(V);

%% Calculate M^1024
for i = 1:10
    M = M * M;
end

%% Create q which start at position 1
q = [1 0 0 0 0 0 0 0 0 0 ]';

%% Matrix Power
MatrixPower = M * q;

%% load matrix
load M.dat

%% State Propagation
Q = q;
for i = 1: 1024
    Q = M * Q;
end
StateProgation = Q;

%% load matrix
load M.dat

%% Calculate M^1024
for i = 1:10
    M = M * M;
end

%% Create q which start at position 1
q = [1 0 0 0 0 0 0 0 0 0 ]';

%% Matrix Power
MatrixPower = M * q;

%% Load matrix
load M.dat

%% State Propagation
Q = q;
for i = 1: 1024
    Q = M * Q;
end
StateProgation = Q;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Part B

%% Create q which start at position 1
q = [ 0.1 0.1 0.1 0.1 0.1 0.1 0.1 0.1 0.1 0.1 ]';

%% Load matrix
load M.dat

%% State Propagation
Q = q;
t = 0;
% while Q ~= StateProgation
for i = 1: 85
    t = t + 1;
    Q = M * Q;
end
Q
StateProgationT = t

%% Load matrix
load M.dat

R = M;
for i = 0: 85
    R = R * M;
end

R * q

MatrixPowerT = 85

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Part D

q = [0 0 0 1 0 0 0 0 0 0 ]';

%% State Propagation
Q = q;
Q = M * Q;
StateProgationD = Q