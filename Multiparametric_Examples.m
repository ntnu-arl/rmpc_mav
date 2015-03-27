%%  Multiparametric Programming Examples
%
%   This tutorial requires MPT

%%  Generic Example:
%   Consider the following simple quadratic program in the decision
%   variable z, solved for a particular value on a parameter x

A = randn(15,3);
b = rand(15,1);
E = randn(15,2);

z = sdpvar(3,1);
x = [0.1;0.2];

F = [A*z <= b+E*x];
obj = (z-1)'*(z-1);

sol = solvesdp(F,obj);
double(z)

%   to obtain the parametric solution with respect to x, we call the
%   function 'solvemp', and tell the solver that x is the parametric
%   variable. Moreover, we must add constraints on x to define the region
%   where we want to compute the parametric solution, the so called
%   exploration set

x = sdpvar(2,1);
F = [A*z <= b + E*x, -1 <= x <= 1];
sol = solvemp(F,obj,[],x);

%   the first output is an MPT structure. In accordance with MPT syntax,
%   the optimizer for the parametric value (0.1, 0.2) is given by the
%   following code

xx = [0.1; 0.2];
[i,j] = isinside(sol{1}.Pn,xx);
sol{1}.Fi{j}*xx + sol{1}.Gi{j}

%   by using more outputs from solvemp, it is possible to simplify things
%   considerably

[sol,diagnostics,aux,Valuefunction,Optimizer] = solvemp(F,obj,[],x);

%   the function now returns solutions using YALMIPs nonlinear operator
%   framework. To retrieve the numerical solution for a particular
%   parameter value, simply use assign and double in standard fashion. 

assign(x,[0.1; 0.2]);
double(Optimizer)

%   some of the plotting capabilities of MPT are overloaded for the
%   piecewise quadratic value function

plot(Valuefunction);
figure
plot(Optimizer);

%   and plot the piecewise affine optimizer

figure
plot(Optimizer(1));