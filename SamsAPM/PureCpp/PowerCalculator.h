//
//  PowerCalculator.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//
#import "MySystemCalculator.h"
#import "MyAPMSystem.h"

class PowerCalculator:public MySystemCalculator{
    
    double alpha;
    
    
    
public:
    PowerCalculator(MyAPMSystem * sys_, double alpha_);    
    virtual double calcValue(int x, int y);
};

