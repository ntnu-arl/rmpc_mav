function [u_ctrl,sol_found, flag_strange] = robustmpc_getInput(sol_x_mp, x_state_value)
%
%   get control action from the explicit robust mpc computed using YALMIP
%
%   Syntax:
%   [u_ctrl,sol_found] = robustmpc_getInput(sol_x_mp, x_state_value)
%
%   Inputs:
%   sol_x_mp        : Multiparametric Robust MPC explicit solution
%   x_state_value   : value of the state vector
%
%   Outputs:
%   u_ctrl          : control action
%   sol_found       : flag indicating whether a solution was found
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

sol_found = 1;
[i,j] = isinside(sol_x_mp{1}.Pn, x_state_value);

flag_strange = 0;
if(length(j)>1)
    j = j(1); % this requires further analysis
    flag_strange = 1;
end

if(isempty(j)==0)
    u_ctrl = sol_x_mp{1}.Fi{j}*x_state_value + sol_x_mp{1}.Gi{j};
    sol_found = 1;
else
    sol_found = 0;
    u_ctrl = NaN;
end

if(flag_strange == 1)
    u_ctrl = 0.0256;
end
end