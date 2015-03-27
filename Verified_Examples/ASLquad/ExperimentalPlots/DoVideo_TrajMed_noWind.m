for kk = 2:2:length(PLOT_DATASET.position.x.resp)
    close all;
    myfig = figure;
    plot3(PLOT_DATASET_RECT.position.y.ref(1:kk),PLOT_DATASET_RECT.position.x.ref(1:kk),PLOT_DATASET_RECT.position.z.ref(1:kk)-0.5,'b','LineWidth',2); hold on;
    plot3(PLOT_DATASET_RECT.position.y.resp(1:kk),PLOT_DATASET_RECT.position.x.resp(1:kk),PLOT_DATASET_RECT.position.z.resp(1:kk)-0.5,'r','LineWidth',2); grid on;
    plot3(PLOT_DATASET_RECT.position.y.ref(1:kk),PLOT_DATASET_RECT.position.x.ref(1:kk),PLOT_DATASET_RECT.position.z.ref(1:kk)-0.5,'b','LineWidth',2); hold on;
    xlim([-0.75 0.75]);
    ylim([-0.75 0.75]);
    xlabel('y (m)','Interpreter','LaTex','FontSize',18);
    ylabel('x (m)','Interpreter','LaTex','FontSize',18);
    zlabel('z (m)','Interpreter','LaTex','FontSize',18);

    x_0 = 0;
    y_0 = -1;
    z_0 = 1.5;
    x_0 = 0;
    y_0 = 1.15;
    z_0 = -0.55;
    Radius = 0.3;
    Height = 0.2;
    SideCount = 50;

    % Vertices
    n_side = SideCount;

    % Axes settings
    axis vis3d equal;
    view([-37.5,30]);
    camlight;
    grid on;
    %
    zlim([0.5  1.5])
    xlim([-0.75 0.75]);
    ylim([-0.75 0.75]);
    material metal
    box on;
    saveas(myfig,['Traj_noWind_' num2str(kk) '.png'],'png');
end

