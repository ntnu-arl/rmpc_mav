function x_state_k = Approximate_ClosedLoop_MinMax_Solution(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type,Simul_Steps)
%
%   Syntax:
%   x_state_k = Approximate_ClosedLoop_MinMax_Solution(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type,Simul_Steps)
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
%   The idea in (Lofberg_2003) was to parametrize future inputs as affine
%   functions of past disturbances. This, in contrast to parametrization in
%   past states, lead to convex and tractable problems.    
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

%A reasonable implementation of the worst-case scenario with this
%approximate closed-loop feedback is given by the following code.    
F_x = [Y_x <= Y_x_Limit, U_x_bounds(1) <= U_x <= U_x_bounds(2)];
objective_x = norm(Y_x-1,norm_type)*Q + norm(U_x,norm_type)*R;
G_x = [W_x_bounds(1) <= W_x <= W_x_bounds(2)];

[Frobust_x,h_x] = robustify(F_x + G_x,objective_x,[],W_x);
%%
x_state_k = zeros(length(x_state),1);

ops = sdpsettings;
for i = 1:Simul_Steps
    solvesdp([Frobust_x, x_state == x_state_k(:,end)],h_x,ops);
    x_state_k = [x_state_k add_usys_d.matrices.A*x_state_k(:,end) + add_usys_d.matrices.B*double(U_x(1)) + add_usys_d.matrices.E*(-1+2*rand(1))];
end

end

