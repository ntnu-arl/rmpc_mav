%clear all; clc;
display( 'TTR_mdl_Dynamics Initializing' );

%% Setup TTR_mdl directories

Current_dir = pwd;
addpath( Current_dir );

%% Setup TTR_mdl values
Ts = 0.010;
Ts_slow = TTR_RobustMPC.Xdir.Ts;
Ts_const = Inf;
    
m_B    = 2.350;  %2.066;    %1
g      = 9.81;              %2
Ixx    = 0.057748141;       %3
Iyy    = 0.025379458;       %4
Izz    = 0.074710979;       %5
Ixy    = 0.0;               %6
Ixz    = 0.0;               %7
Iyz    = 0.0;               %8
d_RL_x = 0.05461;           %9
d_T_x  = 0.395 - d_RL_x;    %10
d_RL_y = 0.25;              %11
d_T_y  = 0;                 %12
d_RL_z = 0.00281;           %13
d_T_z  = 0.01781;           %14
h_RL   = 0.099;             %15
h_T    = 0.0355;            %16
rho     = 1.225;        %17
w_chord = 0.200;        %18
w_span  = 1.000;        %19
w_ac_x  = 0.050;        %20
w_ac_y  = 0.290;        %21
TTR_mdl_Properties_arr = [ m_B; g; Ixx; Iyy; Izz; Ixy; Ixz; Iyz; d_RL_x; d_T_x; d_RL_y; d_T_y; d_RL_z; d_T_z; h_RL; h_T; rho; w_chord; w_span; w_ac_x; w_ac_y ];
TTR_mdl_Properties = cell2struct( num2cell(TTR_mdl_Properties_arr), {'m_B','g','Ixx','Iyy','Izz','Ixy','Ixz','Iyz','d_RL_x','d_T_x','d_RL_y','d_T_y','d_RL_z','d_T_z','h_RL','h_T','rho','w_chord','w_span','w_ac_x','w_ac_y'}, 1 );

%% Control Values

%d_RL_x(FL_0+FR_0)=d_T_x*FT_0  FR_0+FL_0+FT_0=m_b*g , FR_0=FL_0 
  F_0 = solve('d_RL_x*(FL_0+FR_0)=d_T_x*FT_0','FR_0+FL_0+FT_0=m_B*g','FR_0=FL_0','FR_0,FL_0,FT_0');
  F_0.FR_0 = eval(F_0.FR_0);
  F_0.FL_0 = eval(F_0.FL_0);
  F_0.FT_0 = eval(F_0.FT_0);

% Rotors [micros2rpm]
PolyR_R2T = [0.004088843035973 -8.629151839250008];  PolyR_U2R = 1.0e+03*[0.007572258890430   1.420497523706490];
PolyL_R2T = [0.003950480213225 -8.757691155123139];  PolyL_U2R = 1.0e+03*[0.007608780218259   1.472113028259252];
PolyT_R2T = [0.001340898677758  -3.488931935066077];  PolyT_U2R = 1.0e+03*[0.007882307894585   1.022799157396815];
PolyRL_U2R = (PolyR_U2R + PolyL_U2R)/2;
PolyRL_R2T = (PolyR_R2T + PolyL_R2T)/2;
PolyR_R2U = [1/PolyR_U2R(1) -PolyR_U2R(2)/PolyR_U2R(1)];
PolyL_R2U = [1/PolyL_U2R(1) -PolyL_U2R(2)/PolyL_U2R(1)];  
PolyRL_R2U = (PolyR_R2U + PolyL_R2U)/2;
PolyT_R2U = [1/PolyT_U2R(1) -PolyT_U2R(2)/PolyT_U2R(1)];

PolyL_U2T = [PolyL_R2T(1)*PolyL_U2R(1) PolyL_R2T(1)*PolyL_U2R(2)+PolyL_R2T(2)];
PolyR_U2T = [PolyR_R2T(1)*PolyR_U2R(1) PolyR_R2T(1)*PolyR_U2R(2)+PolyR_R2T(2)];
PolyT_U2T = [PolyT_R2T(1)*PolyT_U2R(1) PolyT_R2T(1)*PolyT_U2R(2)+PolyT_R2T(2)];
PolyT_T2U = [1/PolyT_U2T(1) -PolyT_U2T(2)/PolyT_U2T(1)];
PolyRL_U2T = (PolyR_U2T + PolyL_U2T)/2;
PolyRL_T2U = [1/PolyRL_U2T(1) -PolyRL_U2T(2)/PolyRL_U2T(1)];

Rotor_Mappings_arr = [ PolyRL_U2R; PolyRL_R2U; PolyT_U2R; PolyT_R2U ];
Rotor_Mappings = cell2struct( num2cell(Rotor_Mappings_arr), {'PolyRL_U2R','PolyRL_R2U','PolyT_U2R','PolyT_R2U'}, 1 );


% Servos [micros2rad]
PolyGT_U2A = [0.002181661564993 0.0];          PolyGT_A2U = [1/PolyGT_U2A(1) -PolyGT_U2A(2)/PolyGT_U2A(1)];

%% PolyX_R2T  the RPM->Thrust mapping
PolyR_R2T = [0.004088843035973 -8.629151839250008];  PolyR_U2R = 1.0e+03*[0.007572258890430   1.420497523706490];
PolyL_R2T = [0.003950480213225 -8.757691155123139];  PolyL_U2R = 1.0e+03*[0.007608780218259   1.472113028259252];
PolyR_T2R = [1/PolyR_R2T(1) -PolyR_R2T(2)/PolyR_R2T(1)];    PolyL_T2R = [1/PolyL_R2T(1) -PolyL_R2T(2)/PolyL_R2T(1)];  

PolyRL_R2T = (PolyR_R2T+PolyL_R2T)/2;
PolyRL_T2R = (PolyR_T2R+PolyL_T2R)/2;
rpmRL_trim = 4900;
omegaRL_trim = rpmRL_trim * (2*pi/60);

C_T_RL_trim = polyval(PolyRL_R2T,rpmRL_trim)/omegaRL_trim^2;
C_T_RL_prop = (PolyRL_R2T(1) * (60/(2*pi)))/(2*omegaRL_trim);


PolyT_R2T = [0.001340898677758  -3.488931935066077];  PolyT_U2R = 1.0e+03*[0.007882307894585   1.022799157396815];
PolyT_T2R = [1/PolyT_R2T(1) -PolyT_R2T(2)/PolyT_R2T(1)]; 
rpmT_trim = 5050;
omegaT_trim = rpmT_trim * (2*pi/60);

C_T_T_trim = polyval(PolyT_R2T,rpmT_trim)/omegaT_trim^2;
C_T_T_prop = (PolyT_R2T(1) * (60/(2*pi)))/(2*omegaT_trim);


Rotor_Properties_arr = [ C_T_RL_trim; C_T_RL_prop; C_T_T_trim; C_T_T_prop ];
Rotor_Properties = cell2struct( num2cell(Rotor_Properties_arr), {'C_T_RL_trim','C_T_RL_prop','C_T_T_trim','C_T_T_prop'}, 1 );
  
%% Rotor Values

%Only to keep Denominator, Delay
tf_r=tf(7.655946120461085,[0.145992771705270 1]);   tf_r.OutputDelay=0.03388;
tf_l=tf(7.531092760896522,[0.141638503909445 1]);   tf_l.OutputDelay=0.034055;
tf_t=tf(7.4439,[0.069265 1]);                       tf_t.OutputDelay=0.05;  
%%% Change dcgain to match manually mapped values %%%
tf_RL = tf(C_T_RL_prop*2*omegaRL_trim,(tf_r.den{1}+tf_l.den{1})/2);  tf_RL.OutputDelay = (tf_r.OutputDelay+tf_l.OutputDelay)/2;
tf_RL_num = tf_RL.num{1};
tf_RL_den = tf_RL.den{1};
tf_RL_delay = tf_RL.OutputDelay;
tf_RL_dcgain = dcgain(tf_RL);
tf_RL0 = tf(C_T_RL_trim,tf_RL_den);  tf_RL0.OutputDelay = tf_RL.OutputDelay;
tf_RL0_num = tf_RL0.num{1};
tf_RL0_den = tf_RL0.den{1};
tf_RL0_delay = tf_RL0.OutputDelay;
tf_RL0_dcgain = dcgain(tf_RL0);
tf_T = tf(C_T_T_prop*2*omegaT_trim,tf_t.den{1});  tf_T.OutputDelay = tf_t.OutputDelay;
tf_T_num = tf_T.num{1};
tf_T_den = tf_T.den{1};
tf_T_delay = tf_T.OutputDelay;
tf_T_dcgain = dcgain(tf_T);
tf_T0 = tf(C_T_T_trim,tf_t.den{1});  tf_T0.OutputDelay = tf_T.OutputDelay;
tf_T0_num = tf_T0.num{1};
tf_T0_den = tf_T0.den{1};
tf_T0_delay = tf_T0.OutputDelay;
tf_T0_dcgain = dcgain(tf_T0);

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% hr_p=pade(hr,1);  hr_ss = idss(hr,'SSParameterization','Canonical');  hr_pss = idss(hr_p,'SSParameterization','Canonical');
% hl_p=pade(hl,1);  hl_ss = idss(hl,'SSParameterization','Canonical');  hl_pss = idss(hl_p,'SSParameterization','Canonical');
% hr_d=c2d(hr,Ts,'tustin');   tf_m_right_num=hr_d.num{1};   tf_m_right_den=hr_d.den{1};   tf_m_right_delay=hr_d.ioDelay+hr_d.InputDelay+hr_d.OutputDelay;     tf_m_right_dcgain=dcgain(hr);
% hl_d=c2d(hl,Ts,'tustin');   tf_m_left_num =hl_d.num{1};   tf_m_left_den =hl_d.den{1};   tf_m_left_delay =hl_d.ioDelay+hl_d.InputDelay+hl_d.OutputDelay;     tf_m_left_dcgain=dcgain(hl);
tf_RL_p = pade(tf_RL,1);  tf_RL_ss = idss(tf_RL_p,'SSParameterization','Canonical');
tf_RL0_p = pade(tf_RL0,1);  tf_RL0_ss = idss(tf_RL0_p,'SSParameterization','Canonical');
tf_T_p = pade(tf_T,1);  tf_T_ss = idss(tf_T_p,'SSParameterization','Canonical');
tf_T0_p = pade(tf_T0,1);  tf_T0_ss = idss(tf_T0_p,'SSParameterization','Canonical');

% % Servos Right,Left:  100% @ 0.020[s]
% tf_S_ss = ss(-300,300,1,0);
% Servos Right,Left:  100% @ 0.020[s]
tf_S_ss = ss(-30,30,1,0);
[num_S,den_S] = ss2tf(tf_S_ss.A,tf_S_ss.B,tf_S_ss.C,tf_S_ss.D);
tf_S = tf(num_S,den_S);
tf_S_num = tf_S.num{1};
tf_S_den = tf_S.den{1};
tf_S_delay = tf_S.OutputDelay;
tf_S_dcgain = dcgain(tf_S);

%% Wing Aerodynamic Variables
a_vect = load('TTR_mdl_workspace.mat', 'a_vect');   a_vect = a_vect.a_vect;
v_vect = load('TTR_mdl_workspace.mat', 'v_vect');   v_vect = v_vect.v_vect;
cl     = load('TTR_mdl_workspace.mat', 'cl');       cl     = cl.cl;
cd     = load('TTR_mdl_workspace.mat', 'cd');       cd     = cd.cd;
Wing_Properties = struct( ...
                'a_vect' , a_vect ,...
                'v_vect' , v_vect ,...
                'cl' , cl ,...
                'cd' , cd );
            
%% Damping Values

damping_phi = 0.1;
damping_theta = 0.1;
damping_psi = 0.1;
damping_x = 0.5;
damping_y = 0.5;
damping_z = 0.5;
Damping_arr = [damping_phi; damping_theta; damping_psi; damping_x; damping_y; damping_z];
Damping = cell2struct( num2cell(Damping_arr), {'phi','theta','psi','x','y','z'}, 1 );

%% Drag Values

drag_max = 30;
v_max = 15;
drag_mid = drag_max/2;
v_mid = 2*v_max/3;
syms d_a d_b ;
sol = solve('drag_max=d_a*v_max^2+d_b*v_max','drag_mid=d_a*v_mid^2+d_b*v_mid','d_a','d_b');
d_a=subs(sol.d_a);
d_b=subs(sol.d_b);
Drag_arr = [d_a; d_b];
Drag = cell2struct( num2cell(Drag_arr), {'a','b'}, 1 );

%% MPT3
% X
%load('Xmpt3nopwaSy_dmp0.1.mat','ctrl');
%load('Xmpt3pwacplex_dmp0.1.mat','ctrl');
%load('Xmpt3pwaSy_dmp0.1.mat','ctrl');
%load('Xmpt3pwaSy_dmp1.0.mat','ctrl');
%load('Xmpt3pwagurobi_dmp0.1.mat','ctrl');
%load('Xmpt3pwaSygurobi_dmp0.1.mat','ctrl');
%[H_xz,K_xz,Hobj_xz,Fobj_xz,Gobj_xz,F_xz,G_xz,Nc_xz,Qopt_xz,Ropt_xz,Ml_xz,Mr_xz] = GetMPCMatrices_vP(ctrl);

% Y
%load('Ympt3Sygurobi_dmp0.1.mat','ctrl');
%[H_y,K_y,Hobj_y,Fobj_y,Gobj_y,F_y,G_y,Nc_y,Qopt_y,Ropt_y,Ml_y,Mr_y] = GetMPCMatrices_vP(ctrl);

%% Pass to Model Workspace
hws = get_param(bdroot, 'modelworkspace');
hws.DataSource = 'MAT-File';
hws.FileName = [Current_dir '\TTR_mdl_workspace.mat'];
hws.reload;  %!!! load values from already existent mat-file

assignin(hws, 'Ts', Ts);
assignin(hws, 'Ts_slow', Ts_slow);
assignin(hws, 'Ts_const', Ts_const);

assignin(hws, 'TTR_mdl_Properties', TTR_mdl_Properties);
assignin(hws, 'TTR_mdl_Properties_arr', TTR_mdl_Properties_arr);
assignin(hws, 'Rotor_Properties', Rotor_Properties);
assignin(hws, 'Rotor_Mappings', Rotor_Mappings);
assignin(hws, 'Wing_Properties', Wing_Properties);
assignin(hws, 'Damping', Damping);
assignin(hws, 'Drag', Drag);
assignin(hws, 'F_0', F_0);

%assignin(hws, 'tf_RL_num', tf_M_num);    assignin(hws, 'tf_RL_den', tf_RL_den);    assignin(hws, 'tf_RL_delay', tf_RL_delay);     assignin(hws, 'tf_RL_dcgain', tf_RL_dcgain);
%assignin(hws, 'tf_RL0_num', tf_M0_num);  assignin(hws, 'tf_RL0_den', tf_RL0_den);  assignin(hws, 'tf_RL0_delay', tf_RL0_delay);   assignin(hws, 'tf_RL0_dcgain', tf_RL0_dcgain);
assignin(hws, 'tf_S_num', tf_S_num);    assignin(hws, 'tf_S_den', tf_S_den);    assignin(hws, 'tf_S_delay', tf_S_delay);     assignin(hws, 'tf_S_dcgain', tf_S_dcgain);

assignin(hws,'tf_RL_ss',tf_RL_ss);  assignin(hws,'tf_RL0_ss',tf_RL0_ss);
assignin(hws,'tf_T_ss',tf_T_ss);    assignin(hws,'tf_T0_ss',tf_T0_ss);

assignin(hws, 'PolyRL_U2R', PolyRL_U2R);  assignin(hws, 'PolyT_U2R', PolyT_U2R);
assignin(hws, 'PolyGT_U2A', PolyGT_U2A);

assignin(hws, 'PolyRL_T2R', PolyRL_T2R);  assignin(hws, 'PolyT_T2R', PolyT_T2R);  assignin(hws, 'PolyRL_T2U', PolyRL_T2U);  assignin(hws, 'PolyT_T2U', PolyT_T2U);

% assignin(hws, 'H_xz', H_xz);  assignin(hws, 'K_xz', K_xz);  assignin(hws, 'Nc_xz', Nc_xz);
% assignin(hws, 'Hobj_xz', Hobj_xz);  assignin(hws, 'Fobj_xz', Fobj_xz);  assignin(hws, 'Gobj_xz', Gobj_xz);
% assignin(hws, 'F_xz', F_xz); assignin(hws, 'G_xz', G_xz);
% assignin(hws, 'Qopt_xz', Qopt_xz);  assignin(hws, 'Ropt_xz', Ropt_xz);
% assignin(hws, 'Ml_xz', Ml_xz);  assignin(hws, 'Mr_xz', Mr_xz);
% 
% assignin(hws, 'H_y', H_y);  assignin(hws, 'K_y', K_y);  assignin(hws, 'Nc_y', Nc_y);
% assignin(hws, 'Hobj_y', Hobj_y);  assignin(hws, 'Fobj_y', Fobj_y);  assignin(hws, 'Gobj_y', Gobj_y);
% assignin(hws, 'F_y', F_y); assignin(hws, 'G_y', G_y);
% assignin(hws, 'Qopt_y', Qopt_y);  assignin(hws, 'Ropt_y', Ropt_y);
% assignin(hws, 'Ml_y', Ml_y);  assignin(hws, 'Mr_y', Mr_y);

hws.saveToSource;
hws.reload;


