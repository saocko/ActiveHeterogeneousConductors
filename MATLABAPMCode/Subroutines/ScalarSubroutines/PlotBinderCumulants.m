function[] = PlotBinderCumulants(sys_params, input_path, file_path_to_print)
close all;

figure(1);
cumulants = LoadHistogramsAndCalculateBinderCumulants(sys_params, input_path);

max_cum = max(cumulants(:));
min_cum = min(cumulants(:));


cum_step_array = min_cum * exp(log(max_cum/min_cum) * 0:.1:1);


[x_mesh, y_mesh] = meshgrid(sys_params.evap, sys_params.cond);



%Do this because the eps thing is buggy
[expanded_evap_windtax, expanded_cond_windtax, expanded_cum] = expandImage(sys_params.evap, sys_params.cond, cumulants, 55);

imagesc(expanded_evap_windtax, expanded_cond_windtax, expanded_cum);

colormap('bone');

temp_cbarax_1 = colorbar('EastOutside', 'FontSize', 20);

%min_plot_val = min(0, min(cumulants(:)));
min_plot_val = 0;
max_plot_val = max(.1, max(cumulants(:)));

set(gca, 'CLim', [min_plot_val, max_plot_val]);
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
title(sprintf('Contour of cumulants for %s = %.02f', dens_string, sys_params.mean_dens), 'FontSize', 21);

%title(sprintf('Contour of cumulants for %s', run_name), 'FontSize', 14);
%set(h,'YScale','log');
fprintf('Printing cumulants Plot to %s.eps \n', file_path_to_print)
MakeFilePath(file_path_to_print);
%print('-depsc', '-r300', sprintf('%s.eps', file_path_to_print));

print('-dpng', '-r300', sprintf('%s.png', file_path_to_print));

eps_path = sprintf('%s.eps', file_path_to_print);
eps_path;
print('-deps', '-r300', eps_path);

%print2eps(eps_path, 1);


