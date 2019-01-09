function [output] = MakeHorizontal(input)

%Ensures an array is Horizontal, used to guarantee there will be no transpose issues
input_size = size(input);

if(input_size(1) == 1)
    output = input;    
else
    output = input';    
end