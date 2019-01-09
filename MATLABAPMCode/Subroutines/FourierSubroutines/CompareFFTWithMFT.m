function[] = CompareFFTWithMFT(fft_data, fft_err_bar, plot_title_path, MFT_fluct)
close all;
%Compares the low-wavevector amplitudes of the FFT with 



sys_size = size(fft_data);

[ikx, iky] = meshgrid((1:sys_size(2)) -1,(1:sys_size(2)) -1);
kx = ikx/sys_size(2);
ky = iky/sys_size(1);


k_squared = kx.^2 + ky .^2;
horizontality = kx.^2 .* 1./k_squared;
min_non_zero_ksquared = 1./(min(sys_size(:))^2);




thresh_1 = 10^-20;
thresh_2 = 12.55 * min_non_zero_ksquared;
thresh_3 = 2* thresh_2;
valid_region{1} = and(k_squared > thresh_1, k_squared <thresh_2);
valid_region{2} = and(k_squared >= thresh_2, k_squared <thresh_3);


plot_colors{1} = 'r';
plot_colors{2} = 'b';


valid_region{1};
valid_region{2};

foobar = sort(horizontality(:));

foobar1 = sort(horizontality(valid_region{1}));
foobar2 = sort(horizontality(valid_region{2}));


for i=1:2
    
    
    fft_amps = fft_data(valid_region{i});
    horizontality_values = horizontality(valid_region{i});
    error_bars = fft_err_bar(valid_region{i});
    
    valid_region;
    fft_amps;
    horizontality_values;
    
    handle = errorbar(horizontality_values, fft_amps, error_bars, '.', 'Color', plot_colors{i}); %new
    set(handle, 'Marker', 'none');
    set(handle, 'LineWidth', 2.5);
%    set(h, 'LineStyle', );
    
%    scatter(horizontality_values, fft_amps, 60, plot_colors{i}, 'filled'); %Original
%    pause;
    hold on;
end
set(gca, 'XLim', [0 1]);

hold on;

x_values = 0:.001:1;
one_over_y_values = 1./(MFT_fluct.wall) + (1/MFT_fluct.chan - 1/MFT_fluct.wall) * x_values;

y_values = 1./one_over_y_values;
plot(x_values, y_values, 'k', 'LineWidth', 2.5);


%Saves the plot to file
if(length(plot_title_path)> 0)
    MakeFilePath(plot_title_path);
    saveas(1, sprintf('%s.eps', plot_title_path));
    saveas(1, sprintf('%s.png', plot_title_path));    
    SaveFig(plot_title_path);
end