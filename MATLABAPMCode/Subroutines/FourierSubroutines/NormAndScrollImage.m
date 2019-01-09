function[new_image] = NormAndScrollImage(old_image)
%A Helper method for the FFT and TPC such that the zero wavevector is displayed in the middle rather than in the corner. 


sys_size = size(old_image);

y_scroll_array = (round(sys_size(1)/2)) + (1:sys_size(1));
y_scroll_array = mod(y_scroll_array-1, sys_size(1)) + 1;

x_scroll_array = (round(sys_size(2)/2)) + (1:sys_size(2));
x_scroll_array = mod(x_scroll_array-1, sys_size(2)) + 1;


scrolled_image = old_image(y_scroll_array, x_scroll_array);

new_image = (scrolled_image - min(scrolled_image(:)))/(range(scrolled_image(:)));
