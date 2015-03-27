%%  Plot Wind Trajectory
close all;
plot3(PLOT_DATASET_HELIX.y.ref,PLOT_DATASET_HELIX.x.ref,PLOT_DATASET_HELIX.z.ref,'b','LineWidth',2); hold on;
plot3(PLOT_DATASET_HELIX.y.resp,PLOT_DATASET_HELIX.x.resp,PLOT_DATASET_HELIX.z.resp,'r','LineWidth',2); grid on;
xlabel('y (m)','Interpreter','LaTex','FontSize',18);
ylabel('x (m)','Interpreter','LaTex','FontSize',18);
zlabel('z (m)','Interpreter','LaTex','FontSize',18);

x_0 = 0;
y_0 = -1;
z_0 = 1.5;
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
h1 = patch(PatchData1_X,PatchData1_Y,PatchData1_Z,'y');
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
h2 = patch(PatchData2_X,PatchData2_Y,PatchData2_Z,'y');
set(h2,'FaceLighting','phong','EdgeLighting','phong');
set(h2,'EraseMode','normal');

% Axes settings
axis vis3d equal;
view([-37.5,30]);
camlight;
grid on;
%
rotate(h1,[1 0 0],90)
rotate(h2,[1 0 0],90)
zlim([-0.5  0.5])
material metal
box on;

%%
export_fig HelicalTrajectory_WindDisturbance.eps -eps -transparent -m2 -a2 
%%
export_fig HelicalTrajectory_WindDisturbance.png -png -transparent -m3 -a2 