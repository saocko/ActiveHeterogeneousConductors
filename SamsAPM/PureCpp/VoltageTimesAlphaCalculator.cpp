//
//  VoltageTimesAlphaCalculator.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//


#import "VoltageTimesAlphaCalculator.h"
#import "MyAPMSystem.h"


VoltageTimesAlphaCalculator::VoltageTimesAlphaCalculator(MyAPMSystem * sys_, double alpha_): MySystemCalculator(sys_){
    alpha = alpha_;
    
}


double VoltageTimesAlphaCalculator::getAlpha(){
    return alpha;
}

double VoltageTimesAlphaCalculator::calcValue(int x, int y){
    //system();
    
    return system()->voltageThroughNode(x, y) * alpha;
//    return 0;
}
