function[system_list, sys_size] = LoadPlotFile(file_path);
%fprintf('Loading Plot file %s \n', file_path);
%Loads a file and returns a cell full of system snapshots a particular run. 

imported_data = importdata(file_path);

width = imported_data(1,1);
height = imported_data(1, 2);

all_y_indices =  2:(height+1):(size(imported_data, 1) -height);
for i = 1:length(all_y_indices)
    y_index = all_y_indices(i);
    y_stamp = y_index:(y_index+height-1);    
    cur_sys =  imported_data(y_stamp, 1:width);    
    system_list{i} = cur_sys;
end

sys_size = size(cur_sys);
