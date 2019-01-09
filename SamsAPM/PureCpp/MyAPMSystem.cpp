//
//  MyAPMSystem.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 6/10/11.
//  Copyright 2011 MIT. All rights reserved.
//

#import "MyAPMSystem.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#import "CppHelper.h"
#import "VoltageTimesAlphaCellTypeLocalCalc.h"
#import "MyStopwatch.h"
#import "LocalLogVoltageToPowerCalculator.h"
using namespace std;


void MyAPMSystem::step(){    
    // cout<<"At MyAPMSystem step"<<endl;
  //  should_plot = true;
    _time +=1;
    
    MyStopwatch timer;
    
    if(sysParams().step_mode == AddAndRemovNonLocalMode){
       nonLocalEquilibrateStep();
    }
    if(sysParams().step_mode == RemovOnlyMode){
        randRemovStep();
    }
    if(sysParams().step_mode == AddOnlyMode){
        randAddStep();
    }
    if(sysParams().step_mode == AddAndRemovLocalMode){
        localRemovAndAddStep();   
    }
    
 //   cout<<"Finished step"<<endl;
}

void MyAPMSystem::localRemovAndAddStep(){
    //cout<<"In localRemovAndAddStep"<<endl;
    solveForCurrent();
    VoltageTimesAlphaCellTypeCalc remov_calc(this, 1, ParticleCell);
    _rejectionSampler->setCalcAndCoeff(&remov_calc, -sysParams().removCoeff);
    pair<int, int> loc = addOrRemoveParticle(*_rejectionSampler, AccessEmptyCell);
    
    My2Vec removal_point = Make2Vec(loc.first, loc.second + sysParams().bias_diffus);
    
    
    
    solveForCurrent();
    
    LocalLogVoltageToPowerCalculator add_calc(this, sysParams().addCoeff, AccessEmptyCell, removal_point, sysParams().diffus);
    ExpToVelSampler temp_samp(&add_calc, 1, this);
    
    
//    LocalVoltageToPowerCalculator add_calc(
    
 //   VoltageTimesAlphaCellTypeLocalCalc add_calc(this, sysParams().addCoeff, AccessEmptyCell, center_cond, sysParams().diffus);
 //   _rejectionSampler->setCalcAndCoeff(&add_calc, sysParams().addCoeff);
    addOrRemoveParticle(temp_samp, ParticleCell);
}

void MyAPMSystem:: nonLocalEquilibrateStep(){    
    randRemovStep();
    randAddStep();    
}

void MyAPMSystem:: randRemovStep(){
    
    //  cout<<"At randRemovStep"<<endl;
    if(sysParams().removFeedback){
       // solveForCurrent();
        VoltageTimesAlphaCellTypeCalc my_calc(this, 1, ParticleCell);
        _rejectionSampler->setCalcAndCoeff(&my_calc, -sysParams().removCoeff);
        addOrRemoveParticle(*_rejectionSampler, AccessEmptyCell);
    }
    else{
        removeRandomParticle();        
    }
}




void MyAPMSystem::randAddStep(){
        
    //  cout<<"At randAddStep"<<endl;
    
    if(sysParams().addFeedback){
        solveForCurrent();
        VoltageTimesAlphaCellTypeCalc my_calc(this, 1, AccessEmptyCell);               
        _rejectionSampler->setCalcAndCoeff(&my_calc, sysParams().addCoeff);

        addOrRemoveParticle(*_rejectionSampler, ParticleCell);
    }
    else{
        addRandomParticle();
    }
    // cout<<"Finished randAddStep"<<endl;
}








pair<int, int> MyAPMSystem::addOrRemoveParticle(MyRejectionSampler& sampler, MySysCellType new_cell_type){
 //   cout<<"At addOrRemoveParticle"<<endl;
    
    _dirtyBit = true;

    pair<int, int> loc = sampler.getSample();            
    _cellArray[hn->ind(loc.first, loc.second)].cell_type = new_cell_type; 
    return loc;
}




pair<int, int> MyAPMSystem::rejectionSample(MySystemCalculator & calc){
  //  cout<<"At rejection sample!"<<endl;
    calc.calcMaxMinMean();
    int x, y;
    int n_tries = 0;
    double prob;
    bool not_valid;
    do{
        n_tries++;
        x = rand()%sysParams().width;
        y = rand()%sysParams().height;
        
       // prob = calc.acceptanceProb(x, y);
        prob = exp(calc.calcValue(x, y) - calc.max());
     //   cout<<"Prob is "<< prob<<endl;
        not_valid = !(calc.shouldCount(x, y));
      //  cout<<"Not valid is "<<not_valid<<endl;

    }while(not_valid or unitRand()>prob);
    MyAssert(prob<=1, "Acceptance probability must be less than 1!");
//    cout<<"Accepted with prob "<<prob<<endl;
    pair<int, int> my_answer(x,y);
    return my_answer;    
}


void MyAPMSystem::solveForCurrent(){
    circuit()->periodicTubeResistAndVoltage();        
    if(sysParams().randDriv){ circuit()->randomUniformVoltage();}
    updateCellDensities();
    assignCircuitResistance();
    circuit()->solve();
    circuit()->checkSolution();
    
    _dirtyBit  = false;
}

