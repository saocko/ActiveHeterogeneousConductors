function[] = PlotConductivity(sys_params, input_path, file_path_to_print)
close all;

figure(1);
conductivity = LoadConductivity(sys_params, input_path);

max_conduct = max(conductivity(:));
min_conduct = min(conductivity(:));


%For now, we hardcode meximum perm to 1.
perm_step_array = min_conduct * exp(log(max_conduct/min_conduct) * 0:.1:1);
%perm_step_array = (0:.1:1) * max(conductivity(:));







[x_mesh, y_mesh] = meshgrid(sys_params.evap, sys_params.cond);


%Do this because the eps thing is buggy
[expanded_evap_windtax, expanded_cond_windtax, expanded_conduct] = expandImage(sys_params.evap, sys_params.cond, conductivity, 55);

imagesc(expanded_evap_windtax, expanded_cond_windtax, expanded_conduct);

colormap('bone');

temp_cbarax_1 = colorbar('EastOutside', 'FontSize', 20);
set(gca, 'CLim', [0 max(conductivity(:))]);
set(gca, 'XTickLabel', []);
set(gca, 'YTickLabel', []);
set(gca, 'XTick', []);
set(gca, 'YTick', []);


label_font_size = 18;
ylabel(('\alpha_{A}'), 'FontSize', label_font_size);
xlabel(('\alpha_{R}'), 'FontSize', label_font_size);
ylabel(('\alpha_{A}'), 'FontSize', label_font_size);
xlabel(('\alpha_{R}'), 'FontSize', label_font_size);



dens_string = '\rho';
title(sprintf('Contour of conductivity for %s = %.02f', dens_string, sys_params.mean_dens), 'FontSize', 21);

%title(sprintf('Contour of conductivity for %s', run_name), 'FontSize', 14);
%set(h,'YScale','log');
fprintf('Printing conductivity Plot to %s.eps \n', file_path_to_print)
MakeFilePath(file_path_to_print);
%print('-depsc', '-r300', sprintf('%s.eps', file_path_to_print));

print('-dpng', '-r300', sprintf('%s.png', file_path_to_print));

eps_path = sprintf('%s.eps', file_path_to_print);
eps_path;
print('-deps', '-r300', eps_path);

%print2eps(eps_path, 1);


