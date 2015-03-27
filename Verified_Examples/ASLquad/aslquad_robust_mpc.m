%%  ASLquad Robust MPC Control Design
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

% run Init_RobustMPC;

%%  Load TTR Parameters
Ts = 0.16;
Ts = 0.08;
Ts = 0.04;

run example_ASLquad_model

%%  Define Additive Uncertain Model
E_x = 0.1*B_x;
E_y = 0.1*B_y;
% E_z = 0.1*B_z;
E_x = [0 .75 0 0.00]'; E_y = [0 .75 0 0.00]';E_z = [0 .15]';

add_usys_x_d = add_uss(A_x,B_x,C_x,D_x,E_x,Ts)
add_usys_y_d = add_uss(A_y,B_y,C_y,D_y,E_y,Ts)
add_usys_z_d = add_uss(A_z,B_z,C_z,D_z,E_z,Ts)

add_usys_d = add_usys_x_d;

t = 0:0.01:100;
u = 10*sin(t);
[y,t] = lsim_addusys(add_usys_d,[-0.5 0.5],u,t);

%%  MultiParametric Approximate Closed Loop MinMax Solution
close all;

x_state = sdpvar(length(add_usys_d.matrices.A),1);
norm_type = Inf
W_x_bounds = [-0.2 0.2];
W_x_bounds = [-0.05 0.05];
W_x_bounds = 0*[-0.05 0.05];

N = 2; 

Y_x_Limit_orig = [Inf 2 deg2rad(45) deg2rad(360)]; % x 
% Y_x_Limit_orig = [Inf 10]; % z 

U_x_bounds = [-deg2rad(30) deg2rad(30)]; % xy_controller
% U_x_bounds = [-5 5]; % z_controller

%   norm_type = 1;
Q = 0.8*diag([22.5 5 .1 .01]); % x_controller
Q = .65*diag([71 12 1.5 0.3]); % x_controller
Q = .65*diag([71 15 1.5 0.3]); % x_controller
Q = .65*diag([71 17.5 1.65 0.55]); % x_controller
Q = .65*diag([71 7.5 1.65 0.55]); % x_controller % - this one
Q = 10*diag([132 18.5 1.05 0.2]); % x_controller % - this one
% Q = .65*diag([90 17.5 1.65 0.55]); % x_controller % Ts = 0.06
% Q = 1*diag([25 2 .1 .01]); % y_controller
% Q = 1*diag([25 2 .1 .01]); % y_controller
% Q = 10*diag([24 2.5 .1 .01]); % y_controller
% Q = 10*diag([71 7.5 1.65 0.55]); % y_controller % - this one
% Q = .65*diag([71 17.5 1.65 0.55]); % y_controller
% Q = diag([21 2]); % z_controller


R = .15; R = .165; % x_controller
% R = .2; % y_controller
%  R = .15; % y_controller
% R = .35; % z_controller


Y_ref = [0 0 0 0]'; % x, y
% Y_ref = [0 0]'; % z

Q_L = 1;
R_L = 10;
[sol_x_mp,ValueFunction_x,MP_SolutionOut] = MP_CL_MinMax_SDPrelax(add_usys_d, x_state, Y_ref, Y_x_Limit_orig,U_x_bounds,W_x_bounds,Q,R,N,Q_L,R_L,norm_type)

%  Simulate in time
%flag_sim = input('Show Simulation? [0/1]');
flag_sim = 1
if(flag_sim == 1)
    x_state_init = zeros(length(x_state),1); x_state_init = [1 0 0 0]'; %  x_state_init = [1 0]';
    time_sec = 10; % 10 secs
  %  time_sec = input('how long [s]?');
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
xx = [0.08151 0.089 0.0009 0]';
% xx = -2*[0.08151 0.089]';
assign(x_state,xx);
[U,flag_oob] =  RobustMPCInput(xx,ExplicitRobustMPC_obj.H,ExplicitRobustMPC_obj.K,ExplicitRobustMPC_obj.F,ExplicitRobustMPC_obj.G,ExplicitRobustMPC_obj.Nc,ExplicitRobustMPC_obj.Nx)

[u_ctrl,sol_found, flag_strange] = robustmpc_getInput(sol_x_mp, xx);
assign(x_state,xx);
double(Optimizer)

%%
ASLquad_RobustMPC.Xdir = ExplicitRobustMPC_obj;
ASLquad_RobustMPC.Ydir = ExplicitRobustMPC_obj
ASLquad_RobustMPC.Zdir = ExplicitRobustMPC_obj