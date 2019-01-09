//
//  ExpToVelSampler.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 10/29/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "ExpToVelSampler.h"
#import "CppHelper.h"
#import "MyAPMSystem.h"
using namespace std;

ExpToVelSampler::ExpToVelSampler(MySystemCalculator *calc_, double coeff_, MyAPMSystem* sys_):MyRejectionSampler(calc_, coeff_, sys_){
    //cout<<"In ExpToVelSampler constructor"<<endl;
    
    
}


pair<int, int> ExpToVelSampler::getSample(){
    
    
    //  cout<<"At rejection sample!"<<endl;
    _calc->calcMaxMinMean();
    int x, y;
    int n_tries = 0;
    double prob;
    bool not_valid;
    
    double max_value =  fmax(coeff * _calc->max(), coeff * _calc->min() );
    do{
        n_tries++;
        x = rand()%_sys->width();
        y = rand()%_sys->height();
        
        // prob = calc.acceptanceProb(x, y);
        prob = exp(coeff * _calc->calcValue(x, y) - max_value);
        //   cout<<"Prob is "<< prob<<endl;
        not_valid = !(_calc->shouldCount(x, y));
        //  cout<<"Not valid is "<<not_valid<<endl;
        
    }while(not_valid or unitRand()>prob);
    MyAssert(prob<=1, "Acceptance probability must be less than 1!");
    //    cout<<"Accepted with prob "<<prob<<endl;
    pair<int, int> my_answer(x,y);
    return my_answer;        
}