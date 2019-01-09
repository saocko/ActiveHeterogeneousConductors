//
//  VoltageTimesAlphaCellTypeLocalCalc.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "VoltageTimesAlphaCellTypeLocalCalc.h"
#import "MyAPMSystem.h"
#import "CppHelper.h"

VoltageTimesAlphaCellTypeLocalCalc::VoltageTimesAlphaCellTypeLocalCalc(MyAPMSystem * sys_, double alpha_, MySysCellType cell_, My2Vec posit_, double sigma_): VoltageTimesAlphaCellTypeCalc(sys_, alpha_, cell_){
    sigma = sigma_;
    posit = posit_;
}




double VoltageTimesAlphaCellTypeLocalCalc::calcValue(int x, int y){
    double x_dist_squared =  square(system()->width());
        for (int iX = -5; iX<5; iX++) {
            x_dist_squared = fmin(x_dist_squared, square(x - (posit.x + iX * system()->width())));
        }
    
        double y_dist_squared =  square(system()->width());
        for (int iY = -5; iY<5; iY++) {
            y_dist_squared = fmin(y_dist_squared, square(y - (posit.y + iY * system()->height())));
        }    
    
    return VoltageTimesAlphaCellTypeCalc::calcValue(x, y) - (x_dist_squared + y_dist_squared)/(2 * sigma* sigma);
}