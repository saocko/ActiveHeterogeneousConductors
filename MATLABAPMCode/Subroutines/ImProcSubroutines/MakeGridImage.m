function[full_phase_image] = MakeGridImage(file_path_string, sys_params, shading_array, output_file_path);

%Takes a bunch of images and puts them into a big 2D Grid.  
fprintf('Making Grid Image of file %s ... ', fileName(file_path_string));


n_evap_coeffs = length(sys_params.evap);
n_cond_coeffs = length(sys_params.cond);

square_width = 200;
in_between_padding = 5;
boundary_padding_thickness = 5;

individual_image_size = [square_width square_width];

grid_image_size = [individual_image_size(1)*n_cond_coeffs + in_between_padding*(n_cond_coeffs + 1), ...
    individual_image_size(2)*n_evap_coeffs + in_between_padding*(n_evap_coeffs + 1), 3];

full_phase_image = uint8( zeros(grid_image_size));




for evap_ind = 1:n_evap_coeffs
    for cond_ind = 1:n_cond_coeffs
        cur_im_file_path = sprintf(file_path_string, evap_ind, cond_ind);
        
        y_posit = in_between_padding * cond_ind + individual_image_size(1) * (cond_ind -1);
        x_posit = in_between_padding * evap_ind + individual_image_size(2) * (evap_ind -1);
        
        my_y_range = y_posit + (1:individual_image_size(1));
        my_x_range = x_posit + (1:individual_image_size(2));
        
        
        current_image = imread(cur_im_file_path);
        current_image = imresize(current_image, individual_image_size);
        
        for color_ind = 1:3

           current_image(:, :, color_ind) =  current_image(:, :, color_ind) * shading_array(cond_ind, evap_ind, color_ind);
           
        end
        
        
        
    %    fprintf('Loading File %s \n', cur_im_file_path);        
        full_phase_image(my_y_range, my_x_range, :) = current_image;
        
        pause(.01)
        
    end
end

fprintf('Finished \n');



if(length(output_file_path)>0)
   MakeFilePath(output_file_path);
   imwrite(full_phase_image, output_file_path); 
end
