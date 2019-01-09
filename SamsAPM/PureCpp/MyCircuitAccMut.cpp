//
//  MyCircuitAccMut.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 6/30/11.
//  Copyright 2011 MIT. All rights reserved.
//

#import "MyCircuit.h"
#include <math.h>
#import "CppHelper.h"




double MyCircuit:: upVoltage(int x, int y){
    return getCellAt(x, y).up_driv_voltage;
}

double MyCircuit:: rightVoltage(int x, int y){ 
    return getCellAt(x,y).right_driv_voltage;
}

double MyCircuit:: upResist(int x, int y){
    return 1./upCond(x, y);
}

double MyCircuit:: rightResist(int x, int y){    
    return 1./rightCond(x, y);
}



void MyCircuit:: setUpVoltage(int x, int y, double v){
    getCellAt(x, y).up_driv_voltage = v;
}

void MyCircuit:: setRightVoltage(int x, int y, double v){    
    getCellAt(x, y).right_driv_voltage = v;
}

void MyCircuit:: setUpResist(int x, int y, double r){
    getCellAt(x, y).up_cond = 1./r;
}

void MyCircuit:: setRightResist(int x, int y, double r){    
    getCellAt(x, y).right_cond = 1./r;
}


double MyCircuit::meanPressure(){
    double total_pressure = 0;
    
    for(int x = 0; x<_width; x++){
        for (int y = 0; y<_height; y++) {
            total_pressure += getCellAt(x, y).solved_voltage;
        }
    }
    
    return total_pressure/(width() * height());
}

double MyCircuit::upCurrent(int x, int y){
    return getCellAt(x, y).up_current;
}

double MyCircuit::rightCurrent(int x, int y){    
    return getCellAt(x,y).right_current;
}

double MyCircuit::leftCurrent(int x, int y){    
    return -rightCurrent(x-1, y);
    //    return (hn->isLegal(x, y))?_currentCells[hn->ind(x, y)].rightvalue  : 0;
}

double MyCircuit::downCurrent(int x, int y){    
    return -upCurrent(x, y-1);
    //    return (hn->isLegal(x, y))?_currentCells[hn->ind(x, y)].rightvalue  : 0;
}

double MyCircuit:: currentThroughNode(int x, int y){
    
    return sqrt(square(rightCurrent(x, y)) 
                +square(upCurrent(x, y)) 
                +square(rightCurrent(x-1, y)) 
                +square(upCurrent(x, y-1))) * sqrt(.5);
    
}



int MyCircuit:: width(){return _width;}
int MyCircuit:: height(){return _height;}



double MyCircuit:: netYFlow(){
    int y = int(_height * .5);
    
    
    double net_up_current = 0.;
    for(int x = 0; x<_width; x++){
        net_up_current +=upCurrent(x, y);
    }
    return net_up_current;
}

double MyCircuit:: netXFlow(){
    int x = int(_width * .5);
    
    
    double net_right_current = 0.;
    for(int y = 0; y<_height; y++){
        net_right_current +=rightCurrent(x, y);
    }
    return net_right_current;
}


double MyCircuit:: netFlow(){
    return sqrt(square(netXFlow()) + square(netYFlow()));
}

double MyCircuit::maxCurrent(){
    double max_current = 0;
    
    for(int x = 1; x<_width-1; x++){
        for (int y = 1; y<_height-1; y++) {
            max_current = fmax(max_current, currentThroughNode(x, y));
        }
    }
    
    return max_current;
}





double MyCircuit::maxDivergence(){
    
    double max_div = 0;
    
    for(int x = 0; x<_width; x++){
        for (int y = 0; y<_height; y++) {
            double div = upCurrent(x, y) + rightCurrent(x, y) - upCurrent(x, y-1) - rightCurrent(x-1, y);
            max_div = fmax(max_div, abs(div));
        }
    }
    
    return max_div;
    
}


