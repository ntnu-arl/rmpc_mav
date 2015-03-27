function TTR_mdl_Dynamics(block)
% Level-2 M file S-Function for TTR_mdl_dynamics
   
  setup(block);
  
%endfunction


function setup(block)

  %% Register number of dialog parameters   
  block.NumDialogPrms = 5;

  %% Register number of input and output ports
  block.NumInputPorts  = 6;
  block.NumOutputPorts = 4 + 2 + 2;

  %% Setup functional port properties to dynamically
  %% inherited.
  block.SetPreCompInpPortInfoToDynamic;
  block.SetPreCompOutPortInfoToDynamic;
 
  block.InputPort(1).Dimensions        = 1;
  block.InputPort(1).DirectFeedthrough = false;
  block.InputPort(2).Dimensions        = 1;
  block.InputPort(2).DirectFeedthrough = false;
  block.InputPort(3).Dimensions        = 1;
  block.InputPort(3).DirectFeedthrough = false;
  block.InputPort(4).Dimensions        = 1;
  block.InputPort(4).DirectFeedthrough = false;
  block.InputPort(5).Dimensions        = 1;
  block.InputPort(5).DirectFeedthrough = false;
  block.InputPort(6).Dimensions        = 1;
  block.InputPort(6).DirectFeedthrough = false;
  
  block.OutputPort(1).Dimensions       = 3;
  block.OutputPort(2).Dimensions       = 3;
  block.OutputPort(3).Dimensions       = 3;
  block.OutputPort(4).Dimensions       = 3;
  
  block.OutputPort(5).Dimensions       = 3;
  block.OutputPort(6).Dimensions       = 3;
  
  block.OutputPort(7).Dimensions       = 3;
  block.OutputPort(8).Dimensions       = 3;

  
  %% Set block sample time to continuous
  block.SampleTimes = [0 0];
  
  %% Setup Dwork
  block.NumContStates = 12;

  %% Set the block simStateCompliance to default (i.e., same as a built-in block)
  block.SimStateCompliance = 'DefaultSimState';

  %% Register methods
  block.RegBlockMethod('InitializeConditions',     @InitConditions);  
  block.RegBlockMethod('Outputs',                  @Output);  
  block.RegBlockMethod('Derivatives',              @Derivative);
  %%%%%%%%%%%%%%%%%%%
  block.RegBlockMethod('SetInputPortSamplingMode', @SetInpPortFrameData);
  block.RegBlockMethod('PostPropagationSetup',@DoPostPropSetup);
  %block.RegBlockMethod('Update',              @Update);  
  
%endfunction


function DoPostPropSetup(block)

%% Setup Dwork
block.NumDworks = 4;
% block.Dwork(1).Name = 'X0'; 
% block.Dwork(1).Dimensions      = n+1;
% block.Dwork(1).DatatypeID      = 0;
% block.Dwork(1).Complexity      = 'Real';
% block.Dwork(1).UsedAsDiscState = false;
block.Dwork(1).Name = 'Omega_Dot';
block.Dwork(1).Dimensions = 3;
block.Dwork(1).DatatypeID = 0;
block.Dwork(1).Complexity = 'Real';
block.Dwork(1).UsedAsDiscState = false;
block.Dwork(2).Name = 'U_Dot';
block.Dwork(2).Dimensions = 3;
block.Dwork(2).DatatypeID = 0;
block.Dwork(2).Complexity = 'Real';
block.Dwork(2).UsedAsDiscState = false;
block.Dwork(3).Name = 'F_tot';
block.Dwork(3).Dimensions = 3;
block.Dwork(3).DatatypeID = 0;
block.Dwork(3).Complexity = 'Real';
block.Dwork(3).UsedAsDiscState = false;
block.Dwork(4).Name = 'M_tot';
block.Dwork(4).Dimensions = 3;
block.Dwork(4).DatatypeID = 0;
block.Dwork(4).Complexity = 'Real';
block.Dwork(4).UsedAsDiscState = false;

%endfunction


function SetInpPortFrameData(block, idx, fd)
  
  block.InputPort(1).SamplingMode = 0;
  block.InputPort(2).SamplingMode = 0;
  block.InputPort(3).SamplingMode = 0;
  block.InputPort(4).SamplingMode = 0;
  block.InputPort(5).SamplingMode = 0;
  block.InputPort(6).SamplingMode = 0;
  
  block.OutputPort(1).SamplingMode  = 0;
  block.OutputPort(2).SamplingMode  = 0;
  block.OutputPort(3).SamplingMode  = 0;
  block.OutputPort(4).SamplingMode  = 0;
  
  block.OutputPort(5).SamplingMode  = 0;
  block.OutputPort(6).SamplingMode  = 0;
  
  block.OutputPort(7).SamplingMode  = 0;
  block.OutputPort(8).SamplingMode  = 0;
  
%endfunction


function InitConditions(block)

  %% Initialize Dwork
  %block.NumDworks = 1;
  %block.Dwork(1).Name = 'x0'; 
  %block.Dwork(1).Dimensions      = 1;
  %block.Dwork(1).DatatypeID      = 0;
  %block.Dwork(1).Complexity      = 'Real';
  %block.Dwork(1).UsedAsDiscState = true;
  
  TTR_mdl_Properties = block.DialogPrm(1).Data;
  m_B = TTR_mdl_Properties.m_B;
  g = TTR_mdl_Properties.g;
  
  F_0 = block.DialogPrm(5).Data;
  FR_0 = F_0.FR_0;
  FL_0 = F_0.FL_0;
  FT_0 = F_0.FT_0;
  
  block.InputPort(1).Data = FR_0;
  block.InputPort(2).Data = 0.0;
  block.InputPort(3).Data = FL_0;
  block.InputPort(4).Data = 0.0;
  block.InputPort(5).Data = FT_0;
  block.InputPort(6).Data = 0.0;

  block.ContStates.Data(1) = 0.0;
  block.ContStates.Data(2) = 0.0;
  block.ContStates.Data(3) = 0.0;
  block.ContStates.Data(4) = 0.0;
  block.ContStates.Data(5) = 0.0;
  block.ContStates.Data(6) = 0.0;
  
%endfunction


function Output(block)

  Omega_Dot = block.Dwork(1).Data;
  U_Dot = block.Dwork(2).Data;
  
  F_tot = block.Dwork(3).Data;
  M_tot = block.Dwork(4).Data;
  
  block.OutputPort(1).Data = block.ContStates.Data(1:3);
  block.OutputPort(2).Data = block.ContStates.Data(4:6);
  block.OutputPort(3).Data = block.ContStates.Data(7:9);
  block.OutputPort(4).Data = block.ContStates.Data(10:12);
  
  block.OutputPort(5).Data = Omega_Dot;
  block.OutputPort(6).Data = U_Dot;
  
  block.OutputPort(7).Data = F_tot;
  block.OutputPort(8).Data = M_tot;

%endfunction


function Derivative(block)
    %   = block.DialogPrm(1).Data;
    % t = block.CurrentTime;
    
    TTR_mdl_Properties = block.DialogPrm(1).Data;
    m_B = TTR_mdl_Properties.m_B;
    g = TTR_mdl_Properties.g;
    Ixx = TTR_mdl_Properties.Ixx;
    Iyy = TTR_mdl_Properties.Iyy;
    Izz = TTR_mdl_Properties.Izz;
    Ixz = TTR_mdl_Properties.Ixz;
    d_RL_x = TTR_mdl_Properties.d_RL_x;
    d_T_x = TTR_mdl_Properties.d_T_x;
    d_RL_y = TTR_mdl_Properties.d_RL_y;
    d_T_y = TTR_mdl_Properties.d_T_y;
    d_RL_z = TTR_mdl_Properties.d_RL_z;
    d_T_z = TTR_mdl_Properties.d_T_z;
    h_RL   = TTR_mdl_Properties.h_RL;
    h_T   = TTR_mdl_Properties.h_T;
    rho   = TTR_mdl_Properties.rho;
    w_chord = TTR_mdl_Properties.w_chord;
    w_span  = TTR_mdl_Properties.w_span;
    w_ac_x  = TTR_mdl_Properties.w_ac_x;
    w_ac_y  = TTR_mdl_Properties.w_ac_y;
    
    Wing_Properties = block.DialogPrm(2).Data;
    a_vect = Wing_Properties.a_vect;
    v_vect = Wing_Properties.v_vect;
    cl     = Wing_Properties.cl;
    cd     = Wing_Properties.cd;
    
    Damping = block.DialogPrm(3).Data;
    Damping_Omega = [Damping.phi  0              0 ;...
                     0            Damping.theta  0 ;...
                     0            0              Damping.psi];
    Damping_U = [Damping.x  0          0 ;...
                 0          Damping.y  0 ;...
                 0          0          Damping.z];
    
    Drag = block.DialogPrm(4).Data;
    
    I       = [Ixx    0      -Ixz;...
               0      Iyy    0;...
              -Ixz    0      Izz];

%%%%%%%%%%%%  Inputs  %%%%%%%%%%%%        
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                         %
%   a_<F|B>: 0:=Heli , pi/2:=Fixed-Wing   %
%                                         %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    T_R     = block.InputPort(1).Data;
    a_R     = block.InputPort(2).Data;
    T_L     = block.InputPort(3).Data;
    a_L     = block.InputPort(4).Data;
    T_T     = block.InputPort(5).Data;
    a_T     = block.InputPort(6).Data;
    
    Theta   = block.ContStates.Data(1:3);
    X       = block.ContStates.Data(4:6);
    
    Omega   = block.ContStates.Data(7:9);
    U       = block.ContStates.Data(10:12);

    p       = Omega(1);
    q       = Omega(2);
    r       = Omega(3);
    
    phi     = Theta(1);
    theta   = Theta(2);
    psi     = Theta(3);
    
    u       = U(1);
    v       = U(2);
    w       = U(3);
    
    x       = X(1);
    y       = X(2);
    z       = X(3);
           
    s_phi    = sin(phi);    c_phi    = cos(phi);
    s_theta  = sin(theta);  c_theta  = cos(theta);
    s_psi    = sin(psi);    c_psi    = cos(psi);
    
    J        = [ 1  (s_phi*s_theta)/c_theta     (c_phi*s_theta)/c_theta;...
                 0  c_phi                       -s_phi;...
                 0  s_phi/c_theta               c_phi/c_theta];
    
    R_B2W    = [ c_psi*c_theta  c_psi*s_theta*s_phi-s_psi*c_phi  s_psi*s_phi+c_psi*c_phi*s_theta; ...
                 s_psi*c_theta  c_psi*c_phi+s_phi*s_theta*s_psi  s_theta*s_psi*c_phi-c_psi*s_phi; ...
                 -s_theta       c_theta*s_phi                    c_theta*c_phi ];
    
    %R_W2B    = inv(R_B2W);
    
%%%%%%%%%%%%  Distance Components  %%%%%%%%%%%%  
    %%% Rotors %%%
    r_R_x    = d_RL_x + h_RL*sin(a_R);
    r_R_y    = d_RL_y;
    r_R_z    = -(d_RL_z + h_RL*cos(a_R));
    
    r_L_x    = d_RL_x + h_RL*sin(a_L);
    r_L_y    = -d_RL_y;
    r_L_z    = -(d_RL_z + h_RL*cos(a_L));
    
    r_T_x    = -d_T_x + h_T*sin(a_T);
    r_T_y    = d_T_y;
    r_T_z    = -(d_T_z + h_T*cos(a_T));

    %%% Wings %%%
    r_WR_x   = d_RL_x - w_ac_x*sin(a_R);
    r_WR_y   = w_ac_y;
    r_WR_z   = -(d_RL_z - w_ac_x*cos(a_R));
    
    r_WL_x   = d_RL_x - w_ac_x*sin(a_L);
    r_WL_y   = -w_ac_y;
    r_WL_z   = -(d_RL_z - w_ac_x*cos(a_L));
    
 %%%%%%%%%%%%  Forces - Position Vectors - Moments  %%%%%%%%%%%%   
    F_R = [T_R*sin(a_R);...
            0;...
            -(T_R*cos(a_R))];
    F_L = [T_L*sin(a_L);...
            0;...
            -(T_L*cos(a_L))];
    F_T = [ 0;...
            T_T*sin(a_T);...
            -(T_T*cos(a_T))];
    
    cl_R = 0;%interp2(a_vect,v_vect,cl , rad2deg(a_R),u, 'spline');
    cl_L = 0;%interp2(a_vect,v_vect,cl , rad2deg(a_L),u, 'spline');
    A_R = abs(w_chord * (w_span/2) * sin(a_R));
    A_L = abs(w_chord * (w_span/2) * sin(a_L)); 
    cd_R = 0;%interp2(a_vect,v_vect,cd , rad2deg(a_R),u, 'spline');
    cd_L = 0;%interp2(a_vect,v_vect,cd , rad2deg(a_L),u, 'spline');
    
    F_WR = [-0.5*rho*cd_R*A_R*u^2;...
            0;...
            -(0.5*rho*cl_R*A_R*u^2)];
    F_WL = [-0.5*rho*cd_L*A_L*u^2;...
            0;...
            -(0.5*rho*cl_L*A_L*u^2)];
    
    F_DRAG = [0;...%-(Drag.a*u^2+Drag.b*u);...
              0;...
              0];
    
    Gw = [0;...
          0;...
          m_B*g];
    Gb = transpose(R_B2W)*Gw;  %inv(R_B2W)*Gw=R_W2B*Gw
           
    r_R  = [r_R_x;...
            r_R_y;...
            r_R_z];
    r_L  = [r_L_x;...
            r_L_y;...
            r_L_z];
    r_T  = [r_T_x;...
            r_T_y;...
            r_T_z];    
    r_WR = [r_WR_x;...
            r_WR_y;...
            r_WR_z];
    r_WL = [r_WL_x;...
            r_WL_y;...
            r_WL_z];
      
    M_R = cross(r_R,F_R);  M_L = cross(r_L,F_L);  M_T = cross(r_T,F_T);
    M_WR = cross(r_WR,F_WR);  M_WL = cross(r_WL,F_WL);
    
    M_tot = M_R + M_L + M_T + M_WR + M_WL;
    
    F_tot = F_R + F_L + F_T + F_WR + F_WL + F_DRAG + Gb;
    
    block.Dwork(3).Data = F_tot;
    block.Dwork(4).Data = M_tot;
    
%%%%%%%%%%%%  Newton - Euler  %%%%%%%%%%%%

    U_Dot = (m_B*eye(3))\(F_tot -Damping_U*U - cross(Omega,(m_B*eye(3))*U)) ;%- 1*eye*(U.*U);
    
    Omega_Dot = I\(M_tot -Damping_Omega*Omega - cross(Omega,I*Omega)) ;%- 1*eye*(Omega.*Omega);
    
    X_Dot = R_B2W * U;
    
    Theta_Dot = J * Omega;
    
    block.Dwork(1).Data = Omega_Dot;
    block.Dwork(2).Data = U_Dot;

%%%%%%%%%%%%  Derivatives  %%%%%%%%%%%%
    
    block.Derivatives.Data(1:3) = Theta_Dot;
    block.Derivatives.Data(4:6) = X_Dot;
    
    block.Derivatives.Data(7:9) = Omega_Dot;
    block.Derivatives.Data(10:12) = U_Dot;
    
%endfunction

