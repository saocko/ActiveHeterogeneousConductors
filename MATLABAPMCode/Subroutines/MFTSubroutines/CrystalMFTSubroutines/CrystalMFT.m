
function [MFT_shading_image, free_energy_arrays] = CrystalMFT(mean_dens, sys_params, struct_period, output_path)
fprintf('At CrystalMFT! \n');
[evap_cond_rates.wall skew_info] = CrystalWallOrChanMFTRates(mean_dens, sys_params, 1, struct_period);
[evap_cond_rates.chan skew_info]= CrystalWallOrChanMFTRates(mean_dens, sys_params, 0, struct_period);
evap_cond_rates.is_uniform = 0;


[MFT_shading_image free_energy_arrays] =  GenerateMeanFieldImages(sys_params, evap_cond_rates, skew_info, output_path);

fprintf('Finished CrystalMFT! \n');
