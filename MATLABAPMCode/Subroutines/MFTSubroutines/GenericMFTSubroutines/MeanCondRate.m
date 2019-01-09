function [mean_cond, max_frac] = MeanCondRate(density, meanflow, cond_coeff, volt_samples, taxis_mode)

if(taxis_mode ==1)
    all_cond_rates = exp(volt_samples * meanflow * cond_coeff);
elseif(taxis_mode ==2)
    all_cond_rates = (volt_samples * meanflow).^cond_coeff;        
end
mean_cond_per_space = mean(all_cond_rates);

mean_cond = (1-density) * mean_cond_per_space;

max_frac = max(all_cond_rates)/sum(all_cond_rates);











