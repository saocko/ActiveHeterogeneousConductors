function[cumulants] = LoadHistogramsAndCalculateBinderCumulants(sys_params, input_path)

fprintf('At LoadHistogramsAndCalculateBinderCumulants... \n' );

for cond_ind = 1:length(sys_params.cond)
    for evap_ind = 1:length(sys_params.evap)
        for sweep_num = 1:sys_params.n_sweeps
            
            evap_windtax = sys_params.evap(evap_ind);
            cond_windtax = sys_params.cond(cond_ind);
            
            input_path_with_sweep = sprintf(input_path, (sweep_num -1));
            mat_file_path = sprintf(input_path, evap_ind, cond_ind);
            
            load(mat_file_path);            
            
            
            perm_txt_file_path = sprintf('%s/EvapWindTax%.2fcondWindTax%.2f.txt', input_path_with_sweep, evap_windtax, cond_windtax);            
            cumulants(cond_ind, evap_ind) = CalculateBinderCumulant(obj_to_save.vals, obj_to_save.error_bars);
            %permeability(dens_ind, cond_ind, evap_ind)
        end
    end
end



fprintf('Finished LoadHistogramsAndCalculateBinderCumulants \n' );
