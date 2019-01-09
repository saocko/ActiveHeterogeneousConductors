
function [MFT_rates, skew_info] = CrystalWallOrChanMFTRates(mean_dens, sys_params, is_wall, struct_period)
fprintf('At CrystalWallOrChanMFTRates! \n');
%Calculates the MFT rates for different density separations

reg1frac = 1./struct_period;
reg2frac = 1-reg1frac;



%%%%%%Does some file path stuff, fairly boring
if(is_wall)    
    local_path = sprintf('MFTSamples/NonUniform/%d/ThinWall/Dens%.4f/', struct_period, mean_dens);    
else
    local_path = sprintf('MFTSamples/NonUniform/%d/ThinChan/Dens%.4f/', struct_period, mean_dens);
end

%fprintf('Local_path is %s! \n', local_path);
%Finished file path stuff

[skew_info, perm, filled_volt_list_reg1, empty_volt_list_reg1, filled_volt_list_reg2, empty_volt_list_reg2] =...
    LoadCrystalMFTData(local_path, mean_dens);
%Finished loading the data. 

for skew_ind = 1:length(skew_info.values)
   % cur_skew_amount = legal_skews(skew_ind);
    dens_1 = skew_info.reg1dens(skew_ind);
    dens_2 = skew_info.reg2dens(skew_ind);    
    cur_permeability = mean(perm{skew_ind});
        
    %Calculates the condensation at this skew value
    for cond_ind = 1:length(sys_params.cond)
        
        cond_coeff = sys_params.cond(cond_ind);
        
        %Calculates the total cond rates for region 1 and 2.
        taxis_mode = 2;
        [mean_cond_1{cond_ind}(skew_ind), max_cond_1_frac{cond_ind}(skew_ind)] =...
            MeanCondRate(dens_1, cur_permeability, cond_coeff, empty_volt_list_reg1{skew_ind}, taxis_mode);
        
        [mean_cond_2{cond_ind}(skew_ind), max_cond_2_frac{cond_ind}(skew_ind)] =...
            MeanCondRate(dens_2, cur_permeability, cond_coeff, empty_volt_list_reg2{skew_ind}, taxis_mode);
    end
    
   %Calculates the evaporation at this skew value    
    for evap_ind = 1:length(sys_params.evap)
        evap_coeff = sys_params.evap(evap_ind);
        
        %Calculates the total even and odd evap and cond rates.
        taxis_mode = 2;
        [mean_evap_1{evap_ind}(skew_ind), max_evap_1_frac{evap_ind}(skew_ind)] =...
            MeanEvapRate(dens_1, cur_permeability, evap_coeff, filled_volt_list_reg1{skew_ind}, taxis_mode);
        
        [mean_evap_2{evap_ind}(skew_ind), max_evap_2_frac{evap_ind}(skew_ind)] =...
            MeanEvapRate(dens_2, cur_permeability, evap_coeff, filled_volt_list_reg2{skew_ind}, taxis_mode);
    end
end

%Packs the information into a single object. 
MFT_rates.evap1 = mean_evap_1;
MFT_rates.evap2 = mean_evap_2;
MFT_rates.cond1 = mean_cond_1;
MFT_rates.cond2 = mean_cond_2;

for cond_ind = 1:length(sys_params.cond)
    MFT_rates.cond_ratios{cond_ind} = MFT_rates.cond1{cond_ind} .* 1./MFT_rates.cond2{cond_ind};
end

for evap_ind = 1:length(sys_params.evap)
    MFT_rates.evap_ratios{evap_ind} = MFT_rates.evap1{evap_ind} .* 1./MFT_rates.evap2{evap_ind};
end







