function [ouput_im] = CatIms(first_im_path, sec_im_path, output_im_path, is_vertical)

if(is_vertical)
    output_im = [imread(first_im_path); imread(sec_im_path)];    
else
    output_im = [imread(first_im_path) imread(sec_im_path)];
end

if(length(output_im_path) >0)        
    
    MakeFilePath(output_im_path)
    imwrite(output_im, output_im_path);
end
