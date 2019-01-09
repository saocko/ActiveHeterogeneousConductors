function[] = MakeFilePath(file_path_string)


system(sprintf('mkdir -p %s', fileparts(file_path_string)));

%foo = fileparts(file_path_string);
%fprintf('Making file path for %s \n', foo);