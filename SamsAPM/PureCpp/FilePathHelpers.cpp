//
//  FilePathHelpers.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/24/13.
//  Copyright 2013 MIT. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

#include "FilePathHelpers.h"
void makeFilePath(const char* str){
    char sys_command[500];
//    sprintf(buff, "Hello")
    sprintf(sys_command, "mkdir -p %s", str);
    system(sys_command);
}