//
//  UpdatePrinter.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/18/12.
//  Copyright 2012 MIT. All rights reserved.
//


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

class UpdatePrinter {
    int last_string_length;
public:
    UpdatePrinter();
    
    void updatePrint(const char* message);
    void updateProg(double prog);
    void cleanup();
};