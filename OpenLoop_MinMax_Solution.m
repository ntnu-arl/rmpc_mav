function x_state_k = OpenLoop_MinMax_Solution(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type,Simul_Steps)
%
%   Syntax:
%   x_state_k = OpenLoop_MinMax_Solution(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type,Simul_Steps)
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
%   An open-loop Minimax solution to the problem
%   
%   In an open-loop fashion we try to minimze the objective function under
%   the worst possbile assumption for the bounded uncertainty. 
%
%   Code based on: YALMIP Wiki (users.isy.liu.se)
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)
%
U_x = sdpvar(N,1);
W_x = sdpvar(N,1);
x_state = sdpvar(length(x_state),1);

Y_x = [];
x_state_k = x_state;
for k = 1:N
 x_state_k = add_usys_d.matrices.A*x_state_k + add_usys_d.matrices.B*U_x(k)+add_usys_d.matrices.E*W_x(k);
 Y_x = [Y_x;add_usys_d.matrices.C*x_state_k];
end

[H,Su] = create_CHS(add_usys_d.matrices.A,add_usys_d.matrices.B,add_usys_d.matrices.C,N);
[H,Sw] = create_CHS(add_usys_d.matrices.A,add_usys_d.matrices.E,add_usys_d.matrices.C,N);
Y_x = H*x_state_k + Su*U_x + Sw*W_x;

F_x = [Y_x <= Y_x_Limit, U_x_bounds(1) <= U_x <= U_x_bounds(2)];

objective_x = norm(Y_x-1,norm_type)*Q + norm(U_x,norm_type)*R;
G_x = [W_x_bounds(1) <= W_x <= W_x_bounds(2)];
[Frobust_x,h_x] = robustify(F_x + G_x,objective_x,[],W_x);

ops = sdpsettings;

controller_x = optimizer([F_x, G_x, uncertain(W_x)],objective_x,ops,x_state,U_x(1));
x_state_k = zeros(length(x_state),1); %[0;0;0;0;];
for i = 1:Simul_Steps
    x_state_k = [x_state_k add_usys_d.matrices.A*x_state_k(:,end) + add_usys_d.matrices.B*controller_x{x_state_k(:,end)} + add_usys_d.matrices.E*(-1+2*rand(1))];
end

end

