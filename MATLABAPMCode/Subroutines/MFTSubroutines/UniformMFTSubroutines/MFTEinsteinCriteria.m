
function [wall_flucts, chan_flucts] = MFTEinsteinCriteria(sys_params)
fprintf('At MFTEinsteinCriteria ! \n');
%Plugs the evap and cond rates into the einstein criteria


[evap_cond_rates.wall dens_info] = UniformWallOrChanMFTRates(sys_params, 1, sys_params.mean_dens);
[evap_cond_rates.chan  dens_info]= UniformWallOrChanMFTRates(sys_params, 0, sys_params.mean_dens);

delta_dens = dens_info.values(dens_info.base_index +1) - dens_info.values(dens_info.base_index -1);



for evap_ind = 1:length(sys_params.evap)
    for cond_ind = 1:length(sys_params.cond)        
        evap_data{1} = evap_cond_rates.chan.evap{evap_ind};
        cond_data{1} = evap_cond_rates.chan.cond{cond_ind};
        
        evap_data{2} = evap_cond_rates.wall.evap{evap_ind};
        cond_data{2} = evap_cond_rates.wall.cond{cond_ind};
        
        for is_wall_ind = 1:2
            
            cur_evap = evap_data{is_wall_ind};
            cur_cond = cond_data{is_wall_ind};
            
            base_evap = cur_evap(dens_info.base_index);            
            delta_evap = cur_evap(dens_info.base_index +1) - cur_evap(dens_info.base_index -1);
            
            base_cond = cur_cond(dens_info.base_index);            
            delta_cond = cur_cond(dens_info.base_index +1) - cur_cond(dens_info.base_index -1);
            
            norm = base_cond/base_evap; %How much the evap rate is intensified to keep the particle number constant 
            base_noise = base_evap * norm + base_cond;
            
            
            
            jacobian = (delta_evap* norm -delta_cond)/delta_dens;            
            
            amplitude{is_wall_ind} = (1/2) * base_noise/jacobian;
            
        end
        
        chan_flucts{evap_ind}{cond_ind} = amplitude{1};
        wall_flucts{evap_ind}{cond_ind} = amplitude{2};        
        
    end
end

fprintf('Finished MFTEinsteinCriteria ! \n');



