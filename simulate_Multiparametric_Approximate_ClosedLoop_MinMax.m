function [y,u_ctrl_seq,t] = simulate_Multiparametric_Approximate_ClosedLoop_MinMax(add_usys_d,sol_x_mp,x_state_init,time_sec,Optimizer_x,W_x_bounds,x_state)
%
%   Simulate Closed Loop response of Approximate Closed Loop MinMax
%   solution, computed using Multiparametric methods:
%
%   Inputs:
%   add_usys_d      : state space additive uncertainty sys in Discrete time
%   sol_x_mp        : Multiparametric solution of the controller
%   x_state_init    : initial condition
%
%   Outputs:
%   y               : output
%   

t = 0:add_usys_d.Ts:time_sec;
x_state_k = x_state_init;
x_state_k_updt = x_state_init;

if(isrow(x_state_init))
    y(1,:) = x_state_init;
else
    y(1,:) = x_state_init';
end
size_B = size(add_usys_d.matrices.B); N_u = size_B(2);

u_ctrl_seq = zeros(1,N_u);
u_ctrl = 0;
u_ctrl_prev = u_ctrl;
ops = sdpsettings;
for i = 1:length(t)
    [u_ctrl,sol_found, flag_strange] = robustmpc_getInput(sol_x_mp, y(i,:)');
%   assign(x_state,x_state_k_updt);
%   u_ctrl = double(Optimizer_x);
%    if((sol_found == 0)||(flag_strange == 1))
    if((sol_found == 0))

        u_ctrl = u_ctrl_prev;
    end
    if( (i == 1)&&(u_ctrl==0) )
        u_ctrl = 1e-3;
    end
     
    x_state_k_updt = add_usys_d.matrices.A*x_state_k(:,end) + add_usys_d.matrices.B*double(u_ctrl) + add_usys_d.matrices.E*W_x_bounds(2)*(-1+2*rand(1));
    
    x_state_k = [x_state_k x_state_k_updt];
    y(i+1,:) = double(x_state_k(:,i+1));
    
    u_ctrl_seq(i+1,:) = u_ctrl;
    u_ctrl_prev = u_ctrl;
end

close all;
num_outs = min(size(add_usys_d.matrices.C));

for i = 1:num_outs
    figure(i)
    plot(t,y(1:(end-1),i)); grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(i) ')']);
end

for k = 1:N_u
    figure(i+k)
    plot(t,u_ctrl_seq(1:(end-1),k),'r'); grid on;
    xlabel('Time (s)'); ylabel(['u(' num2str(k) ')']);
end

end