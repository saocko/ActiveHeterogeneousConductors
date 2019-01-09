function [new_x, new_y new_im]=expandImage(old_x, old_y, old_im, expand_factor);


new_im = imresize(old_im, expand_factor, 'nearest');
new_x = min(old_x) + ((1:(length(old_x) * expand_factor)) - 1) * range(old_x)/(length(old_x) * expand_factor - 1);
new_y = min(old_y) + ((1:(length(old_y) * expand_factor)) - 1) * range(old_y)/(length(old_y) * expand_factor - 1);

new_x;
new_y;

[ix_mesh, iy_mesh] = meshgrid(1:length(new_x), 1:length(new_y));
shouldnt_black_out = ((mod(ix_mesh, expand_factor)>1 ).* (mod(iy_mesh, expand_factor)>1 ))>0;

new_im = new_im .* shouldnt_black_out;
%my_mask = x_mesh 



