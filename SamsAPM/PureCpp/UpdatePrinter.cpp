//
//  UpdatePrinter.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/18/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "UpdatePrinter.h"
using namespace std;

UpdatePrinter::UpdatePrinter(){
    last_string_length = 0;
    
}

void UpdatePrinter:: updatePrint(const char* message){
    char buff[2];
    buff[0] = (char)8;
    buff[1] = (char)0; //Null
//    sprintf(buff, "\b");
    for (int i =0; i<last_string_length; i++) {
        //printf("\bHere");
    }
    
    
    last_string_length = (int)strlen(message);
  //  cout<<"Last String "<<last_string_length;
    printf("%s \n", message);
}


void UpdatePrinter:: updateProg(double prog){
    char buff[500];
    
    sprintf(buff, "Prog: %.4f", prog);
    updatePrint(buff);
}
void UpdatePrinter:: cleanup(){
    updatePrint("");
}