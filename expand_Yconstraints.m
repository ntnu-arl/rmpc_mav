function Y_x_Limit = expand_Yconstraints(Y_x_Limit_orig,N)
%
%   Expands the output constraints to the prediction horizon
%
%   Syntax: 
%   Y_x_Limit = expand_Yconstraints(Y_x_Limit_orig,N)
%
%   Inputs:
%   Y_x_Limit_orig  : the output constraint
%   N               : the prediction horizon
%
%   Outputs:
%   Y_x_Limit       : Expanded constraint for the optimization functions
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

Y_x_Limit = [];
for i = 1:N
    Y_x_Limit((end+1):(end+length(Y_x_Limit_orig))) = Y_x_Limit_orig;
end

Y_x_Limit = Y_x_Limit';

end