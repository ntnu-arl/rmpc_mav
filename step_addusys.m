function [y,t] = step_addusys(add_usys_d,W_x_bounds)
%
%   Syntax:
%   [y,t] = step_addusys(add_usys_d,W_x_bounds)
%
%   This function works as the "step response" command for the add_usys
%   class. More documentation will follow and the function will be
%   improved.
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

sys = ss(add_usys_d.continuous.matrices.A,add_usys_d.continuous.matrices.B,add_usys_d.continuous.matrices.C,add_usys_d.continuous.matrices.D);
sys_add = ss(add_usys_d.continuous.matrices.A,add_usys_d.continuous.matrices.E,add_usys_d.continuous.matrices.C,add_usys_d.continuous.matrices.D);

min_eig = min(eig(sys.A));
Simul_time = 10*(1/abs(min_eig));

T_simul = Simul_time/10000;

time_vec = 0:T_simul:Simul_time;

y_nom = lsim(sys,ones(length(time_vec),1),time_vec,zeros(length(sys.A),1));
y_add_max = lsim(sys_add,max(W_x_bounds)*ones(length(time_vec),1),time_vec,zeros(length(sys.A),1));
y_add_min = lsim(sys_add,min(W_x_bounds)*ones(length(time_vec),1),time_vec,zeros(length(sys.A),1));

num_out = min(size(sys.C));

close all;

if(num_out == 1)
    plot(time_vec,y_nom,'b'); hold on;
    plot(time_vec,y_nom+y_add_max,'r');
    plot(time_vec,y_nom+y_add_min,'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(1) ')']);
end

if(num_out == 2)
    subplot(2,1,1)
    plot(time_vec,y_nom(:,1),'b'); hold on;
    plot(time_vec,y_nom(:,1)+y_add_max(:,1),'r');
    plot(time_vec,y_nom(:,1)+y_add_min(:,1),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(1) ')']);
    subplot(2,1,2)
    plot(time_vec,y_nom(:,2),'b'); hold on;
    plot(time_vec,y_nom(:,2)+y_add_max(:,2),'r');
    plot(time_vec,y_nom(:,2)+y_add_min(:,2),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(2) ')']);
end

if(num_out == 3)
    subplot(3,1,1)
    plot(time_vec,y_nom(:,1),'b'); hold on;
    plot(time_vec,y_nom(:,1)+y_add_max(:,1),'r');
    plot(time_vec,y_nom(:,1)+y_add_min(:,1),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(1) ')']);
    subplot(3,1,2)
    plot(time_vec,y_nom(:,2),'b'); hold on;
    plot(time_vec,y_nom(:,2)+y_add_max(:,2),'r');
    plot(time_vec,y_nom(:,2)+y_add_min(:,2),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(2) ')']);
    subplot(3,1,3)
    plot(time_vec,y_nom(:,2),'b'); hold on;
    plot(time_vec,y_nom(:,2)+y_add_max(:,3),'r');
    plot(time_vec,y_nom(:,2)+y_add_min(:,3),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(3) ')']);
end

if(num_out >=4)
    for i = 1:num_out
        figure(i)
        plot(time_vec,y_nom(:,i),'b'); hold on;
        plot(time_vec,y_nom(:,i)+y_add_max(:,i),'r');
        plot(time_vec,y_nom(:,i)+y_add_min(:,i),'k'); 
        grid on;
        xlabel('Time (s)'); ylabel(['y(' num2str(i) ')']);
    end
end


y = [];
y.nom = y_nom;
y.add_max = y_add_max;
y.add_min = y_add_min;
t = time_vec;
end
