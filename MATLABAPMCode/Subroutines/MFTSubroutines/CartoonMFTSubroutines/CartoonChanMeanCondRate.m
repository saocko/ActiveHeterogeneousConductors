function [cond_rate] = CartoonChanMeanCondRate(density, meanflow, cond_coeff, taxis_mode)


%fprintf('At CartoonChanMeanCondRate \n');

filled_resistance = 20;
current_through_column = 1./(density* filled_resistance + (1-density));

voltage_through_empty = current_through_column;





if(taxis_mode ==1)
    mean_field_cond_rate = exp(voltage_through_empty * cond_coeff);
elseif(taxis_mode ==2)
    mean_field_cond_rate = (voltage_through_empty * meanflow).^(cond_coeff);
end



cond_rate = mean_field_cond_rate * (1-density);