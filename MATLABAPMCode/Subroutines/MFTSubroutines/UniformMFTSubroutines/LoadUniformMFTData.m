function[dens_info, mean_perm filled_volt_list, empty_volt_list, densities] = LoadUniformMFTData(mean_dens)


densities = MakeHorizontal( importdata(sprintf('MFTSamples/Uniform/DensValues.txt')));

perm_list = cell(length(densities));
empty_volt_list = cell(length(densities));
filled_volt_list =cell(length(densities));

last_length = 0;
for dens_ind = 1:length(densities)
    
    cur_dens = densities(dens_ind);
    
    perm_txt_path = sprintf('MFTSamples/Uniform/PermDens%.4f.txt', cur_dens);
    perm_list{dens_ind} = importdata(perm_txt_path);
    mean_perm(dens_ind)= mean(perm_list{dens_ind});
    empty_volt_list{dens_ind} = importdata(sprintf('MFTSamples/Uniform/EmptyVoltageDens%.4f.txt', cur_dens));
    filled_volt_list{dens_ind} = importdata(sprintf('MFTSamples/Uniform/FilledVoltageDens%.4f.txt', cur_dens));    
%    last_length = updatePrint(sprintf('Loading MFT data for density %.4f', cur_dens), last_length);
end
%last_length = 0 * updatePrint('Finished Loading MFT Data... \n', last_length);


dens_info.values = densities;
dens_info.pad = [0 1];
dens_info.base = mean_dens;
[my_min, my_argmin] = min(abs(dens_info.base - dens_info.values));
dens_info.base_index = my_argmin;

%fprintf('Base Index is %d, density at that index is %f \n', dens_info.base_index, dens_info.values(dens_info.base_index));
