
function [is_stable free_en_diff] = CrystalPhaseSeparationPlot(skew_info, cond_evap_ratios, plot_title_path)

close all;
figure(1);


%Note, this looks first for the max discretized accum and looks locally from there, there might be a bit of a problem, but if it's
%that close it's not too big of a deal. 

%title(sprintf('Cond Rate/Evap Rate vs density for (evap, cond) = (%.3f, %.3f) ', evap_coeff, cond_coeff));


curve_line_width = 2;
phase_sep_line_width = 2;
ax_font_size = 16;
phase_sep_curve_col = 'k';




log_ratio_list = log(cond_evap_ratios);
skew_diff_list = diff(skew_info.values);

%Makes the Accum Array
accum_array = zeros(size(log_ratio_list));
for i = 1:(length(accum_array) -1)
    accum_array(i+1) = accum_array(i) + skew_diff_list(i)* .5 * (log_ratio_list(i) + log_ratio_list(i+1));
    free_en_diff = 0;
end
accum_array = accum_array - accum_array(skew_info.unskewed_index);
%Finished making Accum Array


semilogy(skew_info.values, cond_evap_ratios, 'LineWidth', curve_line_width, 'Color', 'k');
set(gca, 'XLim', skew_info.pad);
set(gca, 'YLim', [min(cond_evap_ratios) max(cond_evap_ratios)]);


if(max(accum_array)<=0)
    is_stable = 1;
else
    
    
    %Finds the lowest free energy index. 
    [max_accum, max_accum_ind] = max(accum_array);
    max_accum_skew = skew_info.values(max_accum_ind);
        
    
    %Sees what the best value is between the indices by doing a simple linear equation.
    if(max_accum_ind ==1)
        best_left_skew = max_accum_skew;
        best_left_skew_accum = max_accum;
    else
        best_left_skew_coeff = log_ratio_list(max_accum_ind)/(log_ratio_list(max_accum_ind) - log_ratio_list(max_accum_ind-1));
        best_left_skew_coeff = max(best_left_skew_coeff, 0);
        
        left_skew_diff = skew_info.values(max_accum_ind) - skew_info.values(max_accum_ind-1);
        best_left_skew = skew_info.values(max_accum_ind)*(1 - best_left_skew_coeff)  + skew_info.values(max_accum_ind -1) * best_left_skew_coeff;
        best_left_skew_accum = max_accum + .5 * best_left_skew_coeff * -left_skew_diff * log_ratio_list(max_accum_ind);
    end
    
    if(max_accum_ind ==length(skew_info.values))
        best_right_skew = max_accum_skew;
        best_right_skew_accum = max_accum;
    else
        right_skew_diff = skew_info.values(max_accum_ind+1) - skew_info.values(max_accum_ind);        
        best_right_skew_coeff = log_ratio_list(max_accum_ind)/(log_ratio_list(max_accum_ind) - log_ratio_list(max_accum_ind+1));
        best_right_skew_coeff = max(best_right_skew_coeff, 0);
        
        best_right_skew = skew_info.values(max_accum_ind)*(1 - best_right_skew_coeff)  + skew_info.values(max_accum_ind +1) * best_right_skew_coeff;
        best_right_skew_accum = max_accum + .5 * best_right_skew_coeff * right_skew_diff * log_ratio_list(max_accum_ind);
    end    
    
    if(best_left_skew_accum> best_right_skew_accum)
        best_skew = best_left_skew;
        best_skew_accum = best_left_skew_accum;
    else
        best_skew = best_right_skew;
        best_skew_accum = best_right_skew_accum;
    end    
    %Done interpolating
        
    
    
        
        
    free_en_diff =  best_skew_accum - accum_array(skew_info.unskewed_index);
    
    hold on;
    semilogy([0 best_skew], [1 1], 'Color', 'r', 'LineWidth', curve_line_width);
%   filledCircle([0, 1],.05,1000,'r');
    hold on;
    semilogy(0, 1,'o', 'Color', 'r', 'MarkerSize', 13, 'MarkerFaceColor', 'r');
    is_stable = 0;
end

%Outputs to file
if(length(plot_title_path)> 0)
    MakeFilePath(plot_title_path);
    saveas(1, sprintf('%s.eps', plot_title_path));
    saveas(1, sprintf('%s.png', plot_title_path));
end
