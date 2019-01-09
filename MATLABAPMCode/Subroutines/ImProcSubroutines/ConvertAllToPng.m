function [] = ConvertAllToPng(sys_params, input_path, output_path)
MakeFilePath(output_path);
%Just converts a bunch of tiffs into pngs


fprintf('Converting to png, input path is %s, output path is %s ...', input_path, output_path);
for evap_index = 1:length(sys_params.evap)
   for cond_index = 1:length(sys_params.cond)
      evap_windtax = sys_params.evap(evap_index);
      cond_windtax = sys_params.cond(cond_index);
      cur_tiff_file_path = sprintf('%s/SnapshotEvapWindTax%.2fcondWindTax%.2f.tiff', input_path, evap_windtax, cond_windtax);
   %   fprintf(cur_tiff_file_path);
    %  fprintf('\n');
      png_file_path = sprintf('%s/EvapInd%dCondInd%d.png',output_path, evap_index, cond_index);
        
       
      my_image = imread(cur_tiff_file_path);
      %Hopefully this is ok. 
      last_part_of_image = my_image(:, :, 4);
      image_range = range(last_part_of_image(:));
      my_image = my_image(:, :, 1:3);
      
     % fprintf(png_file_path);
     % fprintf('\n');
      
      
      imwrite(my_image, png_file_path);
       
       
   end    
end
fprintf('Finished \n');
