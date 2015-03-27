start_i = 3.592e+4;
end_i = 3.732e+4;
% end_i = length(ASLQUAD_FLIGHT_RESULTS.Time);


close all;
PLOT_DATASET.time = ASLQUAD_FLIGHT_RESULTS.Time(start_i:end_i);
PLOT_DATASET.time = PLOT_DATASET.time - PLOT_DATASET.time(1);
PLOT_DATASET.position.x.resp = ASLQUAD_FLIGHT_RESULTS.Translational.Full.Xpos.values(start_i:end_i,1);
PLOT_DATASET.position.x.resp = PLOT_DATASET.position.x.resp - PLOT_DATASET.position.x.resp(1);
PLOT_DATASET.position.x.ref = PLOT_DATASET.position.x.resp*0 + mean(PLOT_DATASET.position.x.resp((end-150):end,1));
PLOT_DATASET.position.y.resp = ASLQUAD_FLIGHT_RESULTS.Translational.Full.Ypos.values(start_i:end_i,1);
PLOT_DATASET.position.y.resp = PLOT_DATASET.position.y.resp - PLOT_DATASET.position.y.resp(1);
PLOT_DATASET.position.y.ref = PLOT_DATASET.position.x.resp*0 + mean(PLOT_DATASET.position.y.resp((end-150):end,1));
PLOT_DATASET.position.y.ref(1:91) = 0;
PLOT_DATASET.position.z.resp = ASLQUAD_FLIGHT_RESULTS.Translational.Full.Zpos.values(start_i:end_i,1) - mean(PLOT_DATASET.position.z.resp);
PLOT_DATASET.position.z.ref = PLOT_DATASET.position.x.resp*0 + mean(ASLQUAD_FLIGHT_RESULTS.Translational.Full.Zpos.values((end_i-150):end_i,1))*0;
PLOT_DATASET.position.vx.resp = ASLQUAD_FLIGHT_RESULTS.Translational.Full.VX.values(start_i:end_i,1);
PLOT_DATASET.position.vy.resp = ASLQUAD_FLIGHT_RESULTS.Translational.Full.VY.values(start_i:end_i,1);
PLOT_DATASET.position.vz.resp = ASLQUAD_FLIGHT_RESULTS.Translational.Full.VZ.values(start_i:end_i,1);
plot(PLOT_DATASET.position.y.resp);
%%
close all;

for kk = 2:2:length(PLOT_DATASET.position.x.resp)
myfig = figure;
ha(1) = subplot(5,1,1);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.x.ref(1:kk),'b--','LineWidth',1); hold on; 
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.x.resp(1:kk),'r','LineWidth',1); 
grid on;   axis([0 10 -1 1]);
legend('x^r','x')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$x^r, x$$ (m)','Interpreter','LaTex','FontSize',18);
ha(2) = subplot(5,1,2);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.y.ref(1:kk),'b--','LineWidth',1); hold on; 
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.y.resp(1:kk),'r','LineWidth',1); 
legend('y^r','y')
grid on;  axis([0 10 -0.25 4]);
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$y^r, y$$ (m)','Interpreter','LaTex','FontSize',18);
ha(3) = subplot(5,1,3);
plot(PLOT_DATASET.time(1:kk),-0*PLOT_DATASET.position.z.resp(1:kk)+1,'b--','LineWidth',1); grid on; hold on
plot(PLOT_DATASET.time(1:kk),-PLOT_DATASET.position.z.resp(1:kk)+1,'r','LineWidth',1); grid on;
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$z^r, z$$ (m)','Interpreter','LaTex','FontSize',18);
legend('z^r','z');
axis([0 10 0.8 1.2]);
ha(4) = subplot(5,1,4);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.vx.resp(1:kk),'k-','LineWidth',1); hold on;
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\dot{x} (m/s)$$','Interpreter','LaTex','FontSize',18);
axis([0 10 -0.4 0.4]); grid on
ha(5) = subplot(5,1,5);
plot(PLOT_DATASET.time(1:kk),PLOT_DATASET.position.vy.resp(1:kk),'k-','LineWidth',1); hold on;
%vline(PLOT_DATASET.time(points_circle(1)),'g--')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\dot{y} (m/s)$$','Interpreter','LaTex','FontSize',18);
axis([0 10 -0.5 3]); grid on;
saveas(myfig,['step_large_' num2str(kk) '.png'],'png');
close all
end