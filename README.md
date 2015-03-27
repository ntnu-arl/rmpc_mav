# rmpc_mav

Robust Model Predictive Control for Micro Aerial Vehicles (MAVs)
----------------------------------------------------------------

This code implementation accompanies the paper submission entitled:

"Robust Model Predictive Flight  Control of Unmanned Rotorcrafts"

submitted to the Journal of Intelligent and Robotic Systems

Below, information on how to use the toolbox is provided. 

The Robust Model Predictive Control for MAVs
------------------------------------------------

The toolbox consists of a set of functions that lead to the computation of Robust Model Predictive Control laws verified to provide superior control performance for the case of a Quadrotor as well as a Tilt--Rotor MAVs. The underlying computations rely on [YALMIP](http://users.isy.liu.se/johanl/yalmip/). 

**Preliminary Resources**
The user should first install [YALMIP](http://users.isy.liu.se/johanl/yalmip/) following the relevant [Download link](http://users.isy.liu.se/johanl/yalmip/pmwiki.php?n=Main.Download). Subsequently, the following instructions copied from the [YALMIP Wiki](http://users.isy.liu.se/johanl/yalmip/pmwiki.php?n=Tutorials.Installation) should be followed:

    YALMIP is entirely based on m-code, and is thus easy to install. Remove any old version of YALMIP, unzip the file YALMIP.zip and add the following directories to your MATLAB path
    /yalmip
    /yalmip/extras
    /yalmip/demos
    /yalmip/solvers
    /yalmip/modules
    /yalmip/modules/parametric
    /yalmip/modules/moment
    /yalmip/modules/global
    /yalmip/modules/sos
    /yalmip/operators
    A lazy way to do this is addpath(genpath(yalmiprootdirectory))
    If you have MPT installed, make sure that you delete the YALMIP distribution residing inside MPT and remove the old path definitions.
    If you have used YALMIP before, type clear classes or restart MATLAB before using the new version.
    The solvers should be installed as described in the solver manuals. Make sure to add required paths.
    To test your installation, run the command yalmiptest. For further examples and tests, run code from this Wiki!
    If you have problems, please read the FAQ.
    YALMIP is primarily developed on a Windows machine using MATLAB 7.12 (2011a). The code should work on any platform, but is developed and thus most extensively tested on Windows. Most parts of YALMIP should in principle work with MATLAB 6.5, but has not been tested (to any larger extent) on these versions. MATLAB 5.2 or earlier versions are definitely not supported.

The user is also highly advised to install the **Multi-Parametric Toolbox**, which is available following this [link](http://people.ee.ethz.ch/~mpt/3/). 

Once both these toolboxes are installed, the user might want to use an external solver. For the purposes of our work we employed [Gurobi](http://www.gurobi.com/).

**Include the Repository in your MATLAB Path**
Subsequently, the user should include the rmpc_mav repository directories into the MATLAB path of the system. This can be done by being at the top directory and executing:

    addpath(genpath([pwd '/rmpc_mav']));

Once this is done, the functions to generate the robust model predictive control approaches may be executed. 

**Run the Examples**
The user can make direct use of the toolbox by running the existing examples - assuming that all underlying toolboxes are installed properly and an efficient solver is employed. The main example that can be used is the one found in *master/Verified_Examples/ASLquad/aslquad_robust_mpc.m*

For the convenience of the reader, the main m-code of this example is copied below:

    %%  ASLquad Robust MPC Control Design
    %
    %   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)
    
    run Init_RobustMPC; % be at the root directory
    
    %%  Load ASLquad Parameters
    Ts = 0.04;
    
    run example_ASLquad_model
    
    %%  Define Additive Uncertain Model
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
    
    N = 4; 
    
    Y_x_Limit_orig = [Inf 2 deg2rad(45) deg2rad(360)]; % x 
    
    U_x_bounds = [-deg2rad(30) deg2rad(30)]; % xy_controller
    
    Q = 10*diag([132 18.5 1.05 0.2]); 
    
    R = .165; 
    
    Y_ref = [0 0 0 0]'; % x, y
    
    Q_L = 1;
    R_L = 10;
    [sol_x_mp,ValueFunction_x,MP_SolutionOut] = MP_CL_MinMax_SDPrelax(add_usys_d, x_state, Y_ref, Y_x_Limit_orig,U_x_bounds,W_x_bounds,Q,R,N,Q_L,R_L,norm_type)
    
    %  Simulate in time
    flag_sim = 1
    if(flag_sim == 1)
	    x_state_init = zeros(length(x_state),1); x_state_init = [1 0 0 0]'; %  x_state_init = [1 0]';
	    time_sec = 10; % 10 secs
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
	assign(x_state,xx);
	[U,flag_oob] =  RobustMPCInput(xx,ExplicitRobustMPC_obj.H,ExplicitRobustMPC_obj.K,ExplicitRobustMPC_obj.F,ExplicitRobustMPC_obj.G,ExplicitRobustMPC_obj.Nc,ExplicitRobustMPC_obj.Nx)
	
	[u_ctrl,sol_found, flag_strange] = robustmpc_getInput(sol_x_mp, xx);
	assign(x_state,xx);
	double(Optimizer)
	
	%%	Write
	ASLquad_RobustMPC.Xdir = ExplicitRobustMPC_obj;
	ASLquad_RobustMPC.Ydir = ExplicitRobustMPC_obj

Note that the results of this code rely heavily on the specific set-up of solvers!

Similarly, one can run the Tilt-Rotor example:

    %%  Tri Tilt-Rotor Robust MPC Control Design
    %
    %   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)
    run Init_RobustMPC;
    
    %%  Load TTR Parameters
    Ts = 0.25;
    
    run example_ttr_model
    
    %%  Define Additive Uncertain Model
    E_x = [0 1 0]'; E_y = [0 1 0]';E_z = [0 .03 0]';
    
    add_usys_x_d = add_uss(A_x,B_x,C_x,D_x,E_x,Ts)
    add_usys_y_d = add_uss(A_y,B_y,C_y,D_y,E_y,Ts)
    add_usys_z_d = add_uss(A_z,B_z,C_z,D_z,E_z,Ts)
    
    add_usys_d = add_usys_y_d;
    
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
    norm_type = 1
    W_x_bounds = [-0.25 0.25];
    N = 2; 
    
    Y_x_Limit_orig = [Inf 1 deg2rad(45)]; % y 
    %Y_x_Limit = expand_Yconstraints(Y_x_Limit_orig,N)
    
    U_x_bounds = [-deg2rad(10) deg2rad(10)]; % x_controller
    U_x_bounds = [-deg2rad(15) deg2rad(15)]; % y_controller
    
    %   norm_type = 1;
    Q = .8*diag([16 1.4 .005]); % y_controller
    
    % %   norm_type = inf
    % Q = diag([22 1.85 .1]); % x_controller
    % % Q = diag([21 2.5 .1]); % y_controller
    % % Q = diag([38 9.8 1.05]); % z_controller
    
    R = 20*.15; 
    
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
    
    %%  Validate
    
    %   1. validator:
    sol_x_mp{1}.Ai = cell(1,length(sol_x_mp{1}.Ai));
    sol_x_mp{1}.Bi = sol_x_mp{1}.Fi;
    sol_x_mp{1}.Ci = sol_x_mp{1}.Gi;
    Optimizer = pwf(sol_x_mp{1},x_state,'general')
    
    %   derive the matrices for real-time execution
    [H,K,F,G,Nc] = GetRobustMPC_Matrices(sol_x_mp{1});
    Nx = length(add_usys_d.matrices.A);
    
    %   validate
    clc
    xx = [0.08151 0.089 0.0009]';
    assign(x_state,xx);
    [U,flag_oob] =  RobustMPCInput(xx,ExplicitRobustMPC_obj.H,ExplicitRobustMPC_obj.K,ExplicitRobustMPC_obj.F,ExplicitRobustMPC_obj.G,ExplicitRobustMPC_obj.Nc,ExplicitRobustMPC_obj.Nx)
    
    [u_ctrl,sol_found, flag_strange] = robustmpc_getInput(sol_x_mp, xx);
    assign(x_state,xx);
    double(Optimizer)
    
    %%  Write
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


Contact
-------
For any question or request don't hesitate to [contact me](mailto:konstantinos.alexis@gmail.com). 

Note
----
Upon successful evaluation, the code will be re-structured and released in combination with installation pipelines for a specific set-up with YALMIP, MPT and solver options. 
