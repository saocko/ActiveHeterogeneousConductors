//
//  Helper.h
//  MyAPM
//
//  Created by Samuel Ocko on 6/10/11.
//  Copyright 2011 MIT. All rights reserved.
//
#pragma once

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#import "My2Vec.h"

#import "Eigen/Sparse"
#import "Eigen/SparseCore"

//#import "/Users/samocko/EigenPackage/Eigen/Sparse"
//#import "/Users/samocko/EigenPackage/Eigen/SparseCore"
static double MY_EPS =   1.0e-14F;        /* min positive value */

typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> EigTrip;


EigTrip transpTrip(EigTrip trip);


void MyAssert(bool not_bad, const char* errormessage);



 double bound(double a, double b, double c);

 double unitRand();

 int pmone();


int randInt(int mod);



 double square(double x);

 double square(My2Vec p);




 double fmod(double a, double b);
int mymod(int a, int b);

 void testFunct();



bool isBetween(double a, double b, double c);
