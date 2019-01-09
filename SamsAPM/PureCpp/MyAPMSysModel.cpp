//
//  MyAPMSystemModelMethods.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 6/20/11.
//  Copyright 2011 MIT. All rights reserved.
//

#import "MyAPMSystem.h"
#include <stdio.h>

#include <iostream>

#import "CppHelper.h"

#include <math.h>
using namespace std;

#import "MySystemCell.h"

    


double MyAPMSystem::resistOfCell(MySystemCell cell){
    switch ( cell.cell_type ) {
        case AccessEmptyCell:            
            return 1; 
            break;
        case InAcessEmptyCell:            
            return 1;
            break;
        case ParticleCell:            
            return sysParams().filled_resistance;
            break;
        case SubstrateCell:            
            return sysParams().filled_resistance;
            break;
        default:            
            char error_mess[500];
            sprintf(error_mess, "In resistOfCell, couldn't match %d", (int)cell.cell_type);
            MyAssert(false, error_mess);
            return 0;
    }
    
}