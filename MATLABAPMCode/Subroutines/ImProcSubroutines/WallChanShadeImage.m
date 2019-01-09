function[new_image] = WallChanShadeImage(old_image, wall_instab, chan_instab);

shade_amount = .6;
new_image = old_image;

if(chan_instab)
   new_image(:, :, 1:2) = new_image(:, :, 1:2) * shade_amount; 
end
if(wall_instab)
   new_image(:, :, 2:3) = new_image(:, :, 2:3) * shade_amount; 
end


