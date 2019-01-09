function [mean_evap] = DummyWallMeanEvapRate(density, meanflow, evap_coeff, taxis_mode)

filled_resist = 20;

wall_conductivity = density/(filled_resist) + (1-density);
wall_resistivity = 1./wall_conductivity;

wall_voltage = meanflow * wall_resistivity;


if(taxis_mode ==1)
    mean_field_evap_rate = exp(wall_voltage  * -evap_coeff);
elseif(taxis_mode ==2)
    mean_field_evap_rate = (wall_voltage).^(-evap_coeff);    
end

mean_evap = density* mean_field_evap_rate;












