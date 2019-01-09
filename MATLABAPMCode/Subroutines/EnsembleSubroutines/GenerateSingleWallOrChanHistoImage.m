function[is_bimodal] = GenerateSingeWallOrChanHistoImage(input_file_path, n_sweeps, output_file_path, is_wall)

close all;
input_file_path;

dummy_input_file_path = sprintf(input_file_path, 0);
[system_list, sys_size] =  LoadPlotFile(dummy_input_file_path);


%fprintf('System size \n');
%sys_size

if(is_wall)
    sum_ind = 2;
    histo_length = sys_size(2) +1;
    total_hist = zeros(histo_length, 1);
    hist_ref_array = -1 + (1:histo_length);
    %    fprintf('IsWall, histo_length is %d \n', histo_length);
else
    sum_ind = 1;
    histo_length = sys_size(1) +1;
    total_hist = zeros(histo_length, 1)';
    hist_ref_array = -1 + (1:histo_length);
    %    fprintf('IsChan, histo_length is %d \n', histo_length);
end

total_hist_squared = total_hist;

%Calculates the mean and standard deviation for times measured. 
for sweep_num  = 0:(n_sweeps - 1)
    cur_sweep_file_path = sprintf(input_file_path, sweep_num);
    [system_list, sys_size] =  LoadPlotFile(cur_sweep_file_path);
%    fprintf('At GenerateSingeWallOrChanHistoImage, Doing sweep %d out of %d \n', sweep_num, n_sweeps);
    
    
    hist_for_this_sweep = zeros(size(total_hist));
    for i = 1:length(system_list)
        image = system_list{i}>1;
        summed_image = sum(image, sum_ind);
        hist_for_this_sweep = hist_for_this_sweep + histc(summed_image, hist_ref_array);
    end
    total_hist = total_hist + hist_for_this_sweep;
    total_hist_squared = total_hist_squared + hist_for_this_sweep.^2;
end
mean_hist = total_hist/n_sweeps;
mean_hist_sqared = total_hist_squared/n_sweeps;
%fprintf('\n \n');
std_dev = sqrt(mean_hist_sqared - mean_hist.^2)/sqrt(n_sweeps);

error_bar_for_bimodal = std_dev * n_sweeps;


%Does a whole lot of plotting and formatting stuff


non_zero_points = mean_hist > 0;
my_bar_hand = bar(hist_ref_array, mean_hist, 'BarWidth', .95, 'FaceColor', [.2 .2 .2]);

hold on;
my_err_hand = errorbar(hist_ref_array(non_zero_points), mean_hist(non_zero_points), std_dev(non_zero_points), '.');
set(my_err_hand, 'Color', [0 0 0]);

set(gca, 'YTick', [])
set(gca, 'XTick', [])
set(gca, 'XLim', [0-.6 (histo_length+.6)]);
set(gca, 'YLim', [0 1.2 * max(mean_hist(:))]);


set(gca, 'Position', get(gca, 'OuterPosition') - ...
    get(gca, 'TightInset') * [-1 0 1 0; 0 -1 0 1; 0 0 1 0; 0 0 0 1]);


%title(sprintf('%s, iswall %d ', file_path, is_wall));

hist_to_save.vals = total_hist;
hist_to_save.error_bars = error_bar_for_bimodal;

%Saves the output to file
if(length(output_file_path)>0)
    
    SaveMat(output_file_path, hist_to_save);
    MakeFilePath(output_file_path);
    saveas(1, output_file_path);
    loadCropAndWrite(output_file_path);
    SaveFig(output_file_path);
    SaveEps(output_file_path);
end

%returns whether the resultant histogram is bimodal
is_bimodal = isBimodal(total_hist, error_bar_for_bimodal);

