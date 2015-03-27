%%  Read values of Robust MPC actions
time_vec = simout_data.time - simout_data.time(1);
pitch = simout_data.signals.values(:,3);
pitch_ref = simout_data.signals.values(:,5);
roll_ref = simout_data.signals.values(:,10);
roll = simout_data.signals.values(:,8);
x = simout_data.signals.values(:,1);
y = RecordYZ.signals(1).values(:,1);
z = RecordYZ.signals(2).values(:,1);
%%
PLOT_DATASET.time = time_vec;
PLOT_DATASET.position.x.resp = x;
PLOT_DATASET.position.y.resp = y;
PLOT_DATASET.position.z.resp = z;
PLOT_DATASET.attitude.roll.ref = roll_ref;
PLOT_DATASET.attitude.roll.resp = roll;
PLOT_DATASET.attitude.pitch.ref = pitch_ref;
PLOT_DATASET.attitude.pitch.resp = pitch;
% PLOT_DATASET.attitude.yaw.ref = pitch_ref;
%PLOT_DATASET.attitude.yaw.resp = pitch;
%%
plot(PLOT_DATASET.position.y.resp)
start_i = 5673;
end_i = 6275; % 
PLOT_DATASET.position.x.resp = PLOT_DATASET.position.x.resp(start_i:end_i) - mean(PLOT_DATASET.position.x.resp(start_i:end_i));
PLOT_DATASET.position.y.resp = PLOT_DATASET.position.y.resp(start_i:end_i) -PLOT_DATASET.position.y.resp(start_i); % mean(PLOT_DATASET.position.y.resp(start_i:end_i));
PLOT_DATASET.position.y.ref = 0*PLOT_DATASET.position.y.resp+1.25; PLOT_DATASET.position.y.ref(1:50) = 0;
PLOT_DATASET.position.x.ref = 0*PLOT_DATASET.position.x.resp;
PLOT_DATASET.position.z.resp = PLOT_DATASET.position.z.resp(start_i:end_i);
%%
PLOT_DATASET.attitude.roll.ref = PLOT_DATASET.attitude.roll.ref(start_i:end_i) - mean(PLOT_DATASET.attitude.roll.ref(start_i:end_i));
PLOT_DATASET.attitude.pitch.ref = PLOT_DATASET.attitude.pitch.ref(start_i:end_i) - mean(PLOT_DATASET.attitude.pitch.ref(start_i:end_i));
%PLOT_DATASET.attitude.yaw.ref = PLOT_DATASET.attitude.yaw.ref - mean(PLOT_DATASET.attitude.yaw.ref);
PLOT_DATASET.attitude.roll.resp = PLOT_DATASET.attitude.roll.resp(start_i:end_i) - mean(PLOT_DATASET.attitude.roll.resp(start_i:end_i));
PLOT_DATASET.attitude.pitch.resp = PLOT_DATASET.attitude.pitch.resp(start_i:end_i) - mean(PLOT_DATASET.attitude.pitch.resp(start_i:end_i));
PLOT_DATASET.time = PLOT_DATASET.time(start_i:end_i) - PLOT_DATASET.time(start_i);
%%
ha(1) = subplot(5,1,1);
plot(PLOT_DATASET.time,PLOT_DATASET.position.x.ref,'b--','LineWidth',1); hold on; 
plot(PLOT_DATASET.time,PLOT_DATASET.position.x.resp,'r','LineWidth',1); 
grid on; axis([0 10 -0.25 0.25]);
legend('x^r','x')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$x^r, x$$ (m)','Interpreter','LaTex','FontSize',18);
ha(2) = subplot(5,1,2);
plot(PLOT_DATASET.time,PLOT_DATASET.position.y.ref,'b--','LineWidth',1); hold on; 
plot(PLOT_DATASET.time,PLOT_DATASET.position.y.resp,'r','LineWidth',1); 
legend('y^r','y')
grid on; axis([0 10 -0.25 2]);
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$y^r, y$$ (m)','Interpreter','LaTex','FontSize',18);
ha(3) = subplot(5,1,3);
plot(PLOT_DATASET.time,-0*PLOT_DATASET.position.z.resp+1,'b--','LineWidth',1); grid on; hold on
plot(PLOT_DATASET.time,-PLOT_DATASET.position.z.resp+.8,'r','LineWidth',1); grid on;
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$z^r, z$$ (m)','Interpreter','LaTex','FontSize',18);
legend('z^r','z');
axis([0 10 0.8 1.2]);
ha(4) = subplot(5,1,4);
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.roll.ref,'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.roll.resp,'r','LineWidth',1); grid on;
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\phi^r, \phi$$','Interpreter','LaTex','FontSize',18);
legend('\phi^r','\phi');
axis([0 10 -0.4 0.4]);
ha(5) = subplot(5,1,5);
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.pitch.ref,'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.pitch.resp,'r','LineWidth',1); grid on;
%vline(PLOT_DATASET.time(points_circle(1)),'g--')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\theta^r, \theta$$','Interpreter','LaTex','FontSize',18);
legend('\theta^r','\theta')
axis([0 10 -0.1 0.1]);
%%

vline(PLOT_DATASET.time(points_circle(1)),'g--')
legend('z^r','z')
axis([0 10 .8 1.2])
ha(3) = subplot(4,1,3);
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.roll.ref,'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.roll.resp,'r','LineWidth',1); grid on;
plot(PLOT_DATASET.time(points_circle), PLOT_DATASET.attitude.roll.resp(points_circle),'ro','LineWidth',2);
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\phi^r, \phi$$','Interpreter','LaTex','FontSize',18);
legend('\phi^r','\phi');
ha(4) = subplot(4,1,4);
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.pitch.ref,'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.pitch.resp,'r','LineWidth',1); grid on;
plot(PLOT_DATASET.time(points_circle), PLOT_DATASET.attitude.pitch.resp(points_circle),'ro','LineWidth',2);
vline(PLOT_DATASET.time(points_circle(1)),'g--')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\theta^r, \theta$$','Interpreter','LaTex','FontSize',18);
legend('\theta^r','\theta')
linkaxes(ha,'x');
%%
export_fig PushQuadSlungLoad2.eps -eps -transparent -m2 -a2 
%%
export_fig PushQuadSlungLoad2b.png -png -transparent -m3 -a2 