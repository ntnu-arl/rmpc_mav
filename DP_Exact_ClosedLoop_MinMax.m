function sol_x = DP_Exact_ClosedLoop_MinMax(add_usys_d,x_state,Y_x_Limit,U_x_bounds,X_expl_bounds,W_x_bounds,Q,R,N,norm_type)
%
%   Syntax:
%   sol_x = DP_Exact_ClosedLoop_MinMax(add_usys_d,x_state,Y_x_Limit,U_x_bounds,X_expl_bounds,W_x_bounds,Q,R,N,norm_type)
%   
%   Inputs:
%   add_usys_d      :
%   x_state         :
%   Y_x_Limit       :
%   U_x_bounds      :
%   ,X_expl_bounds  :
%   W_x_bounds      :
%   Q               :
%   R               :
%   N               :
%   norm_type       :
%   Simul_Steps     :
%
%   Outputs:
%   sol_x           :
%
%   A Dynamic Programming-based exact solution to the closed loop MinMax
%   problem
%
%   Code based on: YALMIP Wiki (users.isy.liu.se)
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)
%


%Dynamic programming solution to closed-loop minimax problem
disp('### Dynamic Programming solution to closed-loop minimax problem');
%It should be mentioned that, for some problems, an exact closed-loop
%solution can probably be computed more efficiently with dynamic
%programming along the lines of the dynamic programming examples.    

%Recall the DP code for the dynamic programming example for LTI systems to
%solve our problem without any uncertainty.   

% Model data
nx = length(x_state); % Number of states
nu = 1; % Number of inputs

% States x(k), ..., x(k+N)
x_state = sdpvar(repmat(nx,1,N),repmat(1,1,N));
% Inputs u(k), ..., u(k+N) (last one not used)
u_x = sdpvar(repmat(nu,1,N),repmat(1,1,N));

J_x{N} = 0;

for k = N-1:-1:1   
    % Feasible region
    F_x = [U_x_bounds(1) < u_x{k} < U_x_bounds(2)];
    F_x = [F_x, add_usys_d.matrices.C*x_state{k}   < Y_x_Limit];
    F_x = [F_x, add_usys_d.matrices.C*x_state{k+1} < Y_x_Limit];

    % Bounded exploration space
    % (recommended for numerical reasons)
    F_x = [F_x, X_expl_bounds(1) <= x_state{k} <= X_expl_bounds(2)];

    % Dynamics
    F_x = [F_x, x_state{k+1} == add_usys_d.matrices.A*x_state{k}+add_usys_d.matrices.B*u_x{k}];

    % Cost in value iteration
    obj_x = norm(add_usys_d.matrices.C*x_state{k}-1,norm_type)*Q + norm(u_x{k},norm_type)*R + J_x{k+1};

    % Solve one-step problem   
    [sol_x{k},diagnost_x{k},Uz_x{k},J_x{k},Optimizer_x{k}] = solvemp(F_x,obj_x,[],x_state{k},u_x{k});
end

%
%We will now make some small additions to solve this problem robustly, i.e.
%minimize worst-case cost and satisfy constraints for all disturbances.    

%The first change is that we cannot work with equality constraints to
%define the state dynamics, since the dynamics are uncertain. Instead, we
%add constraints on the uncertain prediction equations.    

%Furthermore, the value function J{k+1} is defined in terms of the variable
%x{k+1}, but since we do not link x{k+1} with x{k} and u{k} with an
%equality constraint because of the uncertainty, we need to redefine the
%value function in terms of the uncertain prediction, to make sure that the
%objective function will be the worst-case cost.    

% Uncertainty w(k), ..., w(k+N) (last one not used)
w_x = sdpvar(repmat(1,1,N),repmat(1,1,N));

J_x{N} = 0;

for k = N-1:-1:1   

    % Feasible region
    F_x = [U_x_bounds(1) < u_x{k}     < U_x_bounds(2)];
    F_x = [F_x, add_usys_d.matrices.C*x_state{k} < Y_x_Limit];
    F_x = [F_x, add_usys_d.matrices.C*(add_usys_d.matrices.A*x_state{k} + add_usys_d.matrices.B*u_x{k} + add_usys_d.matrices.E*w_x{k}) < Y_x_Limit];

    % Bounded exploration space
    F_x = [F_x, X_expl_bounds(1) <= x_state{k} <= X_expl_bounds(2)];

    % Uncertain value function
    Jw_x = replace(J_x{k+1},x_state{k+1},add_usys_d.matrices.A*x_state{k} + add_usys_d.matrices.B*u_x{k} + add_usys_d.matrices.E*w_x{k});

    % Declare uncertainty model
    F_x = [F_x, uncertain(w_x{k})];
    F_x = [F_x, W_x_bounds(1) <= w_x{k} <= W_x_bounds(2)];

    % Cost in value iteration
    obj_x = norm(add_usys_d.matrices.C*x_state{k}-1,norm_type)*Q + norm(u_x{k},norm_type)*R + Jw_x;

    % Solve one-step problem   
    [sol_x{k},diagnost_x{k},Uz_x{k},J_x{k},Optimizer_x{k}] = solvemp(F_x,obj_x,[],x_state{k},u_x{k});
end

%Please note that this multiparametric problem seems to grow large, hence
%it will take a fair amount of time to compute. Rest assured though, we are
%constantly working on improving performance in both MPT and YALMIP. 

end

