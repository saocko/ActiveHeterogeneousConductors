//
//  VoltageTimesAlphaCalculator.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//


#import "MySystemCalculator.h"

class VoltageTimesAlphaCalculator:public MySystemCalculator{
    
    double alpha;
    
    
    
public:
    VoltageTimesAlphaCalculator(MyAPMSystem * sys_, double alpha_);    
    virtual double calcValue(int x, int y);
    double getAlpha();
};

