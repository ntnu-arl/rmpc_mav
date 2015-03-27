function sol_x_mp = Multiparametric_Approximate_ClosedLoop_MinMax(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type)
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

sol_x_mp = solvemp([F_x,G_x,uncertain(W_x)],objective_x,[],x_state);

end