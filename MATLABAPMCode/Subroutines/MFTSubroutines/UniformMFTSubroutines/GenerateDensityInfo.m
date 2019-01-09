
function [dens_info] = GenerateDensityInfo(mean_dens)

dens_info.values = importdata('Histo/Uniform/DensValues.txt');
dens_info.base = mean_dens;
dens_info.pad(1) = 0;
dens_info.pad(2) = 1;