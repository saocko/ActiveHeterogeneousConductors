function [] = MakeMaxFreeEnImages(sys_params, unif_free_en, half_free_en, quart_free_en, output_path)
fprintf('MakeMaxFreeEnImages, output path is %s ... \n', output_path);
close all;

MakeFilePath(output_path);

square_width = 20;

[x_mesh_grid, y_mesh_grid]  = meshgrid(1:square_width, 1:square_width);

x_mesh_grid = gray2rgb(x_mesh_grid);
y_mesh_grid = gray2rgb(y_mesh_grid);


unif_chan_im = x_mesh_grid > square_width/2;
unif_wall_im = y_mesh_grid > square_width/2;

quart_chan_im = mod(x_mesh_grid, 4)  ==0;
quart_wall_im = mod(y_mesh_grid, 4) == 0;


half_chan_im = mod(x_mesh_grid, 2) == 0;
half_wall_im = mod(y_mesh_grid, 2) ==0;

stable_image = .5 * ones(size(x_mesh_grid));


my_image_list =     {unif_chan_im     , unif_wall_im     , half_chan_im     , half_wall_im,      quart_chan_im,      quart_wall_im};
cat_free_en = cat(3, unif_free_en.chan, unif_free_en.wall, half_free_en.chan, half_free_en.wall, quart_free_en.chan, quart_free_en.wall);

[max_val, max_ind] = max(cat_free_en, [], 3);



for evap_index = 1:length(sys_params.evap)
   for cond_index = 1:length(sys_params.cond)
       
       
       trans_ind =  mod((1:square_width) + randi(square_width), square_width) + 1;  
    %   fprintf(cur_tiff_file_path);
    %  fprintf('\n');
      png_file_path = sprintf('%s/EvapInd%dCondInd%d.png', output_path, evap_index, cond_index);
        
      cur_max_val = max_val(cond_index, evap_index);
      cur_max_ind = max_ind(cond_index, evap_index);
      
      if(cur_max_val == 0)
         im_to_print = double(stable_image); 
      else
          im_to_print = my_image_list{cur_max_ind};     
          im_to_print = .15 + .7 * double(im_to_print(trans_ind, trans_ind, :));
      end

     
      if(cur_max_val>0 && cur_max_ind >2)
         fprintf('Low Length scale stuff! Cur max val is %f, Ind is %d, \n',cur_max_val, cur_max_ind); 
         im_to_print;
 %        imshow(im_to_print);
%         pause;
      end      
      im_print_size = size(im_to_print);   
      imwrite(im_to_print, png_file_path);       
   end    
end
fprintf('Finished \n');
