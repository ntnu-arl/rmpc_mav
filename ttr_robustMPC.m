%%  Tri Tilt-Rotor Robust MPC Control Design
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

run Init_RobustMPC;

%%  Load TTR Parameters
Ts = 0.25;
run example_ttr_model

%%  Define Additive Uncertain Model
E_x = 0.1*B_x;
E_y = 0.1*B_y;
E_z = 0.1*B_z;
E_x = [0 1 0]'; E_y = [0 1 0]';B_z = [0 .3 0]';

add_usys_x_d = add_uss(A_x,B_x,C_x,D_x,E_x,Ts)
add_usys_y_d = add_uss(A_y,B_y,C_y,D_y,E_y,Ts)
add_usys_z_d = add_uss(A_z,B_z,C_z,D_z,E_z,Ts)

add_usys_d = add_usys_z_d;

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
W_x_bounds = [-0.25 0.25];
N = 3; 

%Y_x_Limit_orig = [Inf 1 pi/4 pi];
Y_x_Limit_orig = [Inf 1 deg2rad(15)]; % x 
Y_x_Limit_orig = [Inf 1 deg2rad(45)]; % y 
Y_x_Limit_orig = [Inf 2 5]; % z
% Y_x_Limit_orig = [Inf 1 1];
%Y_x_Limit = expand_Yconstraints(Y_x_Limit_orig,N)

U_x_bounds = [-deg2rad(10) deg2rad(10)]; % x_controller
U_x_bounds = [-deg2rad(15) deg2rad(15)]; % y_controller
U_x_bounds = [-5 5]; % z_controller

% Q = diag([100 .01 .01 .01]);
% R = 0.001;
Q = diag([25 .5 .1]); % x_controller
Q = diag([20 .2 .05]); % y_controller
%Q = diag([11.5 .34 .1]); % y_controller
Q = diag([15 1.8 .55]); % z_controller

R = .15; % x_controller
%R = .15; % y_controller
%R = .125; % z_controller
Y_ref = [0 0 0]';

[sol_x_mp,ValueFunction_x,MP_SolutionOut] = MP_CL_MinMax_SDPrelax(add_usys_d, x_state, Y_ref, Y_x_Limit_orig,U_x_bounds,W_x_bounds,Q,R,N,norm_type)

%%  Simulate in time
flag_sim = input('Show Simulation? [0/1]');
if(flag_sim == 1)
    x_state_init = zeros(length(x_state),1); x_state_init = [1 0 0]';
    time_sec = 10; % 10 secs
    time_sec = input('how long [s]?');
    [y,u_ctrl_seq,t] = simulate_Multiparametric_Approximate_ClosedLoop_MinMax(add_usys_d,sol_x_mp,x_state_init,time_sec,MP_SolutionOut.Optimizer_x,W_x_bounds,x_state);
end
%%  Export and Test Real-Time Controller

ExplicitRobustMPC_obj = Assemble_ExplicitRobustMPC_obj(sol_x_mp,add_usys_d,U_x_bounds,Y_x_Limit_orig)
%%
%   1. validator:
sol_x_mp{1}.Ai = cell(1,length(sol_x_mp{1}.Ai));
sol_x_mp{1}.Bi = sol_x_mp{1}.Fi;
sol_x_mp{1}.Ci = sol_x_mp{1}.Gi;
Optimizer = pwf(sol_x_mp{1},x_state,'general')

%   derive the matrices for real-time execution
[H,K,F,G,Nc] = GetRobustMPC_Matrices(sol_x_mp{1});
Nx = length(add_usys_d.matrices.A);
%%
%   validate
clc
xx = -[0.01151 0.089 0.009]';
assign(x_state,xx);
[U,flag_oob] =  RobustMPCInput(xx,ExplicitRobustMPC_obj.H,ExplicitRobustMPC_obj.K,ExplicitRobustMPC_obj.F,ExplicitRobustMPC_obj.G,ExplicitRobustMPC_obj.Nc,ExplicitRobustMPC_obj.Nx)

[u_ctrl,sol_found, flag_strange] = robustmpc_getInput(sol_x_mp, xx);
assign(x_state,xx);
double(Optimizer)

%%
TTR_RobustMPC.Xdir = ExplicitRobustMPC_obj;
TTR_RobustMPC.Ydir = ExplicitRobustMPC_obj; 
TTR_RobustMPC.Zdir = ExplicitRobustMPC_obj; 

%%
TTR_RobustMPC.Xdir.Explicit.H = H;
TTR_RobustMPC.Xdir.Explicit.K = K;
TTR_RobustMPC.Xdir.Explicit.F = F;
TTR_RobustMPC.Xdir.Explicit.G = G;
TTR_RobustMPC.Xdir.Explicit.Nc = Nc;
TTR_RobustMPC.Xdir.Explicit.Nx = Nx;
%%
TTR_RobustMPC.Zdir.Explicit.H = H;
TTR_RobustMPC.Zdir.Explicit.K = K;
TTR_RobustMPC.Zdir.Explicit.F = F;
TTR_RobustMPC.Zdir.Explicit.G = G;
TTR_RobustMPC.Zdir.Explicit.Nc = Nc;
TTR_RobustMPC.Zdir.Explicit.Nx = Nx;
TTR_RobustMPC.Zdir.Explicit.Ts = add_usys_d.Ts;



