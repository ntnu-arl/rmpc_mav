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
plot(PLOT_DATASET.attitude.roll.resp)
start_i = 4063;
end_i = 4563; % 
PLOT_DATASET.position.x.resp = PLOT_DATASET.position.x.resp(start_i:end_i) - mean(PLOT_DATASET.position.x.resp(start_i:end_i));
PLOT_DATASET.position.y.resp = PLOT_DATASET.position.y.resp(start_i:end_i) - mean(PLOT_DATASET.position.y.resp(start_i:end_i));
PLOT_DATASET.position.y.ref = 0*PLOT_DATASET.position.y.resp;
PLOT_DATASET.position.x.ref = 0*PLOT_DATASET.position.x.resp;
PLOT_DATASET.position.z.resp = PLOT_DATASET.position.z.resp(start_i:end_i);
%%
close all;
PLOT_DATASET.attitude.roll.ref = PLOT_DATASET.attitude.roll.ref(start_i:end_i) - mean(PLOT_DATASET.attitude.roll.ref(start_i:end_i));
PLOT_DATASET.attitude.pitch.ref = PLOT_DATASET.attitude.pitch.ref(start_i:end_i) - mean(PLOT_DATASET.attitude.pitch.ref(start_i:end_i));
%PLOT_DATASET.attitude.yaw.ref = PLOT_DATASET.attitude.yaw.ref - mean(PLOT_DATASET.attitude.yaw.ref);
PLOT_DATASET.attitude.roll.resp = PLOT_DATASET.attitude.roll.resp(start_i:end_i) - mean(PLOT_DATASET.attitude.roll.resp(start_i:end_i));
PLOT_DATASET.attitude.pitch.resp = PLOT_DATASET.attitude.pitch.resp(start_i:end_i) - mean(PLOT_DATASET.attitude.pitch.resp(start_i:end_i));
PLOT_DATASET.time = PLOT_DATASET.time(start_i:end_i) - PLOT_DATASET.time(start_i);
%%
points_circle = [150 192 217 409];
points_circle = [150 194 228 439];

ha(1) = subplot(4,1,1);
plot(PLOT_DATASET.time,PLOT_DATASET.position.x.ref,'b--','LineWidth',1); hold on; 
plot(PLOT_DATASET.time,PLOT_DATASET.position.x.resp,'k','LineWidth',1); 
plot(PLOT_DATASET.time,PLOT_DATASET.position.y.resp,'r','LineWidth',1); grid on;
plot(PLOT_DATASET.time(points_circle), PLOT_DATASET.position.x.resp(points_circle),'ko','LineWidth',2);
plot(PLOT_DATASET.time(points_circle), PLOT_DATASET.position.y.resp(points_circle),'ro','LineWidth',2);
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$x^r,y^r x,y$$ (m)','Interpreter','LaTex','FontSize',18);
legend('x^,y^r','x','y')
vline(PLOT_DATASET.time(points_circle(1)),'g--')
ha(2) = subplot(4,1,2);
plot(PLOT_DATASET.time,-PLOT_DATASET.position.z.resp + .9,'r','LineWidth',1); hold on; grid on;
plot(PLOT_DATASET.time,-0*PLOT_DATASET.position.z.resp + 1,'b--','LineWidth',1);
plot(PLOT_DATASET.time(points_circle), -PLOT_DATASET.position.z.resp(points_circle)+.9,'ro','LineWidth',2);
vline(PLOT_DATASET.time(points_circle(1)),'g--')
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$z^r, z$$ (m)','Interpreter','LaTex','FontSize',18);
legend('z^r','z')
axis([0 10 .8 1.2])
ha(3) = subplot(4,1,3);
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.roll.ref,'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.roll.resp,'r','LineWidth',1); grid on;
plot(PLOT_DATASET.time(points_circle), PLOT_DATASET.attitude.roll.resp(points_circle),'ro','LineWidth',2);
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\phi^r, \phi$$','Interpreter','LaTex','FontSize',18);
legend('\phi^r','\phi');
vline(PLOT_DATASET.time(points_circle(1)),'g--')
ha(4) = subplot(4,1,4);
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.pitch.ref,'b--','LineWidth',1); hold on;
plot(PLOT_DATASET.time,PLOT_DATASET.attitude.pitch.resp,'r','LineWidth',1); grid on;
plot(PLOT_DATASET.time(points_circle), PLOT_DATASET.attitude.pitch.resp(points_circle),'ro','LineWidth',2);
xlabel('Time (s)','Interpreter','LaTex','FontSize',18); ylabel('$$\theta^r, \theta$$','Interpreter','LaTex','FontSize',18);
legend('\theta^r','\theta')
vline(PLOT_DATASET.time(points_circle(1)),'g--')
linkaxes(ha,'x');

%%
export_fig HitSlungLoad2.eps -eps -transparent -m2 -a2 
%%
export_fig HitSlungLoad2.png -png -transparent -m2 -a2 
