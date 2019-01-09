//
//  MyNonConChanSys.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 12/2/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "MyNonConChanSys.h"
#import "CppHelper.h"



MyNonConChanSys::MyNonConChanSys(SystemParams sysparams_):MyAPMSystem(sysparams_){
    cout<<"At MyNonConChanSys constructor"<<endl;
    
    setAllCellsTo(AccessEmptyCell);
    circuit()->zeroVoltage();
    for (int x =0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            _cellArray[hn->ind(x, y)].traveledness= (2 * (height() - y)) * 1./height();
            _cellArray[hn->ind(x, y)].traveledness= 1;

        }
    }    
    setRiverVoltagesAndDivergences();
    assignCircuitResistance();
    circuit()->prohibitPeriodicVerticalFlow();
    solveForCurrent();
    
    
    cout<<"Finished MyNonConChanSys constructor"<<endl;
    
}


void MyNonConChanSys::solveForCurrent(){
        circuit()->zeroVoltage();    
        updateCellDensities();
        assignCircuitResistance();
        circuit()->prohibitPeriodicVerticalFlow();
        circuit()->randomUniformVoltage();
        circuit()->solve();
        circuit()->checkSolution();
        _dirtyBit  = false;
    
}




void MyNonConChanSys::step(){
    
    _time += 1;
    
    should_plot = true;
    sprintf(plot_title, "/Users/samocko/Desktop/FilePathHeadacheReducer/CocoaChannelOutput/RiverFrame%d", int(_time));
    cout<<"At MyNonConChanSys step"<<endl;
    setRiverVoltagesAndDivergences();
    updateTraveledness();
    assignCircuitResistance();
    
    solveForCurrent();
    cout<<"Mean press is "<<circuit()->meanPressure()<<endl;
    
}

void MyNonConChanSys::setRiverVoltagesAndDivergences(){
    for (int x =0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {
            MyCircuitCell& cur_cell = circuit()->getCellAt(x, y);
            
            cur_cell.has_fixed_voltage = 0;
            cur_cell.fixed_voltage = 0;
            cur_cell.base_in_plane_divergence = 0;
            
            if(y == 0){
                cur_cell.fixed_voltage = 0;
                cur_cell.has_fixed_voltage = true;
            }
            else{
                cur_cell.base_in_plane_divergence = 1./(height()-1);
            }
            
            
        }
    }    
    
}

void MyNonConChanSys::updateTraveledness(){
    _dirtyBit = true;
    cout<<"Update traveledness"<<endl;
    
    double total_power = 0;
    
    for (int x =0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            total_power+= (.00 + powerThroughNode(x, y));
        }
    }    
    
    
    double mean_power = total_power/(height() * width());
    
    
    
    
    for (int x =0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            _cellArray[hn->ind(x, y)].traveledness+= .4 * (.00 + powerThroughNode(x, y)/mean_power) * (1 + .00001 * unitRand());
        }
    }
    
    double mean_trav = meanTraveledness();
    cout<<"Mean traveledness to divide by is "<<mean_trav<<endl;
        
    for (int x =0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            _cellArray[hn->ind(x, y)].traveledness*= 1./mean_trav;
        }
    }
    
}




double MyNonConChanSys::resistOfCell(MySystemCell cell){
    return powl(cell.traveledness, -4);
}

/*
void MyNonConChanSys::assignCircuitResistance(){
    
    double mean_traveledness = meanTraveledness();
    
    for(int x =0; x<hn->width(); x++){
        for (int y = 0; y<hn->height(); y++) {
            double cur_norm_trav = getCellAt(x, y).traveledness/mean_traveledness;
            double right_norm_trav = getCellAt(x+1, y).traveledness/mean_traveledness;
            double up_norm_trav = getCellAt(x, y+1).traveledness/mean_traveledness;
            
            
            {
                double mean_up_resist = .5  * (1./ square(cur_norm_trav) + 1./square(up_norm_trav));
                _circ->setUpResist(x, y, mean_up_resist);                                                   
            }
            
            {
                double mean_right_resist = .5  * (1./ square(cur_norm_trav) + 1./square(right_norm_trav));
                _circ->setRightResist(x, y, mean_right_resist);                                                   
            }
        }
    }
}
*/
