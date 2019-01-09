//
//  MyCircuitTranslations.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 6/30/11.
//  Copyright 2011 MIT. All rights reserved.
//

#include "MyCircuit.h"
#include <memory.h>
#include <iostream>
#import "CppHelper.h"
#include <math.h>
using namespace std;

void MyCircuit:: constructCurrentCells(){
    
    //Fills the solved voltage array
    for (int x = 0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            MyCircuitCell& curr_cell = getCellAt(x, y);
            int cur_ind = indOf(x, y);
            curr_cell.solved_voltage = _voltageVector[cur_ind];
           // cout<<"Solved voltage is "<<curr_cell.solved_voltage<<endl;
        }
    }

    
    
    //Now that we have solved for the voltage, we can reconstruct the current through the local driving. 
    for (int x = 0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            MyCircuitCell& curr_cell  = getCellAt(x, y);
            MyCircuitCell& up_cell    = getCellAt(x, y+1);
            MyCircuitCell& right_cell = getCellAt(x+1, y);
            
            curr_cell.up_current = curr_cell.up_cond * (curr_cell.solved_voltage - up_cell.solved_voltage + curr_cell.up_driv_voltage);
            curr_cell.right_current = curr_cell.right_cond * (curr_cell.solved_voltage - right_cell.solved_voltage + curr_cell.right_driv_voltage);
            
        }
    }
}


void MyCircuit:: constructResistanceMatrix(){
    //    cout<<"Starting constructResistanceMatrix"<<endl;
    vector<EigTrip> triplet_vec; 
    triplet_vec.clear();
    
    
    
    //Our matrix is positive-definite, so we just need to flip it at the end
    int dxs[2] = {1, 0};
    int dys[2] = {0, 1};
    
    
    for (int x = 0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            MyCircuitCell& curr_cell = getCellAt(x, y);
            int cur_ind = indOf(x, y);
            for (int i = 0; i<2; i++) {
                MyCircuitCell& neighbor_cell = getCellAt(x + dxs[i], y+dys[i]);
                int neighbor_ind = indOf(x + dxs[i], y+dys[i]);
                double cur_cond = (i ==0)? rightCond(x, y): upCond(x,y);
                
                if (!(curr_cell.has_fixed_voltage or neighbor_cell.has_fixed_voltage)){
                    //Does the off diagonal entries. High voltage in one is negative divergence in the other
                    EigTrip off_diag_trip(cur_ind, neighbor_ind, -cur_cond);
                    triplet_vec.push_back(off_diag_trip);
                    triplet_vec.push_back(transpTrip(off_diag_trip));
                }
                
                if(!curr_cell.has_fixed_voltage){
                    //Does the diagonal entries for the current cell
                    EigTrip eff_diag_trip(cur_ind, cur_ind, cur_cond);
                    triplet_vec.push_back(eff_diag_trip);
                }
                if(!neighbor_cell.has_fixed_voltage){
                    EigTrip eff_diag_trip(neighbor_ind, neighbor_ind, cur_cond);
                    triplet_vec.push_back(eff_diag_trip);
                }
                
            }
            if(!curr_cell.has_fixed_voltage){
               // cout<<"Doing diag_leak_trip with leak_cond "<<curr_cell.leak_cond<< endl;
                EigTrip diag_leak_trip(cur_ind, cur_ind, curr_cell.leak_cond);
                triplet_vec.push_back(diag_leak_trip);
            }
        }
        
        
    }
    
    
    
    
    for (int x = 0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            MyCircuitCell& curr_cell = getCellAt(x, y);
            int cur_ind = indOf(x, y);
            if(curr_cell.has_fixed_voltage){
                EigTrip diag_fixed_volt_trip(cur_ind, cur_ind, 1);
                triplet_vec.push_back(diag_fixed_volt_trip);
                //We have a bunch of dummy indices that share no off-diag terms with anything else. 
            }
        }
    }
    //Just add an extra constraint to make it not underconstrained.
    if(needs_fixed_voltage){
        triplet_vec.push_back(EigTrip(0, 0, 1));    
    }
    
    _resistMatrix.setZero();
    _resistMatrix.setFromTriplets(triplet_vec.begin(), triplet_vec.end());
    //    cout<<"Finishing constructResistanceMatrix"<<endl;
    
}


void MyCircuit::constructDivergenceVectors(){
    
    
    
    //The sparse divergencevector is what the divergence would be if the voltages are 0 - the goal divergence. 
    //ResistMat * voltages + base_divergence = goal_divergence
    //ResistMat * voltages = goal_divergence - base_divergence
    //-ResistMat * voltages = -goal_divergence + base_divergence
    
    
    
    _baseDivergenceVector.setZero();
    //Our matrix is positive-definite, so we just need to flip it at the end
    int dxs[2] = {1, 0};
    int dys[2] = {0, 1};
    
    
    for (int x = 0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            MyCircuitCell& curr_cell = getCellAt(x, y);
            int cur_ind = indOf(x, y);
            for (int i = 0; i<2; i++) {
                MyCircuitCell& neighbor_cell = getCellAt(x + dxs[i], y+dys[i]);
                int neighbor_ind = indOf(x + dxs[i], y+dys[i]);

                double cur_cond = (i ==0)? rightCond(x, y): upCond(x,y);
                double cur_driving = (i ==0)? rightVoltage(x, y): upVoltage(x, y);
                
                
                if(!curr_cell.has_fixed_voltage and neighbor_cell.has_fixed_voltage){
                    //Base voltage diff is driving minus the neighbor voltage
                    _baseDivergenceVector[cur_ind] +=cur_cond * ( - neighbor_cell.fixed_voltage);
                }
                if(curr_cell.has_fixed_voltage and !neighbor_cell.has_fixed_voltage){
                    //Base voltage diff is minus driving minus the neighbor voltage
                    _baseDivergenceVector[neighbor_ind] -=cur_cond * (curr_cell.fixed_voltage);
                }
                
                if(!curr_cell.has_fixed_voltage){
                    _baseDivergenceVector[cur_ind] +=cur_cond * cur_driving;
                }
                if(!neighbor_cell.has_fixed_voltage){
                    _baseDivergenceVector[neighbor_ind] -= cur_cond * cur_driving;
                }
                
            }
            
            
            if(!curr_cell.has_fixed_voltage){
                _baseDivergenceVector[cur_ind] -=curr_cell.leak_cond * curr_cell.leak_voltage;                
            }
        }
        
        
    }
    
    
    _goalDivergenceVector.setZero();
    for (int x = 0; x<width(); x++) {
        for (int y = 0; y<height(); y++) {            
            MyCircuitCell & curr_cell = getCellAt(x, y);
            int cur_ind = indOf(x, y);
            if(curr_cell.fixed_voltage){
                _goalDivergenceVector[cur_ind] = curr_cell.fixed_voltage;
            }
            else{
                _goalDivergenceVector[cur_ind] = curr_cell.base_in_plane_divergence;
            }
        }
    }
    
    
    
    for (int i = 0; i < width()*height(); i++) {
        //cout<<"Goal Div "<<_goalDivergenceVector[i]<<", Base Div "<<_baseDivergenceVector[i]<<endl;
    }
    
}