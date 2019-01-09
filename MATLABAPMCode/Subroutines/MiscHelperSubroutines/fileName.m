function[string_to_return] = FileName(file_path_string)

[pathstr, name, ext] = fileparts(file_path_string);

string_to_return = sprintf('%s%s', name, ext);
