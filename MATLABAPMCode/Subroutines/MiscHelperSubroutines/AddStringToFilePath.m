function[str_to_return] = AddStringToFilePath(file_path_string, string_to_add)
[pathstr name ext] = fileparts(file_path_string);

if(length(pathstr) > 0)
   pathstr = [pathstr '/'];
end

str_to_return = [pathstr name string_to_add ext];



%foo = fileparts(file_path_string);
%fprintf('Making file path for %s \n', foo);