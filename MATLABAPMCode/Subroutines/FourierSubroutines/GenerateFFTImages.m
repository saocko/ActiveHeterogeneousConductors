function[] = GenerateFFTImages(sys_params, input_file_path, output_file_path)


system(sprintf('mkdir -p %s', output_file_path));

fprintf('At GenerateFFTImages for path %s  \n', input_file_path);


[wall_flucts chan_flucts] =  MFTEinsteinCriteria(sys_params);

for cond_ind = 1:length(sys_params.cond)
   for evap_ind = 1:length(sys_params.evap)
       evap_windtax = sys_params.evap(evap_ind);
       cond_windtax = sys_params.cond(cond_ind);
       
       cur_input_file = strcat(input_file_path, sprintf('/EnsemblePlotEvapWindTax%.2fcondWindTax%.2f.txt', evap_windtax, cond_windtax));
       
       
       cur_MFT_fluct.chan = chan_flucts{evap_ind}{cond_ind};
       cur_MFT_fluct.wall = wall_flucts{evap_ind}{cond_ind};
              
       [cur_fft_image, max_weight, fft_err_bar] = GenerateSingleFFTImage(cur_input_file, sys_params.n_sweeps);
       cur_fft_image(1,1) = mean(cur_fft_image(:));
        
       fft_im_path = sprintf('%s/FFTIms/FFTEvapInd%dCondInd%d.png', output_file_path, evap_ind, cond_ind);
       MakeFilePath(fft_im_path);
       imwrite(1.-gray2rgb(NormAndScrollImage(cur_fft_image)), fft_im_path);
       
       CompareFFTWithMFT(cur_fft_image, fft_err_bar, sprintf('%s/FFTScatter/AngleDependFFTEvapInd%dCondInd%d', output_file_path, evap_ind, cond_ind), cur_MFT_fluct) ;
       
       
   end    
end

fprintf('Finished GenerateFFTImages \n');

