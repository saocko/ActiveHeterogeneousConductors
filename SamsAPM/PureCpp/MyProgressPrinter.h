//
//  MyProgressPrinter.h
//  MyAPM
//
//  Created by Samuel Ocko on 1/18/13.
//  Copyright 2013 MIT. All rights reserved.
//


class MyProgressPrinter {
    int total_prints;
    int total_steps;
    int steps_completed;
    int steps_to_next_print;
    
    
    
public:
    MyProgressPrinter(int totalsteps_, int totalprints_);
    void step();
    void reset();    
};