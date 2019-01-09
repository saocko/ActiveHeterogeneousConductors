function[is_bimodal] = isBimodal(vector, error_bars)


%Determines whether a histogram is bimodal, and which peaks are statistically significant.
vector = MakeHorizontal(vector)';
error_bars = MakeHorizontal(error_bars)';

%First gets a list of all peaks
for i = 1:length(vector)
    left_larger = false;
    if(i>1)
        left_larger = vector(i-1) > vector(i);
    end
    
    right_larger = false;
    
    if(i< length(vector))
        right_larger = vector(i+1) > vector(i);
    end
    
    
    
    is_peak(i) = ~or(left_larger, right_larger);    
end

all_peaks = find(is_peak);


my_thresh = 3; %The threshold for statistical significance is 3 standard deviations


is_true_peak = zeros(size(is_peak));

for i = 1:length(vector)
    
    
    if(is_peak(i))
       % fprintf('\n Doing peak %d \n', i);
        
       in_accessible = (vector(i) - my_thresh *sqrt(error_bars(i).^2 + error_bars.^2)) > vector;
       accessible = ~in_accessible;
       reachable_region = boolean(zeros(size(in_accessible)));
       reachable_region(i) = true;
       
       j = i;
       while(j <= length(vector) && accessible(j))
           reachable_region(j) = true;
           j = j+1;
       end
       j = i;
       while(j >= 1 && accessible(j))
           reachable_region(j) = true;
           j = j-1;
       end       
       
       reachable_region;       
       vector(reachable_region);
       biggest_reachable_peak = max(vector(reachable_region));
       
       
       is_true_peak(i) = vector(i) == biggest_reachable_peak;
       
        peak_is_true = is_true_peak(i);
        
        if(vector(i)>0)
          % fprintf('Peak val at %d is %d, error bar %d, trueness %d \n', i,  vector(i), error_bars(i), peak_is_true);
        end
       %Fails for adjancent exactly equal peaks, but this doesn't happen in practice.               
    end        
end

true_peaks = find(is_true_peak);
is_bimodal = sum(is_true_peak) > 1;
