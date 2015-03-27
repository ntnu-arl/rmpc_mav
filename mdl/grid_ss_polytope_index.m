function [index_row,STATESGRID] = grid_ss_polytope_index(sol_x_mp,x_bounds_min,x_bounds_max,desired_samples_per_state)

index_row = [];
STATESGRID = [];
if length(x_bounds_min) ~= length(x_bounds_max)
    disp('ERROR: length(x_bounds_min) not equal to length(x_bounds_max)');
    index_row = [];
else
    for i = 1:length(x_bounds_min)
        sampling_space = (x_bounds_max(i) - x_bounds_min(i))/desired_samples_per_state;
        this_state_samples{i} = x_bounds_min(i):sampling_space:x_bounds_max(i);
    end
    
    min_len = 10^6;
    for i = 1:length(x_bounds_min)
        min_len = min(min_len,length(this_state_samples{i}));
    end
    
    for i = 1:length(x_bounds_min)
        this_state_samples{i} = this_state_samples{i}(1:min_len);
        this_state_samples{i}(end) = x_bounds_max(i);
    end
    
    if length(x_bounds_min) == 1
        STATES_GRID_X1 = this_state_samples{1};
        JJ_prev = [];
        for k = 1:length(STATES_GRID_X1)
            [~,JJ] = isinside(sol_x_mp{1}.Pn, STATES_GRID_X1(k));
            if isempty(JJ) == 1
                JJ = JJ_prev;
            end
            index_row(k) = JJ;
            JJ_prev = JJ;
        end
        STATESGRID.XGRID1 = STATES_GRID_X1;
        STATESGRID.VECTORS.X1 = this_state_samples{1};
    end
    if length(x_bounds_min) == 2
        [STATES_GRID_X1,STATES_GRID_X2] = ndgrid(this_state_samples{1},this_state_samples{2});
        index_row = [];
        JJ_prev = [];
        for k = 1:(desired_samples_per_state+1)
            for l = 1:(desired_samples_per_state+1)
                x_state_value = [STATES_GRID_X1(k,l) STATES_GRID_X2(k,l)]';
                [~,JJ] = isinside(sol_x_mp{1}.Pn, x_state_value);
                if isempty(JJ) == 1
                    JJ = JJ_prev;
                end
                index_row(k,l) = JJ;
                JJ_prev = JJ;
            end
        end
        STATESGRID.XGRID1 = STATES_GRID_X1;
        STATESGRID.XGRID2 = STATES_GRID_X2;
        STATESGRID.VECTORS.X1 = this_state_samples{1};
        STATESGRID.VECTORS.X2 = this_state_samples{2};
    end
    if length(x_bounds_min) == 3
        [STATES_GRID_X1,STATES_GRID_X2,STATES_GRID_X3] = ndgrid(this_state_samples{1},this_state_samples{2},this_state_samples{3});
        index_row = [];
        JJ_prev = [];
        for k = 1:(desired_samples_per_state+1)
            for l = 1:(desired_samples_per_state+1)
                for m = 1:(desired_samples_per_state+1)
                    x_state_value = [STATES_GRID_X1(k,l,m) STATES_GRID_X2(k,l,m) STATES_GRID_X3(k,l,m)]';
                    [~,JJ] = isinside(sol_x_mp{1}.Pn, x_state_value);
                    if isempty(JJ) == 1
                    	JJ = JJ_prev;
                    end
                    index_row(k,l,m) = JJ;
                    JJ_prev = JJ;
                end
            end
        end
        STATESGRID.XGRID1 = STATES_GRID_X1;
        STATESGRID.XGRID2 = STATES_GRID_X2;
        STATESGRID.XGRID3 = STATES_GRID_X3;
        STATESGRID.VECTORS.X1 = this_state_samples{1};
        STATESGRID.VECTORS.X2 = this_state_samples{2};
        STATESGRID.VECTORS.X3 = this_state_samples{3};

    end
    if length(x_bounds_min) == 4
        [STATES_GRID_X1,STATES_GRID_X2,STATES_GRID_X3,STATES_GRID_X4] = ndgrid(this_state_samples{1},this_state_samples{2},this_state_samples{3},this_state_samples{4});
        index_row = [];
        JJ_prev = [];
        for k = 1:(desired_samples_per_state+1)
            for l = 1:(desired_samples_per_state+1)
                for m = 1:(desired_samples_per_state+1)
                    for n = 1:(desired_samples_per_state+1)
                        x_state_value = [STATES_GRID_X1(k,l,m,n) STATES_GRID_X2(k,l,m,n) STATES_GRID_X3(k,l,m,n) STATES_GRID_X4(k,l,m,n)]'; 
                        [~,JJ] = isinside(sol_x_mp{1}.Pn, x_state_value);
                        if isempty(JJ) == 1
                            JJ = JJ_prev;
                        end
                        index_row(k,l,m,n) = JJ;
                        JJ_prev = JJ;
                    end
                end
            end
        end
        STATESGRID.XGRID1 = STATES_GRID_X1;
        STATESGRID.XGRID2 = STATES_GRID_X2;
        STATESGRID.XGRID3 = STATES_GRID_X3;
        STATESGRID.VECTORS.X1 = this_state_samples{1};
        STATESGRID.VECTORS.X2 = this_state_samples{2};
        STATESGRID.VECTORS.X3 = this_state_samples{3};
        STATESGRID.VECTORS.X4 = this_state_samples{4};
    end
    if length(x_bounds_min) > 4
        disp('### Not implemented for more than 4 states');
        STATESGRID = [];
    end

   
end

disp('Hi');
end