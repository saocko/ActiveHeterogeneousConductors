function[] = SaveEps(file_path_string)
[pathstr name ext] = fileparts(file_path_string);

fig_string = [pathstr '/' name '.eps'];

saveas(1, fig_string);

