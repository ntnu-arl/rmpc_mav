%%  Rotorcraft Parameters
%
%   This is just a test-file
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

%%  Step 1: Define TTR-parameters

% Rotor Dynamics
hr=tf(7.655946120461085,[0.145992771705270 1]);   hr.OutputDelay=0.03388;
hl=tf(7.531092760896522,[0.141638503909445 1]);   hl.OutputDelay=0.034055;
PolyR_R2T = [0.004088843035973 -8.629151839250008]; PolyL_R2T = [0.003950480213225 -8.757691155123139];  
tf_RL = tf( 0.5*(PolyR_R2T(1)*hr.num{1}+PolyL_R2T(1)*hl.num{1}) , 0.5*(hr.den{1}+hl.den{1}) ); 
tf_RL.num{1} = 1;  %Assume Directly Controlling Force (dcgain=1)
tf_RL.InputDelay = 0.5*(hr.ioDelay+hr.InputDelay+hr.OutputDelay + hl.ioDelay+hl.InputDelay+hl.OutputDelay);
ss_RL = ss(tf_RL); ss_RL.B=ss_RL.B*ss_RL.C; ss_RL.C=1;

% Servo Dynamics
ss_S = ss(-200,200,1,0);  %Assume Directly Controlling Radians (dcgain=1)
ss_S.InputDelay = 0.02;

%Closed-Loop phi Dynamics
%Old
PHI = tf(1.119,[0.0355 0.1663 1]);   PHI.InputDelay=0.04;
%New
PHI = tf(0.99749,[0.21585 1]);   PHI.InputDelay=0.15517;
%ss
ss_PHI = idss(PHI,'SSParameterization','Canonical');

%Mechanical Setup
m_B = 2.35;
g = 9.81;
d_RL_x = 0.0250;
d_T_x = 0.355;

dmp_x = 0.1;
dmp_y = 0.5;
dmp_z = 0.1;

% X = [x;x_dot;delta_g]

g_0 = 0;
F_0 = ( (m_B*g)/(1+(d_RL_x/d_T_x)) )/cos(g_0);

A = [0  1       0;
     0  -dmp_x  (cos(g_0)*F_0)/m_B;
     0  0       ss_S.A];
     
B = [0;
     0;
     ss_S.B];

C = eye(3);
D = zeros(3,1);

TTR_X=ss(A,B,C,D);

% Z = [z;z_dot;delta_F]

g_0 = 0;
F_0 = ( (m_B*g)/(1+(d_RL_x/d_T_x)) )/cos(g_0);

A = [0  1       0;
     0  -dmp_z  cos(g_0)/m_B;
     0  0       ss_RL.A];

B = [0;
     0;
     ss_RL.B];

C = eye(3);
D = zeros(3,1);

TTR_Z=ss(A,B,C,D);

% Y = [y;y_dot;phi]  OLD

g_0 = 0;
F_0 = ( (m_B*g)/(1+(d_RL_x/d_T_x)) )/cos(g_0);

A = [0  1       0;
     0  -dmp_y  (cos(g_0)*F_0)/m_B;
     0  0       ss_PHI.A];

B = [0;
     0;
     ss_PHI.B];

C = eye(3);
D = zeros(3,1);
TTR_Y=ss(A,B,C,D);

sys_x = TTR_X;
sys_y = TTR_Y;
sys_z = TTR_Z;
A_x = sys_x.A; B_x = sys_x.B; C_x = sys_x.C; D_x = sys_x.D;
A_y = sys_y.A; B_y = sys_y.B; C_y = sys_y.C; D_y = sys_y.D;
A_z = sys_z.A; B_z = sys_z.B; C_z = sys_z.C; D_z = sys_z.D;


%%  Discretize
disp('### Discretize')

sys_x_d = c2d(sys_x,Ts,'zoh');
sys_y_d = c2d(sys_y,Ts,'zoh');
sys_z_d = c2d(sys_z,Ts,'zoh');