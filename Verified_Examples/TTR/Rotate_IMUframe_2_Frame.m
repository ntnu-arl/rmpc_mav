function v_out = Rotate_IMUframe_2_Frame(v_in,phi,theta,psi)
%#codegen

s_phi = sin(phi);      c_phi = cos(phi);
s_theta = sin(theta);  c_theta = cos(theta);
s_psi = sin(psi);      c_psi = cos(psi);

IMUframe_2_frame = ...
    [ c_psi*c_theta  c_psi*s_theta*s_phi-s_psi*c_phi  s_psi*s_phi+c_psi*c_phi*s_theta; ...
      s_psi*c_theta  c_psi*c_phi+s_phi*s_theta*s_psi  s_theta*s_psi*c_phi-c_psi*s_phi; ...
      -s_theta       c_theta*s_phi                    c_theta*c_phi ];
                 
v_out = IMUframe_2_frame * v_in;
end


