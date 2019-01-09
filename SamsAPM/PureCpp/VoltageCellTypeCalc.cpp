//
//  VoltageCellTypeCalc.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "VoltageCellTypeCalc.h"
#import "MyAPMSystem.h"
using namespace std;

VoltageCellTypeCalc::VoltageCellTypeCalc(MyAPMSystem * sys_, MySysCellType cell_): MySystemCalculator(sys_){
    cell = cell_;
}


bool VoltageCellTypeCalc::shouldCount(int x, int y){
    // MyAPMSystem* sys = system();
    //   return true;
    //    system();
    // cout<<"At ShouldCount"<<endl;
    
    return system()->cellTypeAt(x, y) == cell;
}



double VoltageCellTypeCalc::calcValue(int x, int y){
    return system()->voltageThroughNode(x, y);    
}
