//
//  GraphicsHelper.h
//  MyAPM
//
//  Created by Samuel Ocko on 6/27/11.
//  Copyright 2011 MIT. All rights reserved.
//


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#import "MySysCellType.h"
#import "MySystemCell.h"
#import "CppHelper.h"
using namespace std;

//Just a few helper functions to implement size curves.


inline NSColor* sysCellToColor(MySystemCell cell){
    if(1){
        switch (cell.cell_type ) {
            case AccessEmptyCell:            // Note the colon, not a semicolon
                return [NSColor colorWithDeviceWhite:0 alpha:0]; 
                break;
            case InAcessEmptyCell:            // Note the colon, not a semicolon
                return [NSColor colorWithDeviceCyan:1 magenta:0 yellow:0 black:0 alpha:.2];
                break;
            case ParticleCell:            // Note the colon, not a semicolon
                return [NSColor colorWithDeviceWhite:0 alpha:.55]; 
                break;
            case SubstrateCell:            // Note the colon, not a semicolon
                return [NSColor colorWithDeviceRed:.5 green:.25 blue:.1 alpha:.8];
                break;
            default:            // Note the colon, not a semicolon
                
                char error_mess[500];
                sprintf(error_mess, "In sysCellToColor, couldn't match %d", (int)cell.cell_type);
                MyAssert(false, error_mess);
                return 0;
        }
    }
    else{
        return [NSColor colorWithDeviceWhite:0 alpha:.3/cell.traveledness];
//        return [NSColor blackColor];
    }
    
    
    /*    
     switch ( c ) {
     case AccessEmpty:            // Note the colon, not a semicolon
     return [NSColor colorWithDeviceWhite:0 alpha:0]; 
     break;
     case InAcessEmpty:            // Note the colon, not a semicolon
     return [NSColor colorWithDeviceCyan:1 magenta:0 yellow:0 black:0 alpha:.2];
     break;
     case ParticleCell:            // Note the colon, not a semicolon
     return [NSColor colorWithDeviceWhite:0 alpha:.55]; 
     break;
     case Substrate:            // Note the colon, not a semicolon
     return [NSColor colorWithDeviceRed:.5 green:.25 blue:.1 alpha:.8];
     break;
     default:            // Note the colon, not a semicolon
     MyAssert(false, "In sysCellToColor, couldn't match");
     return 0;
     }
     */
}


inline double arrowSize(double current, double meancurrent){
    
    double my_weight=.07;
    
    
    if(1){
        meancurrent = 1;//Do this for Display Purposes.
        my_weight = .22;
    }
    
    double unsigned_current = fabs(current);
    double unsigned_mean_current = fabs(meancurrent);
    
    
    
    
    
    //printf("Unsigned current is %f \n", unsigned_current);
    double unsigned_arrow_size = sqrt(my_weight * unsigned_current/unsigned_mean_current);
    double trunc_size = min(unsigned_arrow_size, .75);
    double cur_sign = (current >0) -(current<0);
    return cur_sign * trunc_size;
}
