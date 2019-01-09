function [] = loadCropAndWrite(file_path);
loaded_image = imread(file_path);
%As a trick I load and crop an image to get rid of the borders, and then write over the original image with the cropped image.

not_white = rgb2gray(loaded_image) ~= 255;
[im_x_mesh, im_y_mesh] = meshgrid(1:size(not_white, 2), 1:size(not_white, 1));

min_x = min(min(im_x_mesh(not_white)));
max_x = max(max(im_x_mesh(not_white)));

min_y = min(min(im_y_mesh(not_white)));
max_y = max(max(im_y_mesh(not_white)));

cropped_image = loaded_image(min_y:max_y, min_x:max_x, :);
imwrite(cropped_image, file_path);




