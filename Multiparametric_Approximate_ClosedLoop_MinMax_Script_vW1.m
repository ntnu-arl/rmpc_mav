%%  Multiparametric Approximate Closed-Loop MinMax
%   runs as script
%
%   Vriables that have to be defined before:
%   add_usys_d  : the system of additive uncertainty
%   x_state     : x_state sdp var
%   Y_x_Limit   : limit of output
%   U_x_bounds  : Bounds of Input
%   W_x_bounds  : Bounds of disturbance
%   Q           : Q Penalizing matrix
%   R           : R Penalizing matrix
%   N           : Prediction horizon
%   norm_type   : norm type
%
%   A multiparametric solution to the approximate closed-loop MinMax 
%   solution.
%
%   Code based on: YALMIP Wiki (users.isy.liu.se)
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)
%

%   sdpvar for disturbing input and state
W_x = sdpvar(N,1);
x_state = sdpvar(length(x_state),1);

%We create a causal feedback U = LW + V and derive the predicted states.  
V_x = sdpvar(N,1);
L_x = sdpvar(N,N,'full').*(tril(ones(N))-eye(N));

U_x = L_x*W_x + V_x;

%   create predicting output vector (length = N*length(y))
Y_x = [];
x_state_k = x_state;
for k = 1:N
	x_state_k = add_usys_d.matrices.A*x_state_k + add_usys_d.matrices.B*U_x(k)+add_usys_d.matrices.E*W_x(k);
	Y_x = [Y_x;add_usys_d.matrices.C*x_state_k];
end

%   define the state and input constraints
F_x = [Y_x >= -Y_x_Limit, Y_x <= Y_x_Limit, U_x_bounds(1) <= U_x <= U_x_bounds(2)];

%   Fix the size of Y ref for horizon
N_y = length(Y_ref);
size_B = size(add_usys_d.matrices.B); N_u = size_B(2);
clear Y_ref_hor
for kk = 1:N
    Y_ref_hor(((kk-1)*N_y+1):(kk*N_y)) = Y_ref;
end

%   Fix the size of Q,R for the prediction horizon value
clear Q_hor R_hor
Q_hor (1:(N*N_y),1:(N*N_y)) = 0;
R_hor (1:(N*N_u),1:(N*N_u)) = 0;

for kk = 1:N
    Q_hor(((kk-1)*N_y+1):(kk*N_y),((kk-1)*N_y+1):(kk*N_y)) = Q;
    R_hor(((kk-1)*N_u+1):(kk*N_u),((kk-1)*N_u+1):(kk*N_u)) = R;
end

%   set the objective function -> currently it only supports 1-, inf-norm
objective_x = norm((Q_hor*(Y_x-Y_ref_hor')),norm_type) + norm(R_hor*U_x,norm_type);

%   define the bounds of the disturbance
G_x = [W_x_bounds(1) <= W_x <= W_x_bounds(2)];

%   Derive robust counterpart
[Frobust_x,h_x] = robustify(F_x + G_x, objective_x, [], W_x);

%   compute explicit robust controller
[sol_x_mp,diagnostics_x,aux_x,ValueFunction_x,Optimizer_x] = solvemp(Frobust_x,h_x,[],x_state,U_x(1));

disp('### Multiparametric Robust MPC computed');
