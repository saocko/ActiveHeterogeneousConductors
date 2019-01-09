function[MFT_shading_image free_energy_arrays] = GenerateMeanFieldImages(sys_params, evap_cond_rates, dens_or_skew_info, output_file_path)


%Takes the evaporation and condensation rates and and calls UniformPhaseSeparationPlot or CrystalPhaseSeparationPlot for every
%possible value
fprintf('Starting GenerateMeanFieldImages for path %s ...', output_file_path);
MFT_shading_image=  WhiteShadingArray(sys_params);
chan_wall_shade_bit_array = cell(2, 1);


for i = 1:2
    if(i ==1)
        wall_chan_string = 'Chan';
        rates_to_use = evap_cond_rates.chan;
    elseif(i ==2)
        wall_chan_string = 'Wall';
        rates_to_use = evap_cond_rates.wall;
    end
    
    
    for cond_ind = 1:length(sys_params.cond)
        for evap_ind = 1:length(sys_params.evap)
            %fprintf('Doing evap, cond (%d, %d) \n', evap_ind, cond_ind);
            
            output_file_name = sprintf('%s/%sEvapInd%dCondInd%d', output_file_path,  wall_chan_string, evap_ind, cond_ind);
            
            
            if(evap_cond_rates.is_uniform)
                cond_evap_ratios = rates_to_use.cond{cond_ind} .* 1./(rates_to_use.evap{evap_ind});
                [is_stable free_en_diff] = UniformPhaseSeparationPlot(dens_or_skew_info, cond_evap_ratios, output_file_name);                
            else
                cond_evap_ratios = (rates_to_use.cond1{cond_ind} .* 1./rates_to_use.evap1{evap_ind}) ./(rates_to_use.cond2{cond_ind} .* 1./rates_to_use.evap2{evap_ind});
                [is_stable free_en_diff] = CrystalPhaseSeparationPlot(dens_or_skew_info, cond_evap_ratios, output_file_name);                
            end            
            
            MFT_shading_image(cond_ind, evap_ind, :) = WallChanShadeImage(MFT_shading_image(cond_ind, evap_ind, :), ~is_stable && (i ==2), ~is_stable&&(i ==1));
            
            if(i ==1)
                free_energy_arrays.chan(cond_ind, evap_ind) = free_en_diff;
            elseif(i == 2)
               free_energy_arrays.wall(cond_ind, evap_ind) = free_en_diff; 
            end
            
        end
    end
end

fprintf('Finished \n');