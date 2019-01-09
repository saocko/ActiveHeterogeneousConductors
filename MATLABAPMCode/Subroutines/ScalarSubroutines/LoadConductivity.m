function[conductivity, conduct_stdev] = LoadConductivity(sys_params, input_path)
%Loads the files for conductivity and puts the information into an array

fprintf('At LoadConductivity... \n' );

for cond_ind = 1:length(sys_params.cond)
    for evap_ind = 1:length(sys_params.evap)
        for sweep_num = 1:sys_params.n_sweeps
            
            evap_windtax = sys_params.evap(evap_ind);
            cond_windtax = sys_params.cond(cond_ind);
            
            input_path_with_sweep = sprintf(input_path, (sweep_num -1));
            conduct_txt_file_path = sprintf('%s/ConductivityEvapWindTax%.2fcondWindTax%.2f.txt', input_path_with_sweep, evap_windtax, cond_windtax);
            
            conduct_txt_file_path;
            conductivity_with_sweeps(cond_ind, evap_ind, sweep_num)   = mean(importdata(conduct_txt_file_path));
            %conductivity(dens_ind, cond_ind, evap_ind)
        end
    end
end

conductivity = mean(conductivity_with_sweeps, 3);


fprintf('Finished LoadConductivity \n' );
