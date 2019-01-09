//
//  ExpToVelSampler.h
//  MyAPM
//
//  Created by Samuel Ocko on 10/29/12.
//  Copyright 2012 MIT. All rights reserved.
//


#import "MyRejectionSampler.h"


class ExpToVelSampler:public MyRejectionSampler {
    
public:

    ExpToVelSampler(MySystemCalculator *calc_, double coeff_, MyAPMSystem* sys_);    
    virtual pair<int, int> getSample();
    
};