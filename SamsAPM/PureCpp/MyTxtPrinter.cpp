//
//  MyTxtPrinter.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/26/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "MyTxtPrinter.h"
#import "MyAPMSystem.h"
using namespace std;

MyTxtPrinter::MyTxtPrinter(char* path_){
    //char buff[300];
    my_path = path_;
    
    my_file.open(my_path);
    
}

MyTxtPrinter::~MyTxtPrinter(){
    my_file.close();
}

void MyTxtPrinter::printSystemHeader(MyAPMSystem* system){
    my_file<<system->width()<<"\t"<<system->height()<<"\t";
    for(int i = 2; i<system->width(); i++){
        my_file<<-1<<"\t";
    }
    my_file<<endl;
}



void MyTxtPrinter::printSystem(MyAPMSystem* system){
    for (int y = system->height()-1; y>=0; y--) {
        for (int x = 0; x<system->width(); x++) {
            my_file<<(int)system->cellTypeAt(x, y)<<"\t";
        }
        my_file<<endl;
    }

    for (int x = 0; x<system->width(); x++) {
        my_file<<-1<<"\t";
    }
    my_file<<endl;    
}

void MyTxtPrinter::printDoubleVec(vector<double>vec){
    for (int i = 0; i<vec.size(); i++) {
        printDouble(vec[i]);
    }
}

void MyTxtPrinter::printDoublePair(double x, double y){
    char buff[500];
    sprintf(buff, "%.06f \t %.06f \n", x, y);
    my_file<<buff;
}


void MyTxtPrinter::printDouble(double x){
    char buff[500];
    sprintf(buff, "%.06f", x);    
    my_file<<x<<endl;
}
