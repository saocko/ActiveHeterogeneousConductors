function[histogram_shade_image] = GenerateClumpHistogramImages(sys_params, input_file_path, output_file_path)



fprintf('At Generate GenerateClumpHistogramImages \n');
fprintf('Starting to Generate Hists for input path %s ...', input_file_path);
%input_file_path
histogram_shade_image=  WhiteShadingArray(sys_params);

for cond_ind = 1:length(sys_params.cond)
   for evap_ind = 1:length(sys_params.evap)
       evap_windtax = sys_params.evap(evap_ind);
       cond_windtax = sys_params.cond(cond_ind);
       
       
       cur_input_file_path =  strcat(input_file_path, sprintf('/EnsemblePlotEvapWindTax%.2fcondWindTax%.2f.txt', evap_windtax, cond_windtax));

       clump_bimodal = GenerateSingleClumpHistoImage(cur_input_file_path, sys_params.n_sweeps, sprintf('%s/Clump/ClumpHistEvapInd%dCondInd%d.png', output_file_path, evap_ind, cond_ind));
       
       histogram_shade_image(cond_ind, evap_ind, :) = WallChanShadeImage(histogram_shade_image(cond_ind, evap_ind, :), clump_bimodal, clump_bimodal);       
              
   end    
end

fprintf('Finished \n');

