%%  Rotorcraft Parameters
%
%   This is just a test-file
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

%%  Step 1: Define the hquad dynamics
disp('### Loading HQUAD models');
g = 9.8065;

DF_pitch = -1.25;
a1_pitch = 59.71;
a0_pitch = 491;
c0_pitch = 491;

DF_roll = -1.25;
a1_roll = 54.9;
a0_roll = 419.1;
c0_roll = 419.1;

A_x = zeros(4,4);
B_x = zeros(4,1);
A_x(1,2) = 1; A_x(2,2) = DF_pitch; A_x(2,3) = -g; 
A_x(3,4) = 1; A_x(4,3) = -a0_pitch; A_x(4,4) = -a1_pitch;
B_x(4,1) = c0_pitch;
C_x = eye(4);
D_x = zeros(4,1);

A_y = zeros(4,4);
B_y = zeros(4,1);
A_y(1,2) = 1; A_y(2,2) = DF_roll; A_y(2,3) = g; 
A_y(3,4) = 1; A_y(4,3) = -a0_roll; A_y(4,4) = -a1_roll;
B_y(4,1) = c0_roll;
C_y = eye(4);
D_y = zeros(4,1);

sys_x = ss(A_x,B_x,C_x,D_x);
sys_y = ss(A_y,B_y,C_y,D_y);
%sys_z = ss(A_z,B_z,C_z,D_z);

%%  Discretize
disp('### Discretize')

sys_x_d = c2d(sys_x,Ts,'zoh');
sys_y_d = c2d(sys_y,Ts,'zoh');
%sys_z_d = c2d(sys_z,Ts,'zoh');

disp('### Models Loaded');