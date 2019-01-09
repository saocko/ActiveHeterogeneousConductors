function[shading_array] = WhiteShadingArray(sys_params)

shading_array =  ones(length(sys_params.cond), length(sys_params.evap), 3);
