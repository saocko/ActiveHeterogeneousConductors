function [mean_evap, max_frac] = MeanEvapRate(density, meanflow, evap_coeff, volt_samples, taxis_mode)


if(taxis_mode ==1)
    all_evap_rates = exp(volt_samples * meanflow * -evap_coeff);
elseif(taxis_mode ==2)
    all_evap_rates = (volt_samples * meanflow).^(-evap_coeff);
end
%Positive evapcoeff means that you DONT evaporate in high flow

mean_evap_per_space = mean(all_evap_rates);

mean_evap = density * mean_evap_per_space;
max_frac = max(all_evap_rates)/sum(all_evap_rates);
%fprintf('Finished meanevaprate \n');








