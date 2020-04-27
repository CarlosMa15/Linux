clear;
clc;

third = 1 / 3;

L = [0 .5 0 0; third 0 0 .5; third 0 0 .5; third .5 1 0]
r = [.25 .25 .25 .25]'

for i = 1 : 100
    r = L * r
end