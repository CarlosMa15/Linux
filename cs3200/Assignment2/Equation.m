clear;
clc;

syms x x0 x1 x2 y0 y1 y2 h a b c;

f = (x - x0)*(x - x1)*(x - x2);
f = int(f,x,x0,x2)
f = ((-2 * h)^3*(x0 - 2*(x0 + h) + (x1 + h)))/12