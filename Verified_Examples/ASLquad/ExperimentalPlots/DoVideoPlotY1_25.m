for kk = 2:2:length(PLOT_DATASET.position.x.resp)
myfig = figure;
ha(1) = subplot(5,1,1);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.x.ref(1:kk),'b--','LineWidth',1); hold on; 
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.x.resp(1:kk),'r','LineWidth',1); 
grid on; axis([0 10 -0.25 0.25]);
legend('x^r','x')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$x^r, x$$ (m)','Interpreter','LaTex','FontSize',18);
ha(2) = subplot(5,1,2);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.y.ref(1:kk),'b--','LineWidth',1); hold on; 
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.y.resp(1:kk),'r','LineWidth',1); 
legend('y^r','y')
grid on; axis([0 10 -0.25 2]);
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$y^r, y$$ (m)','Interpreter','LaTex','FontSize',18);
ha(3) = subplot(5,1,3);
plot(PLOT_DATASET.time(1:kk),-0*PLOT_DATASET.position.z.resp(1:kk)+1,'b--','LineWidth',1); grid on; hold on
plot(PLOT_DATASET.time(1:kk),-PLOT_DATASET.position.z.resp(1:kk)+.8,'r','LineWidth',1); grid on;
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$z^r, z$$ (m)','Interpreter','LaTex','FontSize',18);
legend('z^r','z');
axis([0 10 0.8 1.2]);
ha(4) = subplot(5,1,4);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.attitude.roll.ref(1:kk),'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.attitude.roll.resp(1:kk),'r','LineWidth',1); grid on;
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\phi^r, \phi$$','Interpreter','LaTex','FontSize',18);
legend('\phi^r','\phi');
axis([0 10 -0.4 0.4]);
ha(5) = subplot(5,1,5);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.attitude.pitch.ref(1:kk),'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.attitude.pitch.resp(1:kk),'r','LineWidth',1); grid on;
%vline(PLOT_DATASET.time(points_circle(1)),'g--')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\theta^r, \theta$$','Interpreter','LaTex','FontSize',18);
legend('\theta^r','\theta')
axis([0 10 -0.1 0.1]);
saveas(myfig,['step_125m_' num2str(kk) '.png'],'png');
close all
end