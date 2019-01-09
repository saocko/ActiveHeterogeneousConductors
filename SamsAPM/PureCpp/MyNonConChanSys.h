//
//  MyNonConChanSys.h
//  MyAPM
//
//  Created by Samuel Ocko on 12/2/12.
//  Copyright 2012 MIT. All rights reserved.
//


#import "MyAPMSystem.h"

#include <iostream>



class MyNonConChanSys:public MyAPMSystem{
    
    
public:
    

    MyNonConChanSys(SystemParams sysparams_);
    
    void solveForCurrent();
    void setRiverVoltagesAndDivergences();
    void updateTraveledness();

    double resistOfCell(MySystemCell cell);
    void step();
 //   void assignCircuitResistance();
    
    
};