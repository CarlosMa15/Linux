clear;
clc;

syms y0 y1 y2 x0 x1 x2 x h;

f = subs((y0*(((2*x)-x1-x2)/((x0-x1)*(x0-x2)))) + (y1*(((2*x)-x0-x2)/((x1-x0)*(x1-x2)))) + (y2*(((2*x)-x0-x1)/((x2-x0)*(x2-x1)))),x2 - x1,h);
f = subs(f,x0-x2,-2*h);
f = subs(f,x0-x1,-h);
f = subs(f,x2,h+x1);
f = subs(f,x,x0);
A5 = subs(f,x0-x1,-h)

f = subs((y0*(((2*x)-x1-x2)/((x0-x1)*(x0-x2)))) + (y1*(((2*x)-x0-x2)/((x1-x0)*(x1-x2)))) + (y2*(((2*x)-x0-x1)/((x2-x0)*(x2-x1)))),x2 - x1,h);
f = subs(f,x0-x2,-2*h);
f = subs(f,x0-x1,-h);
f = subs(f,x2,h+x1);
f = subs(f,x,x1);
f = subs(f,x0-x1,-h);
B5 = simplify(f)

f = y0*((x-x1)/(x0-x1))+y1*((x-x0)/(x1-x0));
f = int(f,x,x0,x1);
A7 = subs(f,x0-x1,-h)