//
//  VoltageTimesAlphaCellTypeLocal.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/24/12.
//  Copyright 2012 MIT. All rights reserved.
//

#import "VoltageTimesAlphaCellTypeCalc.h"
#import "My2Vec.h"


class LocalLogVoltageToPowerCalculator: public VoltageTimesAlphaCellTypeCalc{
    
    My2Vec posit;
    double sigma;
    
    
    
public:
    LocalLogVoltageToPowerCalculator(MyAPMSystem * sys_, double alpha_, MySysCellType cell_, My2Vec posit_, double sigma_);
    virtual double calcValue(int x, int y);
    
};

