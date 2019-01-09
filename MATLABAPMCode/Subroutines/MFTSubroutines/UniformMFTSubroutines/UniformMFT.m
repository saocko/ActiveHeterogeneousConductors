
function [MFT_shading_image free_energy_arrays] = UniformMFT(mean_dens, sys_params, output_path)
fprintf('At UniformMFT! \n');


[evap_cond_rates.wall dens_info] = UniformWallOrChanMFTRates(sys_params, 1, mean_dens);
[evap_cond_rates.chan  dens_info]= UniformWallOrChanMFTRates(sys_params, 0, mean_dens);
evap_cond_rates.is_uniform = 1;


[MFT_shading_image free_energy_arrays] =  GenerateMeanFieldImages(sys_params, evap_cond_rates, dens_info, output_path);

fprintf('Finished UniformMFT! \n');
