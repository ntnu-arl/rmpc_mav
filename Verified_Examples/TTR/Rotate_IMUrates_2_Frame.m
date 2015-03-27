function v_out = Rotate_IMUrates_2_Frame(v_in,phi,theta)
%#codegen

s_phi = sin(phi);      c_phi = cos(phi);
s_theta = sin(theta);  c_theta = cos(theta);

IMUrates_2_frame = [ 1 s_phi*(s_theta/c_theta) c_phi*(s_theta/c_theta); ...
                     0 c_phi                   -s_phi; ...
                     0 s_phi/c_theta           c_phi/c_theta ];
                 
v_out = IMUrates_2_frame * v_in;
end


