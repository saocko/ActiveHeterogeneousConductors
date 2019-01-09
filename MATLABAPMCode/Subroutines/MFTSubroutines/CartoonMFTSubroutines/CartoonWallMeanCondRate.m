function [mean_cond] = CartoonWallMeanCondRate(density, meanflow, cond_coeff, taxis_mode)

filled_resist = 20;

wall_conductivity = density/(filled_resist) + (1-density);
wall_resistivity = 1./wall_conductivity;

wall_voltage = meanflow * wall_resistivity;


if(taxis_mode ==1)
    mean_field_cond_rate = exp(wall_voltage * cond_coeff);
elseif(taxis_mode ==2)
    mean_field_cond_rate = (wall_voltage).^cond_coeff;    
end

mean_cond = (1-density) * mean_field_cond_rate;












