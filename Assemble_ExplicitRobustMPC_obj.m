function ExplicitRobustMPC_obj = Assemble_ExplicitRobustMPC_obj(sol_x_mp,add_usys_d,U_bounds,Y_Limit)

%   Get the matrices for the table travelsal algorithm:
[H,K,F,G,Nc] = GetRobustMPC_Matrices(sol_x_mp{1});
disp('### Note 1: Always check that my code works ok!');
Nx = length(add_usys_d.matrices.A);

ExplicitRobustMPC_obj.H = H;
ExplicitRobustMPC_obj.K = K;
ExplicitRobustMPC_obj.F = F;
ExplicitRobustMPC_obj.G = G;
ExplicitRobustMPC_obj.Nc = Nc;
ExplicitRobustMPC_obj.Nx = Nx;
[~,Nu] = size(add_usys_d.continuous.matrices.B);
ExplicitRobustMPC_obj.Nu = Nu;
ExplicitRobustMPC_obj.U_bounds = U_bounds;
ExplicitRobustMPC_obj.Y_Limit = Y_Limit;
ExplicitRobustMPC_obj.Ts = add_usys_d.Ts;


end

