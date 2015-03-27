function [y,t] = lsim_addusys(add_usys_d,W_x_bounds,u,t)
%
%   Syntax:
%   [y,t] = lsim_addusys(add_usys_d,W_x_bounds,u,t)
%
%   This function works as the "linear simulation - lsim" command for the 
%   add_usys class. More documentation will follow and the function will be
%   improved.
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

sys = ss(add_usys_d.continuous.matrices.A,add_usys_d.continuous.matrices.B,add_usys_d.continuous.matrices.C,add_usys_d.continuous.matrices.D);
sys_add = ss(add_usys_d.continuous.matrices.A,add_usys_d.continuous.matrices.E,add_usys_d.continuous.matrices.C,add_usys_d.continuous.matrices.D);

y_nom = lsim(sys,u,t,zeros(length(sys.A),1));
y_add_max = lsim(sys_add,max(W_x_bounds)*ones(length(t),1),t,zeros(length(sys.A),1));
y_add_min = lsim(sys_add,min(W_x_bounds)*ones(length(t),1),t,zeros(length(sys.A),1));


close all;
num_out = min(size(sys.C));

if(num_out == 1)
    plot(t,y_nom,'b'); hold on;
    plot(t,y_nom+y_add_max,'r');
    plot(t,y_nom+y_add_min,'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(1) ')']);
end

if(num_out == 2)
    subplot(2,1,1)
    plot(t,y_nom(:,1),'b'); hold on;
    plot(t,y_nom(:,1)+y_add_max(:,1),'r');
    plot(t,y_nom(:,1)+y_add_min(:,1),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(1) ')']);
    subplot(2,1,2)
    plot(t,y_nom(:,2),'b'); hold on;
    plot(t,y_nom(:,2)+y_add_max(:,2),'r');
    plot(t,y_nom(:,2)+y_add_min(:,2),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(2) ')']);
end

if(num_out == 3)
    subplot(3,1,1)
    plot(t,y_nom(:,1),'b'); hold on;
    plot(t,y_nom(:,1)+y_add_max(:,1),'r');
    plot(t,y_nom(:,1)+y_add_min(:,1),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(1) ')']);
    subplot(3,1,2)
    plot(t,y_nom(:,2),'b'); hold on;
    plot(t,y_nom(:,2)+y_add_max(:,2),'r');
    plot(t,y_nom(:,2)+y_add_min(:,2),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(2) ')']);
    subplot(3,1,3)
    plot(t,y_nom(:,2),'b'); hold on;
    plot(t,y_nom(:,2)+y_add_max(:,3),'r');
    plot(t,y_nom(:,2)+y_add_min(:,3),'k'); 
    grid on;
    xlabel('Time (s)'); ylabel(['y(' num2str(3) ')']);
end

if(num_out >=4)
    for i = 1:num_out
        figure(i)
        plot(t,y_nom(:,i),'b'); hold on;
        plot(t,y_nom(:,i)+y_add_max(:,i),'r');
        plot(t,y_nom(:,i)+y_add_min(:,i),'k'); 
        grid on;
        xlabel('Time (s)'); ylabel(['y(' num2str(i) ')']);
    end
end


y = [];
y.nom = y_nom;
y.add_max = y_add_max;
y.add_min = y_add_min;
t = t;

end
