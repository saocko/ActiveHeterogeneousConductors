//
//  SquareIndexHelper.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 6/24/11.
//  Copyright 2011 MIT. All rights reserved.
//


#include "SquareIndexHelper.h"
#include <math.h>
#include <iostream>
#import "CppHelper.h"
using namespace std;

SquareIndexHelper::SquareIndexHelper(int xdim, int ydim, int extraentries_){
    _width = xdim;
    _height = ydim;
    _extraEntries = extraentries_;

}




//Automatically used periodic
int SquareIndexHelper:: ind(int x, int y){
   // cout<<"At ind, Height is "<<_height<<", width is "<<_width<<endl;
    int new_y = mymod(y, _height);
    int new_x = mymod(x, _width);
  //  cout<<"Old y is "<<y<<", Old x is "<<x<<endl;
  //  cout<<"New y is "<<new_y<<", New x is "<<new_x<<endl;

    
    
    return new_y + new_x* _height;
}

int SquareIndexHelper:: matind(int x1, int y1, int x2, int y2){
    return ind(x1, y1) +vecsize() * ind(x2,y2);
}

int SquareIndexHelper:: diagMatInd(int x, int y){
    return matind(x, y, x, y);
}

int SquareIndexHelper:: matsize(){return vecsize() * vecsize();}
int SquareIndexHelper::vecsize(){return _width * _height + _extraEntries;}
int SquareIndexHelper:: width(){return _width;}
int SquareIndexHelper::height(){return _height;}


bool SquareIndexHelper::isBoundary(int x, int y){
    return x==0 or y==0 or x== width()-1 or y == height() -1;
}

bool SquareIndexHelper::isLegal(int x, int y){
    return x>=0 and y>=0 and x<width() and y< height();
}

