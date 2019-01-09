//
//  MyCircuitCell.h
//  MyAPM
//
//  Created by Samuel Ocko on 6/24/11.
//  Copyright 2011 MIT. All rights reserved.
//

struct MyCircuitCell {
    double up_cond;
    double right_cond;
    
    double base_in_plane_divergence;

    
    double leak_cond;
    double leak_voltage;
    
    bool has_fixed_voltage;
    double fixed_voltage;
    
    
    double solved_voltage;
    
    
    double up_current;
    double right_current;
    
    double up_driv_voltage;
    double right_driv_voltage;
    
};

inline MyCircuitCell zeroNonConCell(){        
    MyCircuitCell c;
        //Makes a nonconducting cell. 
    
    c.up_cond = 0;
    c.right_cond = 0;
    c.base_in_plane_divergence = 0;
    c.leak_cond = 0;
    c.leak_voltage = 0;
    c.has_fixed_voltage = false;
    c.fixed_voltage = 0;    
    return c;
}
