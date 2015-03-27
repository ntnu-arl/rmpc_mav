load nodes.txt
load edges.txt
load optpath.txt
close all
figure
daspect ([1 1 1]);


% Draw the nodes
max_a = max(nodes(:,1));
hold on, plot3(nodes (:,1), nodes(:,2), 0*nodes(:,2)+.5, 'g.')


% Draw the edges 
for i = 1 : length (edges) 
    hold on, plot3( [edges(i,1), edges(i,3)], [edges(i,2), edges(i,4)],[.5, .5], 'Color', [0.1 0.5 0.7], 'LineWidth', 0.5);
end

hold on
plot3(optpath(:,1),optpath(:,2),0*optpath(:,2)+1,'bo-','LineWidth',2,'MarkerFaceColor','g','MarkerEdgeColor','b')
% 
% 
% % Draw the optimal path
% for i = 1:length (optpath)
%     hold on, plot ([optpath(i,1), optpath(i,3)], [optpath(i,2), optpath(i,4)], 'r-', 'LineWidth', 4.0);    
% end

%%
% close all
hold on;
displ_x = -2.5;
displ_y = 2.5;
displ_z = 0;
P_obs_2 = Polyhedron('lb', [-2.855+0.2 ; 0.3893+0.2; 0;], 'ub', [-1.369-0.2; 2.262-0.2; 2-0.2]) - 0*[displ_x displ_y displ_z]';
plot(P_obs_2)
P_obs_1 = Polyhedron('lb', [-2.855 ; 0.3893; 0;], 'ub', [-1.369; 2.262; 2]) - 0*[displ_x displ_y displ_z]';
plot(P_obs_1)

%%

%%  Read values of Robust MPC actions
time_vec = simout_data.time - simout_data.time(1);
pitch = simout_data.signals.values(:,3);
pitch_ref = simout_data.signals.values(:,5);
roll_ref = simout_data.signals.values(:,10);
roll = simout_data.signals.values(:,8);
x = simout_data.signals.values(:,1);
y = RecordYZ.signals(1).values(:,1);
z = RecordYZ.signals(2).values(:,1);
x_ref = traj_FG.X_ref_x(1,:);
y_ref = traj_FG.X_ref_y(1,:);
z_ref = traj_FG.X_ref_z(1,:);

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

PLOT_DATASET.position.x.ref = optpath(:,2);
PLOT_DATASET.position.y.ref = optpath(:,1);

plot(PLOT_DATASET.attitude.roll.resp)
start_i = 7374;
end_i = 11300; % 
PLOT_DATASET.position.x.resp = PLOT_DATASET.position.x.resp(start_i:end_i) - mean(PLOT_DATASET.position.x.resp(start_i:end_i));
PLOT_DATASET.position.y.resp = PLOT_DATASET.position.y.resp(start_i:end_i) - mean(PLOT_DATASET.position.y.resp(start_i:end_i));
PLOT_DATASET.position.z.resp = PLOT_DATASET.position.z.resp(start_i:end_i) - mean(PLOT_DATASET.position.z.resp) + 1
PLOT_DATASET.position.z.ref = 0*PLOT_DATASET.position.x.ref + 1;

PLOT_DATASET.attitude.roll.ref = PLOT_DATASET.attitude.roll.ref(start_i:end_i) - mean(PLOT_DATASET.attitude.roll.ref(start_i:end_i));
PLOT_DATASET.attitude.pitch.ref = PLOT_DATASET.attitude.pitch.ref(start_i:end_i) - mean(PLOT_DATASET.attitude.pitch.ref(start_i:end_i));
PLOT_DATASET.attitude.roll.resp = PLOT_DATASET.attitude.roll.resp(start_i:end_i) - mean(PLOT_DATASET.attitude.roll.resp(start_i:end_i));
PLOT_DATASET.attitude.pitch.resp = PLOT_DATASET.attitude.pitch.resp(start_i:end_i) - mean(PLOT_DATASET.attitude.pitch.resp(start_i:end_i));
PLOT_DATASET.time = PLOT_DATASET.time(start_i:end_i) - PLOT_DATASET.time(start_i);
PLOT_DATASET.position.y.resp = PLOT_DATASET.position.y.resp - (-1.539 - (-2.334));
PLOT_DATASET.position.x.resp = PLOT_DATASET.position.x.resp - (-1.193 - 0.11);
PLOT_DATASET.position.y.resp = lsim(tf(6.28,[1 6.28]),PLOT_DATASET.position.y.resp,PLOT_DATASET.time,PLOT_DATASET.position.y.resp(1));
PLOT_DATASET.position.x.resp = lsim(tf(6.28,[1 6.28]),PLOT_DATASET.position.x.resp,PLOT_DATASET.time,PLOT_DATASET.position.x.resp(1));
PLOT_DATASET.position.y.resp(1:46) = PLOT_DATASET.position.y.resp(46);
PLOT_DATASET.position.x.resp(1:46) = PLOT_DATASET.position.x.resp(46);
%%
% close all
hold on;
displ_x = -2.5;
displ_y = 2.5;
displ_z = 0;
P_obs_1 = Polyhedron('lb', [-2.855 ; 0.3093; 0;], 'ub', [-1.239; 2.262; 2]) - 0*[displ_x displ_y displ_z]';
plot(P_obs_1)


plot3(PLOT_DATASET.position.y.ref,PLOT_DATASET.position.x.ref,PLOT_DATASET.position.z.ref,'b','LineWidth',2); hold on;
plot3(PLOT_DATASET.position.y.resp,PLOT_DATASET.position.x.resp,PLOT_DATASET.position.z.resp,'r','LineWidth',2); grid on;
xlabel('y (m)','Interpreter','LaTex','FontSize',18);
ylabel('x (m)','Interpreter','LaTex','FontSize',18);
zlabel('z (m)','Interpreter','LaTex','FontSize',18);

x_0 = -1.5-1*1.5+1+.6; % -1
y_0 = 0+2.5+ 0.25;
z_0 = .8 - 0+0*2+0.5;
% x_0 = 0-1-2;
% y_0 = 1.25-2;
% z_0 = 0.75;
Radius = 0.3;
Height = 0.2;
SideCount = 50;

% Vertices
n_side = SideCount;

for i_ver=1:n_side
    VertexData(i_ver,:) = [Radius*cos(2*pi/n_side*i_ver),Radius*sin(2*pi/n_side*i_ver),0];
    VertexData(n_side+i_ver,:) = [Radius*cos(2*pi/n_side*i_ver),Radius*sin(2*pi/n_side*i_ver),Height];
end

% Side Patches
for i_pat=1:n_side-1
    Index_Patch1(i_pat,:) = [i_pat,i_pat+1,i_pat+1+n_side,i_pat+n_side];
end
Index_Patch1(n_side,:) = [n_side,1,1+n_side,2*n_side];

for i_pat=1:n_side
    
    % Side patches data
    PatchData1_X(:,i_pat) = VertexData(Index_Patch1(i_pat,:),1);
    PatchData1_Y(:,i_pat) = VertexData(Index_Patch1(i_pat,:),2);
    PatchData1_Z(:,i_pat) = VertexData(Index_Patch1(i_pat,:),3);
end

PatchData1_X = PatchData1_X + x_0;
PatchData1_Y = PatchData1_Y + y_0;
PatchData1_Z = PatchData1_Z + z_0;

% Draw side patches
figure(1);
h1 = patch(PatchData1_X,PatchData1_Y,PatchData1_Z,'b');
rotate(h1,[0,1,0],deg2rad(45)*180/pi,[-1.65,2.75,1])
rotate(h1,[1 0 0],90,[-1.65,2.75,1])
% [-1.0,1.8,1]
% rotate(h1,[1 0 0],90)
set(h1,'FaceLighting','phong','EdgeLighting','phong');
set(h1,'EraseMode','normal');

% Bottom Patches
Index_Patch2(1,:) = [1:n_side];
Index_Patch2(2,:) = [n_side+1:2*n_side];

for i_pat=1:2
    
    % Bottom patches data
    PatchData2_X(:,i_pat) = VertexData(Index_Patch2(i_pat,:),1);
    PatchData2_Y(:,i_pat) = VertexData(Index_Patch2(i_pat,:),2);
    PatchData2_Z(:,i_pat) = VertexData(Index_Patch2(i_pat,:),3);
end

PatchData2_X = PatchData2_X + x_0;
PatchData2_Y = PatchData2_Y + y_0;
PatchData2_Z = PatchData2_Z + z_0;

% Draw bottom patches
figure(1);
h2 = patch(PatchData2_X,PatchData2_Y,PatchData2_Z,'b');
rotate(h2,[0,1,0],deg2rad(45)*180/pi,[-1.65,2.75,1])
rotate(h2,[1 0 0],90,[-1.65,2.75,1])
set(h2,'FaceLighting','phong','EdgeLighting','phong');
set(h2,'EraseMode','normal');

%%
% Axes settings
axis vis3d equal;
view([35,30]);
camlight;
grid on;
%
% rotate(h1,[1 0 0],90)
% rotate(h2,[1 0 0],90)
% zlim([0.5  1.5])
material metal
box on;
