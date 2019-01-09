//
//  MyStopwatch.h
//  MyAPM
//
//  Created by Samuel Ocko on 10/18/12.
//  Copyright 2012 MIT. All rights reserved.
//


class MyStopwatch {
    
    long start_time;
    
    
public:
    MyStopwatch();
    
    double secondsElapsed();
    void reset();
};