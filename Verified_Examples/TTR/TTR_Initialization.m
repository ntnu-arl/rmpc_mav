%clear all; clc;
display( 'TTR_Matlab Initializing' );

%% Setup TTR directories
if ispc
    Sep = '\';
    Home_dir = ['C:' getenv('HOMEPATH')];
end
if isunix
    Sep = '/';
    Home_dir = getenv('HOME');
end
Work_dir = [Home_dir Sep 'workspace' Sep 'Matlab'];
TTR_Peripheral_dir = [Home_dir Sep 'workspace' Sep 'TTR_Peripheral'];
%Data_dir = [Work_dir Sep 'Data'];
Scripts_dir = [Work_dir Sep 'Scripts'];
Src_dir = [Work_dir Sep 'src'];
Lib_dir = [Work_dir Sep 'src' Sep 'lib'];
Gui_dir = [Work_dir Sep 'Gui'];
Mdl_dir = [Work_dir Sep 'TTR_mdl'];
addpath( Work_dir );
addpath( Scripts_dir );
addpath( Src_dir );
addpath( Gui_dir );
addpath( Mdl_dir );

addpath( [Scripts_dir Sep 'Parametric_ID'] );
addpath( [Scripts_dir Sep 'HybridLibrary'] );

if ispc
tbxmanager restorepath
Cplex_dir = 'C:\Program Files\IBM\ILOG\CPLEX_Studio_Preview1251\cplex\matlab';
Gurobi_dir = 'C:\gurobi550\win64\matlab';
addpath( genpath(Cplex_dir) );
addpath( genpath(Gurobi_dir) );
mpt_init;
%mptopt('miqpsolver','yalmip','qpsolver','cplex');
%mptopt
%run yalmiptest;
end
if isunix
Cplex_dir = '/opt/ibm/ILOG/CPLEX_Studio_Preview1251/cplex/matlab';
addpath( genpath(Cplex_dir) );
mpt_dir = [Work_dir Sep 'mpt']
addpath( genpath( mpt_dir) );  %Subfolders too
mpt_init;
end

%% Misc Stuff
remote_checkpoint_control = 0;

%% Setup TTR values
Ts = 0.010;
Ts_slow = 0.250
Ts_const = Inf;

% !!! IF SOMTHING HERE IS CHANGED, UPDATE ALL BLOCKS USING TTR_Properties !!!
% i.e.: (TTR_Matlab/Controller/Calculate Control Actions)
%       (TTR_Matlab/Controller/Calculate State Reference/CTRL_Block/CTRL)
%       (TTR_Matlab/Controller/Calculate State Vector/form_state_vector)
m_B    = 2.350;  %2.066;    %1
g      = 9.81;              %2
Ixx    = 0.057748141;       %3
Iyy    = 0.025379458;       %4
Izz    = 0.074710979;       %5
Ixy    = 0.0;               %6
Ixz    = 0.0;               %7
Iyz    = 0.0;               %8
d_RL_x = 0.05461;  %0.025;  %9
d_T_x  = 0.395 - d_RL_x;    %10
d_RL_y = 0.25;              %11
d_T_y  = 0;                 %12
d_RL_z = 0.00281;           %13
d_T_z  = 0.01781;           %14
h_RL   = 0.099;             %15
h_T    = 0.0355;            %16
TTR_Properties = [ m_B; g; Ixx; Iyy; Izz; Ixy; Ixz; Iyz; d_RL_x; d_T_x; d_RL_y; d_T_y; d_RL_z; d_T_z; h_RL; h_T ];
%TTR_Properties_struct = cell2struct( num2cell(TTR_Properties), {'m_B','g','Ixx','Iyy','Izz','Ixy','Ixz','Iyz','d_RL_x','d_T_x','d_RL_y','d_T_y','d_RL_z','d_T_z','h_RL','h_T'}, 1 );

EKF_State_Transition_Matrix_A = ...  % X=[x_k;x_dot_k;x_die_k;y_k;y_dot_k;y_die_k;z_k;z_dot_k;z_die_k]
    [1  Ts 0  0  0  0  0  0  0;  ...
     0  1  Ts 0  0  0  0  0  0;  ...
     0  0  1  0  0  0  0  0  0;  ...
     0  0  0  1  Ts 0  0  0  0;  ...
     0  0  0  0  1  Ts 0  0  0;  ...
     0  0  0  0  0  1  0  0  0;  ...
     0  0  0  0  0  0  1  Ts 0;  ...
     0  0  0  0  0  0  0  1  Ts; ...
     0  0  0  0  0  0  0  0  1];
EKF_Process_Noise_Covariance_Q = ...  % 0.05*eye(9)
    0.005*eye(9);
EKF_Measurement_Update_Matrix_H = ...  % Z=[OF_X_dot(x)_k;ACC_X_die(x)_k;OF_X_dot(y)_k;ACC_X_die(y)_k;sonar_k;ACC_X_die(z)_k]  
    [0 1 0 0 0 0 0 0 0; ...
     0 0 1 0 0 0 0 0 0; ...
     0 0 0 0 1 0 0 0 0; ...
     0 0 0 0 0 1 0 0 0; ...
     0 0 0 0 0 0 1 0 0; ...
     0 0 0 0 0 0 0 0 1];
OF_X_dot_COVFACT = 1;
ACC_X_die_COVFACT = 0.5;  %Accelerometer values input to the EKF have been prefiltered (Lowpass), so we trust them more
sonar_COVFACT = 0.05;  %Sonar values input to the EKF have been prefiltered (Threshold + Median), so we trust them more
EKF_Measurement_Noise_Covariance_R = ...  % 1*eye(6)
    [OF_X_dot_COVFACT 0                 0                0                 0             0; ...
     0                ACC_X_die_COVFACT 0                0                 0             0; ...
     0                0                 OF_X_dot_COVFACT 0                 0             0; ...
     0                0                 0                ACC_X_die_COVFACT 0             0; ...
     0                0                 0                0                 sonar_COVFACT 0; ...
     0                0                 0                0                 0             ACC_X_die_COVFACT];
 
%PolyX_U2R  the Microsecs->RPM mapping
%PolyX_R2T  the RPM->Thrust mapping
%PolyX_U2T  the Microsecs->Thrust mapping  A2*(A1*x+B1)+B2
PolyR_R2T = [0.004088843035973 -8.629151839250008];  PolyR_U2R = 1.0e+03*[0.007572258890430   1.420497523706490];
PolyL_R2T = [0.003950480213225 -8.757691155123139];  PolyL_U2R = 1.0e+03*[0.007608780218259   1.472113028259252];
%Maytech 40A ESC w/ SimonK Firmware & Roxxy 2827-34 w/ MasterAirscrew 3-blade 9x6
PolyT_R2T = [0.001340898677758  -3.488931935066077];  PolyT_U2R = 1.0e+03*[0.007882307894585   1.022799157396815];
%Maytech 40A ESC w/ SimonK Firmware & HobbyKing Ducted Fan
%PolyT_R2T = [0.000301635975655  -3.923983879636754];  PolyT_U2R = 1.0e+04*[0.003125153523400   1.128894773084204];
%Mystery 40A ESC & Diamond Models Ducted Fan
%PolyT_R2T = [0.000259396779347 -3.882722200597666];  PolyT_U2R = 1.0e+04*[0.003502997607320 1.495161281040277];

%PolyR_R2T = [0.004088843035973 -8.629151839250008];  PolyR_U2R = 1.0e+03*[0.003457757643128 1.711071443895406];
%PolyL_R2T = [0.003664718548006 -7.819140690232608];  PolyL_U2R = 1.0e+03*[0.003451601253829 1.725420416114207];
%PolyT_R2T = [0.000259396779347 -3.882722200597666];  PolyT_U2R = 1.0e+04*[0.003502997607320 1.495161281040277];
PolyL_U2T = [PolyL_R2T(1)*PolyL_U2R(1) PolyL_R2T(1)*PolyL_U2R(2)+PolyL_R2T(2)];
PolyR_U2T = [PolyR_R2T(1)*PolyR_U2R(1) PolyR_R2T(1)*PolyR_U2R(2)+PolyR_R2T(2)];
PolyT_U2T = [PolyT_R2T(1)*PolyT_U2R(1) PolyT_R2T(1)*PolyT_U2R(2)+PolyT_R2T(2)];
%d_RL_y(dFL-dFR)=0  dFX=PolyX_U2T(1)*dUX
%d_RL_y*PolyR_U2T(1)*UR*R = d_RL_y*PolyL_U2T(1)*UL*L  UR=UL
  control_analogies_roll = solve('PolyR_U2T(1)*R = PolyL_U2T(1)*L','R=1', 'R,L');
  control_analogies_roll.R = eval( control_analogies_roll.R );
  control_analogies_roll.L = eval( control_analogies_roll.L );
if control_analogies_roll.L < 1
  control_analogies_roll = solve('PolyR_U2T(1)*R = PolyL_U2T(1)*L','L=1', 'R,L');
  control_analogies_roll.R = eval( control_analogies_roll.R );
  control_analogies_roll.L = eval( control_analogies_roll.L );
end
Control_Analogies_Roll = [ control_analogies_roll.R; control_analogies_roll.L ];
%d_RL_x(dFL+dFR)=d_T_x*dFT  dFX=PolyX_U2T(1)*dUX
%d_RL_x*(PolyR_U2T(1)*UR*R + PolyL_U2T(1)*UL*L)*RL = d_T_x*PolyT_U2T(1)*UT*T  UR=UL=UT
  control_analogies_pitch = solve('d_RL_x*(PolyR_U2T(1)*Control_Analogies_Roll(1) + PolyL_U2T(1)*Control_Analogies_Roll(2))*RL = d_T_x*PolyT_U2T(1)*T', 'T=1', 'RL,T');
  control_analogies_pitch.RL = eval( control_analogies_pitch.RL );
  control_analogies_pitch.T = eval( control_analogies_pitch.T );
if control_analogies_pitch.RL < 1
  control_analogies_pitch = solve('d_RL_x*(PolyR_U2T(1)*Control_Analogies_Roll(1) + PolyL_U2T(1)*Control_Analogies_Roll(2))*RL = d_T_x*PolyT_U2T(1)*T', 'RL=1', 'RL,T');
  control_analogies_pitch.RL = eval( control_analogies_pitch.RL );
  control_analogies_pitch.T = eval( control_analogies_pitch.T );
end
Control_Analogies_Pitch = [ control_analogies_pitch.RL; control_analogies_pitch.T ];
Control_Analogies = [ control_analogies_roll.R; control_analogies_roll.L; control_analogies_pitch.RL; control_analogies_pitch.T ];
%Control_Analogies_struct = cell2struct( num2cell(Control_Analogies), {'roll_R','roll_L','pitch_RL','pitch_T'}, 1);
PolyR_R2U = [1/PolyR_U2R(1) -PolyR_U2R(2)/PolyR_U2R(1)];  PolyL_R2U = [1/PolyL_U2R(1) -PolyL_U2R(2)/PolyL_U2R(1)];  PolyT_R2U = [1/PolyT_U2R(1) -PolyT_U2R(2)/PolyT_U2R(1)];
PolyR_T2R = [1/PolyR_R2T(1) -PolyR_R2T(2)/PolyR_R2T(1)];  PolyL_T2R = [1/PolyL_R2T(1) -PolyL_R2T(2)/PolyL_R2T(1)];  PolyT_T2R = [1/PolyT_R2T(1) -PolyT_R2T(2)/PolyT_R2T(1)];
PolyR_T2U = [1/PolyR_U2T(1) -PolyR_U2T(2)/PolyR_U2T(1)];  PolyL_T2U = [1/PolyL_U2T(1) -PolyL_U2T(2)/PolyL_U2T(1)];  PolyT_T2U = [1/PolyT_U2T(1) -PolyT_U2T(2)/PolyT_U2T(1)];

% (See Identification/Identification_FindVal.m)
%PolyGR_U2A = [-0.0007680123884501160 0.0];     PolyGR_A2U = [1/PolyGR_U2A(1) -PolyGR_U2A(2)/PolyGR_U2A(1)];
%PolyGL_U2A = [0.001027553152689 0.0];          PolyGL_A2U = [1/PolyGL_U2A(1) -PolyGL_U2A(2)/PolyGL_U2A(1)];
Servo_T_x0 = round( 1405 );
PolyGT_U2A = [0.002181661564993 0.0];          PolyGT_A2U = [1/PolyGT_U2A(1) -PolyGT_U2A(2)/PolyGT_U2A(1)];
% Create Fake LookUp for Tail Servo
%Servo_T_LUx = [-500 Servo_T_x0 500];  Servo_T_LUx = Servo_T_LUx - Servo_T_x0;  %Zero-Offset
%Servo_T_LUy = polyval(PolyGT_U2A,Servo_T_LUx);
Servo_T_LUx = Servo_T_x0 + [round( -500 ) 0 round( 500 )];  %Full-Scale
Servo_T_LUy = polyval(PolyGT_U2A,Servo_T_LUx - Servo_T_x0);
% Laser-based mapping Right
%Servo_R_x0 = 1540;  Servo_R_d0 = 50.0;  Servo_R_vert = 74.0;
%Servo_R_d = [75.1 71.1 67.6 64.0 60.3 56.5 53.7 51.0 50.0 49.4 47.5 43.7 41.2 38.4 35.0 31.9 28.6];  Servo_R_d = Servo_R_d - Servo_R_d0;
%%Servo_R_LUx = [1940 1890 1840 1790 1740 1690 1640 1590 1540 1490 1440 1390 1340 1290 1240 1190 1140];  Servo_R_LUx = Servo_R_LUx - Servo_R_x0;  %Zero-Offset
%Servo_R_LUx = [1940 1890 1840 1790 1740 1690 1640 1590 1540 1490 1440 1390 1340 1290 1240 1190 1140];  %Full-Scale
% SIGNUM-FIX (+u) => (+gamma)
%Servo_R_LUy = -sin( Servo_R_d./Servo_R_vert );
% Laser-based mapping Left
%Servo_L_x0 = round( 1580 );  Servo_L_d0 = 50.0;  Servo_L_vert = 74.0;
%Servo_L_d = [25.6 29.7 33.8 37.5 40.5 44.2 47.0 49.5 50.0 50.6 53.0 55.4 58.5 62.8 66.6 70.4 74.4];  Servo_L_d = Servo_L_d - Servo_L_d0;
%%Servo_L_LUx = [1980 1930 1880 1830 1780 1730 1680 1630 1580 1530 1480 1430 1380 1330 1280 1230 1180];  Servo_L_LUx = Servo_L_LUx - Servo_L_x0;  %Zero-Offset
%Servo_L_LUx = [1980 1930 1880 1830 1780 1730 1680 1630 1580 1530 1480 1430 1380 1330 1280 1230 1180];  %Full-Scale
% SIGNUM-FIX (+u) => (-gamma)
% Servo_L_LUy = -sin( Servo_L_d./Servo_L_vert );
load('Servo_LU.mat');
% Accelerometer-based mapping Right
Servo_R_FixRISE = deg2rad(1.5);  
Servo_R_FixFALL = deg2rad(1.5);
% !!! (LUx_RISE,LUyRISE),(LUx_FALL,LUyFALL) will now cross, SHOULD FIX THAT !!!
Servo_R_LUxRISE; Servo_R_LUyRISE + Servo_R_FixRISE;
Servo_R_LUxFALL; Servo_R_LUyFALL - Servo_R_FixFALL;
Servo_R_x0 = round( 0.5*(interp1(Servo_R_LUyRISE,Servo_R_LUxRISE,0)+interp1(Servo_R_LUyFALL,Servo_R_LUxFALL,0)) );
Servo_R_LUx = [600:1:2400];
Servo_R_LUy = 0.5*(interp1(Servo_R_LUxRISE,Servo_R_LUyRISE,Servo_R_LUx)+interp1(Servo_R_LUxFALL,Servo_R_LUyFALL,Servo_R_LUx));
% Accelerometer-based mapping Left
Servo_L_FixRISE = deg2rad(1.5);  
Servo_L_FixFALL = deg2rad(1.5);
% !!! (LUx_RISE,LUyRISE),(LUx_FALL,LUyFALL) will now cross, SHOULD FIX THAT !!!
Servo_L_LUxRISE; Servo_L_LUyRISE + Servo_L_FixRISE;
Servo_L_LUxFALL; Servo_L_LUyFALL - Servo_L_FixFALL;
Servo_L_x0 = round( 0.5*(interp1(Servo_L_LUyRISE,Servo_L_LUxRISE,0)+interp1(Servo_L_LUyFALL,Servo_L_LUxFALL,0)) );
Servo_L_LUx = [600:1:2400];
Servo_L_LUy = 0.5*(interp1(Servo_L_LUxRISE,Servo_L_LUyRISE,Servo_L_LUx)+interp1(Servo_L_LUxFALL,Servo_L_LUyFALL,Servo_L_LUx));

%Ad-Hoc change Zero if Long-Time Wear has drifted it
%Servo_L_x0 = Servo_L_x0 -10;

%%
% Gain Scheduling : LUx:=error , LUy:=Gain
Kp_roll_LUx = deg2rad([0 5 10]);  Kp_roll_LUy = [130 120 110];
Kp_pitch_LUx = deg2rad([0 5 10]);  Kp_pitch_LUy = [140 120 110];
%Kp_roll_LUx = deg2rad([0 5 7.5 10]);  Kp_roll_LUy = [150 120 60 40];  % From Step Experiments
%Kp_roll_LUx = deg2rad([0 5 7.5 10]);  Kp_roll_LUy = [200 180 120 100];  %Better Tracking in Chirp
%Kp_pitch_LUx = deg2rad([0 5 10]);  Kp_pitch_LUy = [130 110 80];  % From Step Experiments

%roll_km1:  [130 110 60 40]
%pitch_km1: [130 110 80]

% Integrator Leakage Factors
Int_Leakage_X = 1.0;
Int_Leakage_Theta = 3.0;

%%
% !!! Offset of 1000 is added at the end 
%MK_ESC_absolute_constraints = [0 2047];
%MK_ESC_constraints = [0 2047];                  MK_ESC_slewrate_constraint = 0.1/Ts;
PWM_ESC_absolute_constraints = [1000 2000];
PWM_ESC_constraints = [0 1000];                 PWM_ESC_slewrate_constraint = 0.1/Ts;
PWM_SERVO_absolute_constraints = [600 2400];
PWM_SERVO_constraints = [600 2400];             PWM_SERVO_slewrate_constraint = Inf;  %no constraint

Sonar_Upper_ValueThreshold_constraint  = 6.00;  %[m]
Sonar_Lower_ValueThreshold_constraint  = 0.05;  %[m] Lower possible sonar value so we can easily reject out-of-place values lower than this
Sonar_Integrator_constraint = 0.20;  %[m*s]
Sonar_Derivative_constraint = 1.0;   %[m/s]

Roll_Error_constraint = Inf;
Pitch_Error_constraint = Inf;
Yaw_Error_constraint = deg2rad( 20 );  %[rad] (+-20deg total40deg)
ThetaError_constraints = [Roll_Error_constraint; Pitch_Error_constraint; Yaw_Error_constraint];

Roll_ControlAction_constraint = Inf;
Pitch_ControlAction_constraint = Inf;
Yaw_ControlAction_constraint = Inf;
ThetaControlAction_constraints = [Roll_ControlAction_constraint; Pitch_ControlAction_constraint; Yaw_ControlAction_constraint];
Roll_ControlAction_slewconstraint = Inf;
Pitch_ControlAction_slewconstraint = Inf;
Yaw_ControlAction_slewconstraint = Inf;
ThetaControlAction_slewconstraints = [Roll_ControlAction_slewconstraint; Pitch_ControlAction_slewconstraint; Yaw_ControlAction_slewconstraint];


RPM_Right_RateThreshold_constraint = 1500;  % 1500 [rpm/Ts]
RPM_Left_RateThreshold_constraint = 1500;
RPM_Tail_RateThreshold_constraint = 1500;
RPM_RateThreshold_constraint = [RPM_Right_RateThreshold_constraint; RPM_Left_RateThreshold_constraint; RPM_Tail_RateThreshold_constraint];

RPM_Right_ModelErrors_constraint = 100;  %[rpm]
RPM_Left_ModelErrors_constraint = 100;
RPM_Tail_ModelErrors_constraint = 1000;
RPM_Right_ModelErrors_Integrator_constraint = 30 * 4 * PolyR_U2R(1);  %[rpm*s]  (control_action_constraint * settling_time: 50 * 4 -> translate to rpm)
RPM_Left_ModelErrors_Integrator_constraint  = 30 * 4 * PolyL_U2R(1);
RPM_Tail_ModelErrors_Integrator_constraint  = 12 * 3.5 * PolyT_U2R(1);

hr=tf(7.655946120461085,[0.145992771705270 1]);   hr.OutputDelay=0.03388;
hl=tf(7.531092760896522,[0.141638503909445 1]);   hl.OutputDelay=0.034055;
%hr=tf(4.4375,[0.1867 1]);   hr.OutputDelay=0.03453;   hr_p=pade(hr,1);  
%hl=tf(4.4007,[0.1925 1]);   hl.OutputDelay=0.03465;   hl_p=pade(hl,1); 
%Maytech 40A ESC w/ SimonK Firmware & Roxxy 2827-34 w/ MasterAirscrew 3-blade 9x6
ht=tf(7.4439,[0.069265 1]);   ht.OutputDelay=0.05;  
%Maytech 40A ESC w/ SimonK Firmware & HobbyKing Ducted Fan
%ht=tf(32.321,[0.063859 1]);   ht.OutputDelay=0.04;  
%Mystery 40A ESC & Diamond Models Ducted Fan
%ht=tf(42.873,[0.1619 1]);   ht.OutputDelay=0.0905;  
%%% Change dcgain to match manually mapped values %%%
hr.num{1} = [0 PolyR_U2R(1)];   hl.num{1} = [0 PolyL_U2R(1)];   ht.num{1} = [0 PolyT_U2R(1)];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
hr_p=pade(hr,1);  hr_ss = idss(hr,'SSParameterization','Canonical');  hr_pss = idss(hr_p,'SSParameterization','Canonical');
hl_p=pade(hl,1);  hl_ss = idss(hl,'SSParameterization','Canonical');  hl_pss = idss(hl_p,'SSParameterization','Canonical');
ht_p=pade(ht,1);  ht_ss = idss(ht,'SSParameterization','Canonical');  ht_pss = idss(ht_p,'SSParameterization','Canonical');
hr_d=c2d(hr,Ts,'tustin');   tf_m_right_num=hr_d.num{1};   tf_m_right_den=hr_d.den{1};   tf_m_right_delay=hr_d.ioDelay+hr_d.InputDelay+hr_d.OutputDelay;     tf_m_right_dcgain=dcgain(hr);
hl_d=c2d(hl,Ts,'tustin');   tf_m_left_num =hl_d.num{1};   tf_m_left_den =hl_d.den{1};   tf_m_left_delay =hl_d.ioDelay+hl_d.InputDelay+hl_d.OutputDelay;     tf_m_left_dcgain=dcgain(hl);
ht_d=c2d(ht,Ts,'tustin');   tf_m_tail_num =ht_d.num{1};   tf_m_tail_den =ht_d.den{1};   tf_m_tail_delay =ht_d.ioDelay+ht_d.InputDelay+ht_d.OutputDelay;     tf_m_tail_dcgain=dcgain(ht);

% Servos Right,Left:  100% @ 0.020[s]
[num_s_right_left,den_s_right_left] = ss2tf(-300,300,1,0);
tf_s_right_left = tf(num_s_right_left,den_s_right_left);
tf_s_right_left_d=c2d(tf_s_right_left,Ts,'tustin');   tf_s_right_left_num=tf_s_right_left_d.num{1};   tf_s_right_left_den=tf_s_right_left_d.den{1};   tf_s_right_left_delay=tf_s_right_left_d.ioDelay+tf_s_right_left_d.InputDelay+tf_s_right_left_d.OutputDelay;     tf_s_right_left_dcgain=dcgain(tf_s_right_left_d);
% Servo Tail:  100% @ 0.02[s]
[num_s_tail,den_s_tail] = ss2tf(-300,300,1,0);
tf_s_tail = tf(num_s_tail,den_s_tail);
tf_s_tail_d=c2d(tf_s_tail,Ts,'tustin');   tf_s_tail_num=tf_s_tail_d.num{1};   tf_s_tail_den=tf_s_tail_d.den{1};   tf_s_tail_delay=tf_s_tail_d.ioDelay+tf_s_tail_d.InputDelay+tf_s_tail_d.OutputDelay;     tf_s_tail_dcgain=dcgain(tf_s_tail_d);


%%
load('TTR_ss_BAK.mat', 'PHI_CL_v3');
tf_phi_cl = tf( PHI_CL_v3 );
%DCgain Ad-Hoc Fix
%tf_phi_cl = tf_phi_cl * 1.2;
tf_phi_cl_d=c2d(tf_phi_cl,Ts,'tustin');  tf_phi_cl_d_num=tf_phi_cl_d.num{1};   tf_phi_cl_d_den=tf_phi_cl_d.den{1};   tf_phi_cl_d_delay=tf_phi_cl_d.ioDelay+tf_phi_cl_d.InputDelay+tf_phi_cl_d.OutputDelay;     tf_phi_cl_d_dcgain=dcgain(tf_phi_cl_d);

%% OF-related
OF_calibration_width = 640;  % 640x480@calibration_time
OF_runtime_width = 320;  % 320x240@run_time
K_OFcam_ratio = OF_runtime_width/OF_calibration_width;
Fu = 1100.145811;  Fv = 1094.746674;  
%ou = 247.361949;  ov = 288.479584;
ou = 0;  ov = 0;  
K_OFcam = [Fu*K_OFcam_ratio  0                 ou*K_OFcam_ratio;...
           0                -Fv*K_OFcam_ratio  ov*K_OFcam_ratio;...
           0                 0                 1]

%% Ad-Hoc Parameters/Changes

%Control_Analogies are: [roll_R; roll_L; pitch_RL; pitch_T]
a_roll = -5;
%Control_Analogies(1) = 1 + 0.01*a_roll;  Control_Analogies(2) = 1 - 0.01*a_roll;

%Control Integrators (!!! Expressed @ ControlFunction loop values !!!)
roll_control_int_0 = 0;  %[us]
pitch_control_int_0 = -8;  %[us]
yaw_control_int_0 = deg2rad( 3 );  %[deg]
x_control_int_0 = deg2rad( 0 );  %[rad]
y_control_int_0 = deg2rad( 1.25 );  %[rad]
z_control_int_0 = 0;  %[N]
Control_Int_0 = [x_control_int_0;y_control_int_0;z_control_int_0;roll_control_int_0;pitch_control_int_0;yaw_control_int_0];

%Damping
damping_phi = 0.1;
damping_theta = 0.1;
damping_psi = 0.1;
damping_x = 0.1;
damping_y = 0.1;
damping_z = 0.1;
Damping = [damping_phi; damping_theta; damping_psi; damping_x; damping_y; damping_z];
%Damping_struct = cell2struct( num2cell(Damping), {'phi','theta','psi','x','y','z'}, 1 );

%% MPC Stuff %%
% % MPC State Estimator
% load('TTR_ss_BAK.mat', 'MPC_EXYPHIZS');
% MPC_Obj = MPC_EXYPHIZS;
% 
% %set(MPC_Obj,'controlhorizon',1);
% 
% % Check: eigs(estim_Cm - estim_A*estim_M)
% [MPC_numst,MPC_numin] = size(MPC_Obj.Model.Plant);
% [estim_M,estim_A,estim_Cm] = getestim(MPC_Obj);
% 
% estim_M(1:MPC_numst,1:MPC_numst) = 1.1*eye(MPC_numst);  %Over-Correct?
% setestim(MPC_Obj,estim_M);
% 
% % estim_M_FIX = 0*estim_M;
% % estim_M_FIX(1:7,1:7) = 1.1*eye(7);      %Over-Correct
% % estim_M_FIX(9:11,8:10) = 1.1*eye(3);    %Over-Correct
% % estim_M_FIX(8,6:7) = estim_M(8,6:7);  %Keep only terms relying on phi,y_dot
% % setestim(MPC_Obj,estim_M_FIX);
% 
% % Debug
% estim_M = getestim(MPC_Obj)

%% MPT3
% X
%load('Xmpt3pwaSygurobi_dmp0.1.mat','ctrl');
%[H_xz,K_xz,Hobj_xz,Fobj_xz,Gobj_xz,F_xz,G_xz,Nc_xz,Qopt_xz,Ropt_xz,Ml_xz,Mr_xz] = GetMPCMatrices_vP(ctrl);
load('Xmpt3pwagurobi_dmp1.0.mat','H_xz','K_xz','Hobj_xz','Fobj_xz','Gobj_xz','F_xz','G_xz','Nc_xz','Qopt_xz','Ropt_xz','Ml_xz','Mr_xz');

% Y
%load('Ympt3Sygurobi_dmp0.1.mat','ctrl');
%[H_y,K_y,Hobj_y,Fobj_y,Gobj_y,F_y,G_y,Nc_y,Qopt_y,Ropt_y,Ml_y,Mr_y] = GetMPCMatrices_vP(ctrl);
load('Ympt3Sygurobi_dmp1.0.mat','H_y','K_y','Hobj_y','Fobj_y','Gobj_y','F_y','G_y','Nc_y','Qopt_y','Ropt_y','Ml_y','Mr_y');

%% Trajectory
% Cuboid
X_Profile = [0 0 0 1 1 1 0];
Y_Profile = [0 1 1 1 0 0 0];
Z_Profile = [0 0 0.5 0.5 0.5 0 0];
Psi_Profile = [0 0 0 0 0 0 0];
T_Profile = cumsum(7.5*ones(1,length(X_Profile)));  T_Profile = T_Profile - T_Profile(1);

% %Forward
% X_Profile = [0 2];
% Y_Profile = [0 0];
% Z_Profile = [0 0];
% Psi_Profile = [0 0];
% T_Profile = cumsum(10*ones(1,length(X_Profile)));  T_Profile = T_Profile - T_Profile(1);

% % Circle
% thetacirc = [0:deg2rad(10):2*pi];
% rcirc = 1;
% xcirc = -rcirc*cos(thetacirc) + rcirc; 
% ycirc = rcirc*sin(thetacirc);
% %figure; plot(xcirc); hold on; plot(ycirc,'g'); hold off;  figure; plot(xcirc,ycirc); hold on; plot(xcirc(1),ycirc(1),'g*'); plot(xcirc(idx(pi/2,thetacirc)),ycirc(idx(pi/2,thetacirc)),'r*'); axis equal; hold off;
% X_Profile = xcirc;
% Y_Profile = ycirc;
% Z_Profile = zeros(1,length(X_Profile));
% Psi_Profile = thetacirc;
% T_Profile = cumsum(1*ones(1,length(X_Profile)));  T_Profile = T_Profile - T_Profile(1);

%% Pass to Model Workspace
hws = get_param(bdroot, 'modelworkspace');
hws.DataSource = 'MAT-File';
hws.FileName = [Work_dir '/Data/TTR_Matlab_workspace.mat'];
hws.reload;  %!!! load values from already existent mat-file

assignin(hws, 'Home_dir', Home_dir);
assignin(hws, 'Work_dir', Work_dir);
assignin(hws, 'TTR_Peripheral_dir', TTR_Peripheral_dir);
assignin(hws, 'Scripts_dir', Scripts_dir);
assignin(hws, 'Src_dir', Src_dir);
assignin(hws, 'Lib_dir', Lib_dir);

assignin(hws, 'remote_checkpoint_control', remote_checkpoint_control);

assignin(hws, 'Ts', Ts);
assignin(hws, 'Ts_slow', Ts_slow);
assignin(hws, 'Ts_const', Ts_const);

assignin(hws, 'TTR_Properties', TTR_Properties);
assignin(hws, 'Damping', Damping);

assignin(hws, 'EKF_State_Transition_Matrix_A', EKF_State_Transition_Matrix_A);
assignin(hws, 'EKF_Process_Noise_Covariance_Q', EKF_Process_Noise_Covariance_Q);
assignin(hws, 'EKF_Measurement_Update_Matrix_H', EKF_Measurement_Update_Matrix_H);
assignin(hws, 'EKF_Measurement_Noise_Covariance_R', EKF_Measurement_Noise_Covariance_R);

assignin(hws, 'PolyR_U2R', PolyR_U2R);  assignin(hws, 'PolyL_U2R', PolyL_U2R);  assignin(hws, 'PolyT_U2R', PolyT_U2R);
assignin(hws, 'PolyR_R2T', PolyR_R2T);  assignin(hws, 'PolyL_R2T', PolyL_R2T);  assignin(hws, 'PolyT_R2T', PolyT_R2T);
assignin(hws, 'PolyR_U2T', PolyR_U2T);  assignin(hws, 'PolyL_U2T', PolyL_U2T);  assignin(hws, 'PolyT_U2T', PolyT_U2T);
assignin(hws, 'PolyR_R2U', PolyR_R2U);  assignin(hws, 'PolyL_R2U', PolyL_R2U);  assignin(hws, 'PolyT_R2U', PolyT_R2U);
assignin(hws, 'PolyR_T2U', PolyR_T2U);  assignin(hws, 'PolyL_T2U', PolyL_T2U);  assignin(hws, 'PolyT_T2U', PolyT_T2U);

%assignin(hws, 'PolyGR_U2A', PolyGR_U2A);  assignin(hws, 'PolyGR_A2U', PolyGR_A2U);
%assignin(hws, 'PolyGL_A2U', PolyGL_A2U);  assignin(hws, 'PolyGL_U2A', PolyGL_U2A);
%assignin(hws, 'PolyGT_U2A', PolyGT_U2A);  assignin(hws, 'PolyGT_A2U', PolyGT_A2U);  assignin(hws,'Servo_T_x0', Servo_T_x0);
%assignin(hws, 'Servo_R_LUx', Servo_R_LUx);  assignin(hws, 'Servo_R_LUy', Servo_R_LUy);  assignin(hws,'Servo_R_x0', Servo_R_x0);
%assignin(hws, 'Servo_L_LUx', Servo_L_LUx);  assignin(hws, 'Servo_L_LUy', Servo_L_LUy);  assignin(hws,'Servo_L_x0', Servo_L_x0);
assignin(hws, 'Servo_R_LUxRISE', Servo_R_LUxRISE);  assignin(hws, 'Servo_R_LUyRISE', Servo_R_LUyRISE);  assignin(hws, 'Servo_R_LUxFALL', Servo_R_LUxFALL);  assignin(hws, 'Servo_R_LUyFALL', Servo_R_LUyFALL);  assignin(hws,'Servo_R_x0', Servo_R_x0);  assignin(hws, 'Servo_R_LUx', Servo_R_LUx);  assignin(hws, 'Servo_R_LUy', Servo_R_LUy);
assignin(hws, 'Servo_L_LUxRISE', Servo_L_LUxRISE);  assignin(hws, 'Servo_L_LUyRISE', Servo_L_LUyRISE);  assignin(hws, 'Servo_L_LUxFALL', Servo_L_LUxFALL);  assignin(hws, 'Servo_L_LUyFALL', Servo_L_LUyFALL);  assignin(hws,'Servo_L_x0', Servo_L_x0);  assignin(hws, 'Servo_L_LUx', Servo_L_LUx);  assignin(hws, 'Servo_L_LUy', Servo_L_LUy);
assignin(hws, 'Servo_T_LUx', Servo_T_LUx);  assignin(hws, 'Servo_T_LUy', Servo_T_LUy);  assignin(hws,'Servo_T_x0', Servo_T_x0);

assignin(hws, 'Int_Leakage_X', Int_Leakage_X);  assignin(hws, 'Int_Leakage_X', Int_Leakage_X);  assignin(hws,'Int_Leakage_X', Int_Leakage_X);
assignin(hws, 'Int_Leakage_Theta', Int_Leakage_Theta);  assignin(hws, 'Int_Leakage_Theta', Int_Leakage_Theta);  assignin(hws,'Int_Leakage_Theta', Int_Leakage_Theta);

assignin(hws, 'Control_Analogies', Control_Analogies);

assignin(hws, 'Kp_roll_LUx', Kp_roll_LUx);  assignin(hws, 'Kp_roll_LUy', Kp_roll_LUy);
assignin(hws, 'Kp_pitch_LUx', Kp_pitch_LUx);  assignin(hws, 'Kp_pitch_LUy', Kp_pitch_LUy);

assignin(hws, 'tf_m_right_num', tf_m_right_num);    assignin(hws, 'tf_m_right_den', tf_m_right_den);    assignin(hws, 'tf_m_right_delay', tf_m_right_delay);    assignin(hws, 'tf_m_right_dcgain', tf_m_right_dcgain);
assignin(hws, 'tf_m_left_num', tf_m_left_num);      assignin(hws, 'tf_m_left_den', tf_m_left_den);      assignin(hws, 'tf_m_left_delay', tf_m_left_delay);      assignin(hws, 'tf_m_left_dcgain', tf_m_left_dcgain); 
assignin(hws, 'tf_m_tail_num', tf_m_tail_num);      assignin(hws, 'tf_m_tail_den', tf_m_tail_den);      assignin(hws, 'tf_m_tail_delay', tf_m_tail_delay);      assignin(hws, 'tf_m_tail_dcgain', tf_m_tail_dcgain);

assignin(hws, 'tf_s_right_left_num', tf_s_right_left_num);    assignin(hws, 'tf_s_right_left_den', tf_s_right_left_den);    assignin(hws, 'tf_s_right_left_delay', tf_s_right_left_delay);    assignin(hws, 'tf_s_right_left_dcgain', tf_s_right_left_dcgain);
assignin(hws, 'tf_s_tail_num', tf_s_tail_num);    assignin(hws, 'tf_s_tail_den', tf_s_tail_den);    assignin(hws, 'tf_s_tail_delay', tf_s_tail_delay);    assignin(hws, 'tf_s_tail_dcgain', tf_s_tail_dcgain);

assignin(hws, 'tf_phi_cl_d_num', tf_phi_cl_d_num);    assignin(hws, 'tf_phi_cl_d_den', tf_phi_cl_d_den);    assignin(hws, 'tf_phi_cl_d_delay', tf_phi_cl_d_delay);    assignin(hws, 'tf_phi_cl_d_dcgain', tf_phi_cl_d_dcgain);

%assignin(hws, 'MK_ESC_absolute_constraints', MK_ESC_absolute_constraints);    assignin(hws, 'MK_ESC_constraints', MK_ESC_constraints);        assignin(hws, 'MK_ESC_slewrate_constraint', MK_ESC_slewrate_constraint);
assignin(hws, 'PWM_ESC_absolute_constraints', PWM_ESC_absolute_constraints);  assignin(hws, 'PWM_ESC_constraints', PWM_ESC_constraints);      assignin(hws, 'PWM_ESC_slewrate_constraint', PWM_ESC_slewrate_constraint);
assignin(hws, 'PWM_SERVO_absolute_constraints', PWM_SERVO_absolute_constraints);  assignin(hws, 'PWM_SERVO_constraints', PWM_SERVO_constraints);  assignin(hws, 'PWM_SERVO_slewrate_constraint', PWM_SERVO_slewrate_constraint);

assignin(hws, 'Sonar_Lower_ValueThreshold_constraint', Sonar_Lower_ValueThreshold_constraint);
assignin(hws, 'Sonar_Upper_ValueThreshold_constraint', Sonar_Upper_ValueThreshold_constraint);
assignin(hws, 'Sonar_Integrator_constraint', Sonar_Integrator_constraint);
assignin(hws, 'Sonar_Derivative_constraint', Sonar_Derivative_constraint);

assignin(hws, 'RPM_RateThreshold_constraint', RPM_RateThreshold_constraint);

assignin(hws, 'RPM_Right_ModelErrors_constraint', RPM_Right_ModelErrors_constraint);
assignin(hws, 'RPM_Left_ModelErrors_constraint', RPM_Left_ModelErrors_constraint);
assignin(hws, 'RPM_Tail_ModelErrors_constraint', RPM_Tail_ModelErrors_constraint);
assignin(hws, 'RPM_Right_ModelErrors_Integrator_constraint', RPM_Right_ModelErrors_Integrator_constraint);
assignin(hws, 'RPM_Left_ModelErrors_Integrator_constraint', RPM_Left_ModelErrors_Integrator_constraint);
assignin(hws, 'RPM_Tail_ModelErrors_Integrator_constraint', RPM_Tail_ModelErrors_Integrator_constraint);

assignin(hws, 'ThetaError_constraints', ThetaError_constraints);
assignin(hws, 'ThetaControlAction_constraints', ThetaControlAction_constraints);
assignin(hws, 'ThetaControlAction_slewconstraints', ThetaControlAction_slewconstraints);

assignin(hws, 'Control_Int_0', Control_Int_0);

assignin(hws, 'K_OFcam', K_OFcam);

assignin(hws, 'X_Profile', X_Profile); assignin(hws, 'Y_Profile', Y_Profile); assignin(hws, 'Z_Profile', Z_Profile); assignin(hws, 'Psi_Profile', Psi_Profile); assignin(hws, 'T_Profile', T_Profile);

assignin(hws, 'H_xz', H_xz);  assignin(hws, 'K_xz', K_xz);  assignin(hws, 'Nc_xz', Nc_xz);
assignin(hws, 'Hobj_xz', Hobj_xz);  assignin(hws, 'Fobj_xz', Fobj_xz);  assignin(hws, 'Gobj_xz', Gobj_xz);
assignin(hws, 'F_xz', F_xz); assignin(hws, 'G_xz', G_xz);
assignin(hws, 'Qopt_xz', Qopt_xz);  assignin(hws, 'Ropt_xz', Ropt_xz);
assignin(hws, 'Ml_xz', Ml_xz);  assignin(hws, 'Mr_xz', Mr_xz);

assignin(hws, 'H_y', H_y);  assignin(hws, 'K_y', K_y);  assignin(hws, 'Nc_y', Nc_y);
assignin(hws, 'Hobj_y', Hobj_y);  assignin(hws, 'Fobj_y', Fobj_y);  assignin(hws, 'Gobj_y', Gobj_y);
assignin(hws, 'F_y', F_y); assignin(hws, 'G_y', G_y);
assignin(hws, 'Qopt_y', Qopt_y);  assignin(hws, 'Ropt_y', Ropt_y);
assignin(hws, 'Ml_y', Ml_y);  assignin(hws, 'Mr_y', Mr_y);

hws.saveToSource;
hws.reload;

% Update Matlab Simulation
%set_param('TTR_Matlab', 'SimulationCommand', 'Update');

%% Invoke System Calls
%system('killall TTR_Peripheral');
%system([TTR_Peripheral_dir '/TTR_Peripheral &']);


