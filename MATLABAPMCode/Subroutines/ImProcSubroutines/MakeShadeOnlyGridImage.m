function[phase_picture] = MakeShadeOnlyGridImage(sys_params, shade_image, output_file_path)

fprintf('At MakeShadeOnlyGridImage... ');

square_width = round(800/max(length(sys_params.evap), length(sys_params.cond)));
in_between_padding = round(square_width/8);

individual_image_size = [square_width square_width];

grid_image_size = [individual_image_size(1)*length(sys_params.cond) + in_between_padding*(length(sys_params.cond) + 1), ...
    individual_image_size(2)*length(sys_params.evap) + in_between_padding*(length(sys_params.evap) + 1), 3];



grid_image_size;
my_image = (zeros(grid_image_size));



for evap_ind = 1:length(sys_params.evap)
    for cond_ind = 1:length(sys_params.cond)
        
        %X is evap, y is condens
        y_posit = in_between_padding * cond_ind + individual_image_size(1) * (cond_ind -1);
        x_posit = in_between_padding * evap_ind + individual_image_size(2) * (evap_ind -1);
        
        my_y_range = y_posit + (1:individual_image_size(1));
        my_x_range = x_posit + (1:individual_image_size(2));
                
        cur_image = (ones([individual_image_size(1), individual_image_size(2), 3]));
           
        for color_ind = 1:3
           cur_image (:, :, color_ind) = shade_image(cond_ind, evap_ind, color_ind);            
        end        
        my_image(my_y_range, my_x_range, :) = cur_image;
    end
end


if(length(output_file_path)>0)
    MakeFilePath(output_file_path);
    imwrite(my_image, output_file_path);
end

phase_picture = my_image;
fprintf('Finished \n');
