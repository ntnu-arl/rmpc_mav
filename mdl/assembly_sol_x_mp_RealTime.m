function sol_x_mp_rt = assembly_sol_x_mp_RealTime(sol_x_mp,x_bounds_min,x_bounds_max,desired_samples_per_state)


Pn = sol_x_mp{1}.Pn;

Fi_cell = sol_x_mp{1}.Fi;
Gi_cell = sol_x_mp{1}.Gi;

for i = 1:length(Fi_cell)
    Fi(i,:) = Fi_cell{i};
    Gi(i,:) = Gi_cell{i};
end

sol_x_mp_rt.Pn = Pn;
sol_x_mp_rt.Fi = Fi;
sol_x_mp_rt.Gi = Gi;


[index_row,STATESGRID] = grid_ss_polytope_index(sol_x_mp,x_bounds_min,x_bounds_max,desired_samples_per_state)

sol_x_mp_rt.GRID.index_row = index_row;
sol_x_mp_rt.GRID.STATESGRID = STATESGRID;

end
