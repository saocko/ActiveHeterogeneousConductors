//
//  IsCellTypeCalculator.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//
#import "MySystemCalculator.h"
#import "VoltageTimesAlphaCalculator.h"


class IsCellTypeCalculator: public MySystemCalculator{
    IsCellTypeCalculator(MyAPMSystem * sys_, MySysCellType cell_);
    
    MySysCellType cell;
    
    
public:
    virtual bool shouldCount(int x, int y);
    virtual double calcValue(int x, int y);    
    
};

