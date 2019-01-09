
function [MFT_rates, dens_info] = UniformWallOrChanMFTRates(sys_params, is_wall, mean_dens);

%Calculates the evaporation and condensation rates for different densities for both the wall phase separation and channelization
%phase separation
fprintf('At UniformWallOrChanMFTRates! \n');
taxis_mode = 2;
[dens_info, mean_perm filled_volt_list, empty_volt_list] = LoadUniformMFTData(mean_dens);



for dens_ind = 1:length(dens_info.values)
    cur_permeability = myTern(is_wall, 1, mean_perm(dens_ind));
    cur_dens = dens_info.values(dens_ind);
    
    %Calculates the condensation at this dens value
    for cond_ind = 1:length(sys_params.cond)
        cond_coeff = sys_params.cond(cond_ind);
        
        %Calculates the total cond rates for region 1 and 2.
        [mean_cond{cond_ind}(dens_ind), max_cond_frac{cond_ind}(dens_ind)] =...
            MeanCondRate(cur_dens, cur_permeability, cond_coeff, empty_volt_list{dens_ind}, taxis_mode);
    end
    
    %Calculates the evaporation at this dens value
    for evap_ind = 1:length(sys_params.evap)
        evap_coeff = sys_params.evap(evap_ind);
        
        %Calculates the total even and odd evap and cond rates.
        [mean_evap{evap_ind}(dens_ind), max_evap_frac{evap_ind}(dens_ind)] =...
            MeanEvapRate(cur_dens, cur_permeability, evap_coeff, filled_volt_list{dens_ind}, taxis_mode);
    end
end

%Packs the information into a single object.
MFT_rates.evap = mean_evap;
MFT_rates.cond = mean_cond;
fprintf('Finished UniformWallOrChanMFTRates! \n');
