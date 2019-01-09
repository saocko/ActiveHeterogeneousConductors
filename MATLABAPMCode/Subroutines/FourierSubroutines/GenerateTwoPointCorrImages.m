function[] = GenerateTwoPointCorrImages(sys_params, input_file_path, output_file_path)


system(sprintf('mkdir -p %s', output_file_path));

fprintf('Starting GenerateTwoPointCorrImages for path %s \n', input_file_path);


for cond_ind = 1:length(sys_params.cond)
   for evap_ind = 1:length(sys_params.evap)
       evap_windtax = sys_params.evap(evap_ind);
       cond_windtax = sys_params.cond(cond_ind);
       cur_input_file = strcat(input_file_path,  sprintf('/EnsemblePlotEvapWindTax%.2fcondWindTax%.2f.txt', evap_windtax, cond_windtax));
       cur_corr_image = GenerateSingleTwoPointCorrImage(cur_input_file, sys_params.n_sweeps);
       
%       cur_corr_image(1,1) = 0;
       imwrite(1.-gray2rgb(NormAndScrollImage(cur_corr_image)), sprintf('%s/TPCEvapInd%dCondInd%d.png', output_file_path, evap_ind, cond_ind));
       
   end    
end
fprintf('Finished GenerateTwoPointCorrImages \n');

