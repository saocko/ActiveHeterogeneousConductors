function [evap_rate] = CartoonMeanChanEvapRate(density, meanflow, evap_coeff, taxis_mode)

%fprintf('At CartoonChanMeanEvapRate \n');

filled_resistance = 20;
current_through_column = 1./(density* filled_resistance + (1-density));

voltage_through_filled = (filled_resistance) * current_through_column;



if(taxis_mode ==1)
    mean_field_evap_rate = exp(voltage_through_filled * -evap_coeff);
elseif(taxis_mode ==2)
    mean_field_evap_rate = (voltage_through_filled * meanflow).^(-evap_coeff);
end



evap_rate = mean_field_evap_rate * density;