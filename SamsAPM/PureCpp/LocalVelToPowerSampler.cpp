//
//  LocalVelToPowerSampler.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 10/29/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "LocalVelToPowerSampler.h"
#import "CppHelper.h"
#import "MyAPMSystem.h"

using namespace std;

LocalVelToPowerSampler::LocalVelToPowerSampler(MySystemCalculator *calc_, double coeff_, MyAPMSystem* sys_):MyRejectionSampler(calc_, coeff_, sys_){
    cout<<"In LocalVelToPowerSampler constructor"<<endl;
    
    
}


pair<int, int> LocalVelToPowerSampler::getSample(){
    
    //  cout<<"At rejection sample!"<<endl;
    _calc->calcMaxMinMean();
    int x, y;
    int n_tries = 0;
    double prob;
    bool not_valid;
    
    
    
    double max_calc_value = _calc->max();
    double min_calc_value = _calc->min();
    double denom_value = (coeff>0)?max_calc_value:min_calc_value;
    
    do{
        n_tries++;
        x = rand()%_sys->width();
        y = rand()%_sys->height();
        
        // prob = calc.acceptanceProb(x, y);
        double calc_value = _calc->calcValue(x, y);
        prob = pow(calc_value/denom_value, coeff);
        //   cout<<"Prob is "<< prob<<endl;
        not_valid = !(_calc->shouldCount(x, y));
        //  cout<<"Not valid is "<<not_valid<<endl;        
    }while(not_valid or unitRand()>prob);
    //   cout<<"Coeff is "<<coeff<<"max_value is "<<max_value<<endl;    
    //    cout<<"Coeff is "<<coeff<<"Prob is "<<prob<<endl;
    MyAssert(prob<=1, "Acceptance probability must be less than or equal to 1!");
    //    cout<<"Accepted with prob "<<prob<<endl;
    pair<int, int> my_answer(x,y);
    return my_answer;        
}