function[fft_image, max_weight, fft_error_bar] = GenerateSingleFFTImage(file_path, n_sweeps)



[system_list, sys_size] =  LoadPlotFile(sprintf(file_path, 0));%Loads a file to get the system size.

total_fft_image = zeros(sys_size);
total_fft_image_squared = zeros(sys_size);


for sweep_num = 0:(n_sweeps-1)
    
%    fprintf('At GenerateSingleFFTImage, sweep is %d \n', sweep_num);
    [system_list, sys_size] =  LoadPlotFile(sprintf(file_path, sweep_num));    
%    fprintf('%s \n', sprintf(file_path, sweep_num));
    total_fft_image_for_this_sweep = zeros(sys_size);    
    for i = 1:length(system_list)
        fft_image = abs(fft2(system_list{i}>1)).^2;
        
        
%        image = system_list{i}>1;
        
        total_fft_image_for_this_sweep = total_fft_image_for_this_sweep + fft_image;

    end
    
    total_fft_image = total_fft_image + total_fft_image_for_this_sweep;
    total_fft_image_squared = total_fft_image_squared + total_fft_image_for_this_sweep.^2;
end

%Calculates the standard deviation
mean_sweep_fft_im = total_fft_image/n_sweeps;
mean_sweep_fft_im_squared = total_fft_image_squared/n_sweeps;
std_dev = sqrt(mean_sweep_fft_im_squared - mean_sweep_fft_im.^2)/sqrt(n_sweeps);


fft_image = mean_sweep_fft_im/(length(system_list) * prod(sys_size));
fft_image(1,1) = 0;
max_weight = max(fft_image(:));
fft_error_bar = std_dev/(length(system_list) * prod(sys_size));


%imshow(resized_scrolled_total_fft);
