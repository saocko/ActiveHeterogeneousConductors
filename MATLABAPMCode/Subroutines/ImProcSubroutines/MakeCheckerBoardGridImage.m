function[phase_picture] = MakeCheckerBoardGridImage(sys_params, shade_image, output_file_path)

fprintf('At MakeCheckerBoardGridImage... ');



is_walled = shade_image(:, :, 1) < .99;
is_channeled = shade_image(:, :, 3) < .99;



square_width = round(800/max(length(sys_params.evap), length(sys_params.cond)));
in_between_padding = round(square_width/13);

individual_image_size = [square_width square_width];

grid_image_size = [individual_image_size(1)*length(sys_params.cond) + in_between_padding*(length(sys_params.cond) + 1), ...
    individual_image_size(2)*length(sys_params.evap) + in_between_padding*(length(sys_params.evap) + 1), 3];



grid_image_size;
my_image = (zeros(grid_image_size));


[y_grid, x_grid] = meshgrid(1:individual_image_size(1), 1:individual_image_size(2));
image_array{1}{1} = ones(individual_image_size);
image_array{2}{1} = mod(x_grid+ 03, 50) < 20;
image_array{1}{2} = mod(y_grid+ 03, 50) < 20;
image_array{2}{2} = min(image_array{2}{1} + image_array{1}{2}, 1);




for evap_ind = 1:length(sys_params.evap)
    for cond_ind = 1:length(sys_params.cond)
        
        %X is evap, y is condens
        y_posit = in_between_padding * cond_ind + individual_image_size(1) * (cond_ind -1);
        x_posit = in_between_padding * evap_ind + individual_image_size(2) * (evap_ind -1);
        
        my_y_range = y_posit + (1:individual_image_size(1));
        my_x_range = x_posit + (1:individual_image_size(2));
        
        cur_is_chan = is_channeled(cond_ind, evap_ind);
        cur_is_wall = is_walled(cond_ind, evap_ind);
        
        cur_bw_im = image_array{cur_is_chan+1}{cur_is_wall+1};
                
        cur_image = (ones([individual_image_size(1), individual_image_size(2), 3]));
           
        for color_ind = 1:3
           cur_image (:, :, color_ind) = cur_bw_im;            
        end        
        my_image(my_y_range, my_x_range, :) = cur_image;
    end
end


if(length(output_file_path)>0)
    MakeFilePath(output_file_path);
    imwrite(my_image, output_file_path);
%    imwrite(image_array{1}{1}, 'CheckerNone.png');
%    imwrite(image_array{1}{2}, 'CheckerWall.png');
%    imwrite(image_array{2}{1}, 'CheckerChan.png');
%    imwrite(image_array{2}{2}, 'CheckerBoth.png');   
end

phase_picture = my_image;
fprintf('Finished \n');
