function[] = SaveMat(file_path_string, obj_to_save)
%Just a wrapper to save some object to a MAT file

[pathstr name ext] = fileparts(file_path_string);

output_file_string = [pathstr '/' name '.mat'];

MakeFilePath(output_file_string);
save(output_file_string, 'obj_to_save');

