%%  Rotorcraft Robust MPC Control Design
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

run Init_RobustMPC;

%%  Load Rotorcraft Parameters
run example_rotorcraft_model

%%  Define Additive Uncertain Model
E_x = 0.1*B_x;
E_z = 0.1*B_z;
E_y = 0.1*B_y;
add_usys_d = add_uss(A_x,B_x,C_x,D_x,E_x,Ts)
%add_usys_d = add_uss(A_rp,B_rp,C_rp,D_rp,0.1*B_rp,Ts);
%add_usys_d = add_uss(A_z,B_z,C_z,D_z,E_z,Ts)
add_usys_x_d = add_uss(A_x,B_x,C_x,D_x,E_x,Ts)
add_usys_y_d = add_uss(A_y,B_y,C_y,D_y,E_y,Ts)
add_usys_z_d = add_uss(A_z,B_z,C_z,D_z,E_z,Ts)


t = 0:0.01:100;
u = 10*sin(t);
[y,t] = lsim_addusys(add_usys_d,[-0.5 0.5],u,t);

%%  Open-Loop MinMax Solution
norm_type = 1;
W_x_bounds = [-0.5 0.5];
N = 5; 

Y_x_Limit = 1;
% Y_x_Limit = 1*ones(min(size(add_usys_d.matrices.C))*N,1);
U_x_bounds = [-10 10];
x_state = sdpvar(length(add_usys_d.matrices.A),1);

Q = 10;
R = 0.1;

disp('Open-Loop MiniMax Solution');
Simul_Steps = 100;
x_state_k = OpenLoop_MinMax_Solution(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type,Simul_Steps);
close all
plot(x_state_k(2,1:end))

%%  Approximate Closed Loop MinMax Solution
    
norm_type = 1;
W_x_bounds = [-0.5 0.5];
Y_x_Limit = 1;
% Y_x_Limit = 1*ones(min(size(add_usys_d.matrices.C))*N,1);

U_x_bounds = [-100 100];

Q = 10;
R = 0.1;

x_state_k = Approximate_ClosedLoop_MinMax_Solution(add_usys_d,x_state,Y_x_Limit,U_x_bounds,W_x_bounds,Q,R,N,norm_type,Simul_Steps)
hold on
plot(x_state_k(2,1:end),'r')

%%  MultiParametric Approximate Closed Loop MinMax Solution

x_state = sdpvar(length(add_usys_d.matrices.A),1);
norm_type = 1;
W_x_bounds = 0*[-0.05 0.05];
N = 2; 

Y_x_Limit_orig = [Inf 1 pi/4 pi];
%Y_x_Limit_orig = [2 .8];

% Y_x_Limit_orig = [Inf 1 1];
Y_x_Limit = expand_Yconstraints(Y_x_Limit_orig,N)

U_x_bounds = [-1 1];

Q = diag([100 .1 25 .5]);
R = 0.001;
% Q = diag([100 .01 .01 .01]);
% R = 0.001;
% Q = diag([25 .5]);
% R = .15;

Y_ref = [0 0 0 0]';
%Y_ref = [0 0]';

%
run Multiparametric_Approximate_ClosedLoop_MinMax_Script_vW1

beep 
%%
flag_sim = input('Show Simulation? [0/1]');
if(flag_sim == 1)
    x_state_init = zeros(length(x_state),1); x_state_init = [-1 0 0 0]'; % x_state_init = [2 0]';
    time_sec = 10; % 10 secs
    time_sec = input('how long [s]?');
    [y,u_ctrl_seq,t] = simulate_Multiparametric_Approximate_ClosedLoop_MinMax(add_usys_d,sol_x_mp,x_state_init,time_sec,Optimizer_x,x_state);
end
%%
x_bounds_min = -[1 1 1 1];
x_bounds_max =  [1 1 1 1];
desired_samples_per_state = 2;
sol_x_mp_rt = assembly_sol_x_mp_RealTime(sol_x_mp,x_bounds_min,x_bounds_max,desired_samples_per_state);

%%  Alternative Methods and plots

assign(x_state,[1 0 1 1]')
double(Optimizer_x)
%%

plot(ValueFunction_x)
figure
plot(Optimizer_x(1));

%%  Dynamic Programming Exact Solution

norm_type = 1;
W_x_bounds = [-0.5 0.5];
Y_x_Limit_orig = [Inf 1 pi 2*pi];
Y_x_Limit = expand_Yconstraints(Y_x_Limit_orig,N);
U_x_bounds = [-100 100];

X_expl_bounds = [-100 100];

sol_x = DP_Exact_ClosedLoop_MinMax(add_usys_d,x_state,Y_x_Limit,U_x_bounds,X_expl_bounds,W_x_bounds,Q,R,N,norm_type)

