%%  Rotorcraft Parameters
%
%   This is just a test-file
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

%%  Step 1: Assume some second order closed loop attitude dynamics
disp('### Define the Attitude Subsystem Closed-Loop Dynamics');

A_rp = [0 1; 0 -10];
B_rp = [0; 10];
C_rp = eye(2);
D_rp = zeros(2,1);

A_y = [0 1; 0 -2;];
B_y = [0; 2];
C_y = eye(2);
D_y = zeros(2,1);

sys_rp = ss(A_rp,B_rp,C_rp,D_rp);
sys_y = ss(A_y,B_y,C_y,D_y);

close all
subplot(2,1,1); step(sys_rp(2,1)); grid on;
subplot(2,1,2); step(sys_y(2,1)); grid on;

%%  Step 2: Assume a first order thrust model
disp('### Define Thrust Model');

A_t = -1;
B_t = 1;
C_t = 1;
D_t = 0;

sys_t = ss(A_t,B_t,C_t,D_t);


%%  Cascade the attitude - thrust dynamics with the translational dynamics
disp('### Cascade the Attitude-Thrust Dynamics with the Translational Dynamics');
disp('!!! -> sys_x, sys_y WE ASSUME WE ONLY MEASURE VELOCITY');

m = 1.0;
g = 9.8065; % m/s^2
DF_x = -5;
A_x =  [0   1   0   0;
        0   DF_x   g   0;
        0   0   A_rp(1,1)   A_rp(1,2);
        0   0   A_rp(2,1)   A_rp(2,2);];
B_x = [ 0; 0; B_rp(1); B_rp(2) ];
C_x = eye(4);% C_x(1,1) = 0; C_x(3,3) = 0; C_x(4,4) = 0;
% C_x = eye(4);
D_x = zeros(4,1);

A_y = A_x;
A_y(2,3) = - A_y(2,3);
B_y = B_x;
C_y = C_x;
D_y = D_x;

A_z = [0 1 0;
       0 0 (1/m);
       0 0 A_t(1);];
B_z = [0; 0; B_t(1);];
C_z = eye(3); %C_z(1,1) = 0; C_z(3,3) = 0;
D_z = zeros(3,1);

sys_x = ss(A_x,B_x,C_x,D_x);
sys_y = ss(A_y,B_y,C_y,D_y);
sys_z = ss(A_z,B_z,C_z,D_z);

%%  Discretize
disp('### Discretize')
Ts = 0.02;

sys_x_d = c2d(sys_x,Ts,'zoh');
sys_y_d = c2d(sys_y,Ts,'zoh');
sys_z_d = c2d(sys_z,Ts,'zoh');