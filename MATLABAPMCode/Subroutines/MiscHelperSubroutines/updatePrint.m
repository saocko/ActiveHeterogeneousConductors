function [last_length] = updatePrint(string_to_print, lastlength);
%Helper method for plotting progress
for j = 1:lastlength
   fprintf('\b') 
end
fprintf(string_to_print)

last_length = length(string_to_print);







