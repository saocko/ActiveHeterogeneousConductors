//
//  SamsRegionHelper.cpp
//  SamsChannelization
//
//  Created by Samuel Ocko on 12/19/12.
//  Copyright 2012 MIT. All rights reserved.
//

#import "CppHelper.h"
#include "SamsRegionHelper.h"
using namespace std;

SamsRegionHelper:: SamsRegionHelper(bool varyhoriz,  int period_, bool signflip_)
{
    
    cout<<"At SamsRegionHelper Constructor "<<endl;
    vary_horiz = varyhoriz;
    period = period_;
    sign_flipped = signflip_;
    
}


double SamsRegionHelper::areaFrac(){
    return  1./period * (1- sign_flipped) + (1-1./period) * sign_flipped;
}

bool SamsRegionHelper::operator() (int x, int y){
    return vary_horiz? (((x % period)==0) != sign_flipped) : (((y % period)==0) != sign_flipped);
}

SamsRegionHelper SamsRegionHelper::inverse(){
    return SamsRegionHelper(vary_horiz, period, !sign_flipped);
}

