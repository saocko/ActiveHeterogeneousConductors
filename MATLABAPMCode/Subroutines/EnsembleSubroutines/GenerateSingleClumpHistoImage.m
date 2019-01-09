function[is_bimodal] = GenerateSingleClumpHistoImage(input_file_path, n_sweeps, output_file_path)

close all;

dummy_input_file_path = sprintf(input_file_path, 0);%Load data to find the system size
[system_list, sys_size] =  LoadPlotFile(dummy_input_file_path);


%fprintf('Using handmade filter \n');

my_filter = ones(5, 5);
my_filter([1 5], [1 5]) = 0;


histo_length = sum(my_filter(:)) + 1; %All the possible densities a single cell can have
total_hist = zeros(histo_length, 1)'; %We'll add to this histogram as we go
hist_ref_array = -1 + (1:histo_length);
total_hist_squared = total_hist; %Used for determining standard deviation and statistical significance



for sweep_num  = 0:(n_sweeps - 1)
    cur_sweep_file_path = sprintf(input_file_path, sweep_num);
    [system_list, sys_size] =  LoadPlotFile(cur_sweep_file_path);
    %   fprintf('%s \n', cur_sweep_file_path);
    %fprintf('At GenerateSingleClumpHistoImage, Doing sweep %d out of %d \n', sweep_num, n_sweeps);
    hist_for_this_sweep = zeros(size(total_hist));
    for i = 1:length(system_list)        
        %For each snapshot in a sweep, filter the image of particle densities and add it to the total histgram
        image = uint8(system_list{i}>1);
        filtered_image = imfilter(image, my_filter, 'circular'); 
        filtered_image;        
        hist_to_add = histc(filtered_image(:), hist_ref_array)';
%        whos        
        hist_for_this_sweep = hist_for_this_sweep + hist_to_add;        
    end
                
    total_hist = total_hist + hist_for_this_sweep;
    total_hist_squared = total_hist_squared + hist_for_this_sweep.^2;
end


mean_hist = total_hist/n_sweeps;
mean_hist_sqared = total_hist_squared/n_sweeps;
%fprintf('\n \n');
std_dev = sqrt(mean_hist_sqared - mean_hist.^2)/sqrt(n_sweeps);
error_bar_for_bimodal = std_dev * n_sweeps;


%errorbar(hist_ref_array, mean_hist, std_dev);

non_zero_points = mean_hist > 0;
my_bar_hand = bar(hist_ref_array, mean_hist, 'BarWidth', .95, 'FaceColor', [.2 .2 .2]);

%pause;
%Does a whole lot of formatting of the figure. 
hold on;
my_err_hand = errorbar(hist_ref_array(non_zero_points), mean_hist(non_zero_points), std_dev(non_zero_points), '.');
set(my_err_hand, 'Color', [0 0 0]);

set(gca, 'YTick', [])
set(gca, 'XTick', [])
set(gca, 'XLim', [0-.6 (histo_length+.6)]);
set(gca, 'YLim', [0 1.2 * max(mean_hist(:))]);


set(gca, 'Position', get(gca, 'OuterPosition') - ...
    get(gca, 'TightInset') * [-1 0 1 0; 0 -1 0 1; 0 0 1 0; 0 0 0 1]);


hist_to_save.vals = total_hist;
hist_to_save.error_bars = error_bar_for_bimodal;


%Saves the output to file
if(length(output_file_path)>0)    
    SaveMat(output_file_path, hist_to_save);
    MakeFilePath(output_file_path);
    saveas(1, output_file_path);
    loadCropAndWrite(output_file_path);
    SaveFig(output_file_path);
end

%returns whether the resultant histogram is bimodal
is_bimodal = isBimodal(total_hist, error_bar_for_bimodal);
