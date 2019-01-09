//
//  VoltageTimesAlphaCellTypeCalc.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "VoltageTimesAlphaCellTypeCalc.h"
#import "MyAPMSystem.h"
using namespace std;

VoltageTimesAlphaCellTypeCalc::VoltageTimesAlphaCellTypeCalc(MyAPMSystem * sys_, double alpha_, MySysCellType cell_): VoltageTimesAlphaCalculator(sys_, alpha_){
    cell = cell_;
}


bool VoltageTimesAlphaCellTypeCalc::shouldCount(int x, int y){
   // MyAPMSystem* sys = system();
 //   return true;
//    system();
   // cout<<"At ShouldCount"<<endl;
    
    return system()->cellTypeAt(x, y) == cell;
}
