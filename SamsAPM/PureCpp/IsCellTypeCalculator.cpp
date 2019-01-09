//
//  IsCellTypeCalculator.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//

//
//  IsCellTypeCalculator.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/21/12.
//  Copyright 2012 MIT. All rights reserved.
//
#import "MySystemCalculator.h"
#import "IsCellTypeCalculator.h"
#import "MyAPMSystem.h"

bool IsCellTypeCalculator::shouldCount(int x, int y){
    return true;    
}

double IsCellTypeCalculator::calcValue(int x, int y){
    if(cell == system()->cellTypeAt(x, y)){
        return 1;
    }
    else{
        return 0;
    }
}