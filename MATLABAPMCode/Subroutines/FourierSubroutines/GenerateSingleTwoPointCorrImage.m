function[two_point_corr_image] = GenerateSingleTwoPointCorrImage(file_path, n_sweeps)

fprintf('At GenerateSingleTwoPointCorrImage! \n');

%Loads it first just to find out the size. A bit computationally waseful, but thats ok 
[dummy_system_list, dummy_sys_size] =  LoadPlotFile(sprintf(file_path, 0));

total_fft_image = zeros(dummy_sys_size);
total_fft_image_squared = zeros(dummy_sys_size);


for sweep_num = 0:(n_sweeps-1)
    [system_list, sys_size] =  LoadPlotFile(sprintf(file_path, sweep_num));    
  %  fprintf('%s \n', sprintf(file_path, sweep_num));
    total_fft_image_for_this_sweep = zeros(sys_size);    
    for i = 1:length(system_list)
        fft_image = abs(fft2(system_list{i}>1)).^2;
        
        
        total_fft_image_for_this_sweep = total_fft_image_for_this_sweep + fft_image;

    end
    
    total_fft_image = total_fft_image + total_fft_image_for_this_sweep;
    total_fft_image_squared = total_fft_image_squared + total_fft_image_for_this_sweep.^2;
end
%fprintf('\n');

total_fft_image = total_fft_image/(length(system_list) * n_sweeps);

%Now that we have the FFT Image, we transform it back to get the TPC

two_point_correlation = fft2(total_fft_image)/(prod(sys_size).^2);%Need to do this for normalization purposes


image = system_list{1} > 1;
mean_density = mean(image(:));
expected_zero_corr = mean_density * (1-mean_density)^2 + (1-mean_density)*mean_density^2;
actual_zero_corr = two_point_correlation(1,1);
two_point_corr_image = two_point_correlation;
