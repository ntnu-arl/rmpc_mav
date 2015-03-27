%%  Aerial Writer Plots
%
%   This script provides functionality for plotting of the Aerial Writer
%   Experiments conducted in collaboration with G. Darivianakis.
%
%   Authors: Kostas Alexis (konstantinos.alexis@mavt.ethz.ch)

addpath(genpath([pwd '/HQUAD_Physical_Interaction']));

%%  PLOT 1: 
PLOT_DATASET.time = simout_data.time - simout_data.time(1);
start_i = 9605; % boiler pipes
start_i = 851;
end_i = 450;
end_i = 1055;
end_i = 1680;

PLOT_DATASET.time = PLOT_DATASET.time(start_i:end_i); PLOT_DATASET.time = PLOT_DATASET.time - PLOT_DATASET.time(1);
PLOT_DATASET.position.x = simout_data.signals.values(start_i:end_i,1);
PLOT_DATASET.position.y = RecordYZ.signals(1).values(start_i:end_i,1);
PLOT_DATASET.position.z = RecordYZ.signals(2).values(start_i:end_i,1);
PLOT_DATASET.drawing_ref.y = .96*RecordYZ.signals(1).values(start_i:end_i,2) - 0.045;
PLOT_DATASET.drawing_ref.z = .84*RecordYZ.signals(2).values(start_i:end_i,2)-0.0865;
PLOT_DATASET.angles.pitch = simout_data.signals.values(start_i:end_i,3);
PLOT_DATASET.angles.pitch_ref = simout_data.signals.values(start_i:end_i,5);
close all;
plot3(PLOT_DATASET.position.x,PLOT_DATASET.position.y,PLOT_DATASET.position.z);

%
format compact 

WALL_POS = max(PLOT_DATASET.position.x);
rect_start_x = WALL_POS;
rect_end_x = WALL_POS + 0.25;

SURFACE_RECTANGLE.YMIN = min(PLOT_DATASET.position.y) - 0.2;
SURFACE_RECTANGLE.YMAX = max(PLOT_DATASET.position.y) +0.2;
SURFACE_RECTANGLE.ZMIN = min(PLOT_DATASET.position.z) - 0.2;
SURFACE_RECTANGLE.ZMAX = max(PLOT_DATASET.position.z) + 0.2;
SURFACE_RECTANGLE.XMIN = WALL_POS;
SURFACE_RECTANGLE.XMAX = WALL_POS + 0.25;
SURFACE_RECTANGLE.color = [.5 1 1];

PLOT3D_OPTIONS.Light.Position = [-3 -1 3];
PLOT3D_OPTIONS.Light.Camlight = [45 45];
PLOT3D_OPTIONS.ViewAngle = [45 45];
%

PIPES_OBJECTS.YMIN = min(PLOT_DATASET.position.y) - 0.2;
PIPES_OBJECTS.YMAX = max(PLOT_DATASET.position.y) +0.2;
PIPES_OBJECTS.ZMIN = min(PLOT_DATASET.position.z) - 0.2;
PIPES_OBJECTS.ZMAX = max(PLOT_DATASET.position.z) + 0.2;
PIPES_OBJECTS.XMIN = WALL_POS;
PIPES_OBJECTS.XMAX = WALL_POS + 0.25;
PIPES_OBJECTS.color = [.5 1 1];
PIPES_OBJECTS.radius = 0.05;
PIPES_OBJECTS.points = 20;
PIPES_OBJECTS.color = [.9 .55 0.4];
PIPES_OBJECTS.alpha = .8;


h = plot_aerial_obstacleCollision_Pies(PLOT_DATASET,PIPES_OBJECTS,PLOT3D_OPTIONS)
%%

export_fig CollisionUnknownObstacle_vS.eps -eps -transparent -a2 -m2
%%
export_fig CollisionUnknownObstacle_vS.png -png -transparent -a2 -m3
