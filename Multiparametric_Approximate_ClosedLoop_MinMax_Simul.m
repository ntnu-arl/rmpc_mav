function [x_state_k,sol_x_mp,diagnostics_x,aux_x,ValueFunction_x,Optimizer_x] = Multiparametric_Approximate_ClosedLoop_MinMax_Simul(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type,Simul_Steps)
%
%   Syntax:
%   sol_x_mp = Multiparametric_Approximate_ClosedLoop_MinMax(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type)
%   
%   Inputs:
%   add_usys_d      :
%   x_state         :
%   Y_x_Limit       :
%   U_x_bounds      :
%   W_x_bounds      :
%   Q               :
%   R               :
%   N               :
%   norm_type       :
%   Simul_Steps     :
%
%   Outputs:
%   x_state_k       :
%
%
%   A multiparametric solution to the approximate closed-loop MinMax 
%   solution.
%
%   Code based on: YALMIP Wiki (users.isy.liu.se)
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)
%

W_x = sdpvar(N,1);
x_state = sdpvar(length(x_state),1);

%We create a causal feedback U = LW + V and derive the predicted states.  
V_x = sdpvar(N,1);
L_x = sdpvar(N,N,'full').*(tril(ones(N))-eye(N));

U_x = L_x*W_x + V_x;

Y_x = [];
x_state_k = x_state;
for k = 1:N
 x_state_k = add_usys_d.matrices.A*x_state_k + add_usys_d.matrices.B*U_x(k)+add_usys_d.matrices.E*W_x(k);
 Y_x = [Y_x;add_usys_d.matrices.C*x_state_k];
end

F_x = [Y_x <= Y_x_Limit, U_x_bounds(1) <= U_x <= U_x_bounds(2)];
objective_x = norm(Y_x-1,norm_type)*Q + norm(U_x,norm_type)*R;
G_x = [W_x_bounds(1) <= W_x <= W_x_bounds(2)];

[Frobust_x,h_x] = robustify(F_x + G_x, objective_x, [], W_x);
[sol_x_mp,diagnostics_x,aux_x,ValueFunction_x,Optimizer_x] = solvemp(Frobust_x,h_x,[],x_state,U_x(1));

% 
% [Frobust,h] = robustify(F + G,objective,[],W);
% sol = solvemp(Frobust,h,[],x);

% sol_x_mp = solvemp([F_x,G_x,uncertain(W_x)],objective_x,[],x_state);

% x_state_k = [0;0;0;0;];
x_state_k = zeros(length(x_state));
sol_x_mp = [];
ops = sdpsettings;
% for i = 1:Simul_Steps
% %    sol_x_mp = solvemp([F_x,G_x,uncertain(W_x)],objective_x,[],x_state);
%     [Frobust_x,h_x] = robustify(F_x + G_x, objective_x, [], W_x);
%     sol_x_mp{i} = solvemp(Frobust_x,h_x,[],x_state);
%   %  solvesdp([Frobust_x, x_state == x_state_k(:,end)],h_x,ops);
%     x_state_k = [x_state_k add_usys_d.matrices.A*x_state_k(:,end) + add_usys_d.matrices.B*double(U_x(1)) + add_usys_d.matrices.E*(-1+2*rand(1))];
% end

end