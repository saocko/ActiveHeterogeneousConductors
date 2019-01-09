//
//  MyCircuit.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 6/24/11.
//  Copyright 2011 MIT. All rights reserved.
//

#include "MyCircuit.h"
#include <memory.h>
//#include <Accelerate/Accelerate.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#import "CppHelper.h"
#import "MyStopwatch.h"


using namespace std;

MyCircuit:: MyCircuit(int width, int height, bool needsfixedvoltage_):_resistMatrix(width*height, width*height), _voltageVector(width*height), _baseDivergenceVector(width*height), _goalDivergenceVector(width*height)
{
    
    
    needs_fixed_voltage = needsfixedvoltage_;
    _width = width;
    _height = height;
    
    hn = new SquareIndexHelper(width, height);    
    
    _cells = new MyCircuitCell[width * height];
    //Sets up the cells that represent the resistance and voltage. 
    for (int i = 0; i<width*height; i++) { _cells[i] = zeroNonConCell();}
}



void MyCircuit:: solve(){    
    // cout<<"At sparseSolve"<<endl;
    
    constructResistanceMatrix(); //Translates into vector form.
    constructDivergenceVectors();
    
    SpMat flipped_mat = -1 * _resistMatrix;
    //   cout<<flipped_mat<<endl;
    //Make a solver, then have it initialize with the matrix, and then have it actually solve for the current. 
    
    MyStopwatch timer;
    Eigen::SimplicialLDLT<SpMat> my_solver;
    //    Eigen::ConjugateGradient<SpMat> my_solver;    
    
    my_solver.compute(flipped_mat);
    if(my_solver.info()!=Eigen::Success) {
        // decomposition failed
        cout<<"Decomposition failed! :("<<endl;
        return;
    }
    
    _voltageVector.setZero();
    //    _voltageVector = my_solver.solve(_baseDivergenceVector -_goalDivergenceVector);
    _voltageVector = my_solver.solve(_baseDivergenceVector - _goalDivergenceVector);
    
    if(my_solver.info()!=Eigen::Success) {
        // solving failed
        cout<<"Solving failed! :("<<endl;
        return;
    }
    
    // cout<<timer.secondsElapsed()<<" seconds elapsed in eigsolve"<<endl;
    
    constructCurrentCells();
    //  cout<<"Finished sparsesolve"<<endl;
}








void MyCircuit:: checkSolution(){
    
    
   // cout<<"Checking solution!"<<endl;
    double epsilon = .00001;
    
    
   // cout<<"Checking fixed volages!"<<endl;
    
    for (int x = 0; x < width() ; x++) {
        for (int y = 0; y<height(); y++) {
            MyCircuitCell & curr_cell = getCellAt(x, y);
            if(curr_cell.has_fixed_voltage){
            //    cout<<"Goal Voltage is "<<curr_cell.fixed_voltage<<", Actual is "<<curr_cell.solved_voltage<<endl;
                MyAssert(abs(curr_cell.fixed_voltage - curr_cell.solved_voltage)<epsilon, "Fixed voltage doesn't work!");
            }
            
        }
    }
    
    
    
    //cout<<"Checking divergence!"<<endl;
    
    for (int x = 0; x < width() ; x++) {
        for (int y = 0; y<height(); y++) {
            MyCircuitCell & curr_cell = getCellAt(x, y);
            if(!curr_cell.has_fixed_voltage){
                double current_div = upCurrent(x, y) + rightCurrent(x, y) + leftCurrent(x, y) + downCurrent(x, y) + 
                curr_cell.leak_cond * (curr_cell.solved_voltage - curr_cell.leak_voltage);
                
               // cout<<"Leak cond is "<<curr_cell.leak_cond <<" Solved voltage is "<<curr_cell.solved_voltage<<" Leak voltage is "<< curr_cell.leak_voltage<<endl;
             //   cout<<"x, y ("<<x<<", "<<y<<") Div is "<<current_div <<" Goal div is "<<curr_cell.base_in_plane_divergence<<endl;
                MyAssert((abs(current_div - curr_cell.base_in_plane_divergence) < epsilon), "Divergence doesn't match up! ");            
            }
        }
    }
    
 //   cout<<"Checking currents and voltages!"<<endl;
    
    for (int x = 0; x < width() ; x++) {
        for (int y = 0; y<height(); y++) {
            MyCircuitCell & curr_cell = getCellAt(x, y);
            MyCircuitCell & up_neighbor = getCellAt(x, y+1);
            MyCircuitCell & right_neighbor = getCellAt(x+1, y);
            
            double expected_up_current = (curr_cell.solved_voltage - up_neighbor.solved_voltage + curr_cell.up_driv_voltage) * curr_cell.up_cond;
            double expected_right_current = (curr_cell.solved_voltage - right_neighbor.solved_voltage + curr_cell.right_driv_voltage) * curr_cell.right_cond;
            
            //            cout<<"Up Curr is "<<curr_cell.up_current<<", Expected is "<<expected_up_current <<"Diff is "<<curr_cell.up_current - expected_up_current<<endl;
            //cout<<"Right Curr is "<<curr_cell.right_current<<", Expected is "<<expected_right_current<<"Diff is "<<curr_cell.right_current - expected_right_current<< endl;
            
            MyAssert(abs(expected_up_current - curr_cell.up_current)<epsilon, "Up Current Doesn't Match!");
            MyAssert(abs(expected_right_current - curr_cell.right_current)<epsilon, "Right Current Doesn't Match!");
            
        }
    }
    

    
  //  cout<<"Successful Check!"<<endl;
    
    
    
    
    
    
}







