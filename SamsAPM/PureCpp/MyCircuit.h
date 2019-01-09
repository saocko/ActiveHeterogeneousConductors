//
//  MyCircuit.h
//  MyAPM
//
//  Created by Samuel Ocko on 6/24/11.
//  Copyright 2011 MIT. All rights reserved.
//

#import "SquareIndexHelper.h"
#import "MyCircuitCell.h"
#include <math.h>
#import "Eigen/Sparse"
#import "Eigen/SparseCore"

typedef Eigen::SparseMatrix<double> SpMat; // declares a column-major sparse matrix type of double
typedef Eigen::Triplet<double> EigTrip;


class MyCircuit {
    int _width, _height;
    SquareIndexHelper *hn;
    MyCircuitCell *_cells;    
    SpMat _resistMatrix;
    Eigen::VectorXd _baseDivergenceVector, _goalDivergenceVector, _voltageVector; 
    
    
    int curr_vec_size;
    int curr_mat_size;
    bool needs_fixed_voltage;
    
    
    
public:
    
    
    MyCircuit(int width, int height, bool needsfixedvoltage_ = false);
    
    //For a width*height grid of nodes. Periodic boundary conditions. 
    int width();
    int height();
    
    
    //Various Accessors and mutators
    void setUpVoltage(int x, int y, double v);
    void setRightVoltage(int x, int y, double v);
    void zeroVoltage(); //Sets all voltages to 0
    void setUpResist(int x, int y, double r);    
    void setRightResist(int x, int y, double v);    
    void setOutOfPlaneVoltage(int x, int y, double v);
    void setOutOfPlaneConductance(int x, int y, double c);
    
    void setHasFixedVoltage(int x, int y, bool isfixed_);
    void setFixedVoltage(int x, int y, double v);
    
    
    double upCond(int x, int y){return getCellAt(x, y).up_cond;}
    double rightCond(int x, int y){return getCellAt(x, y).right_cond;}
    
    
    inline MyCircuitCell& getCellAt(int x, int y){return _cells[hn->ind(x, y)];}
    inline int indOf(int x, int y){return  hn->ind(x, y);}

    
    
    
    double upVoltage(int x, int y);
    double rightVoltage(int x, int y);
    double upResist(int x, int y);
    double rightResist(int x, int y);
    
    double upCurrent(int x, int y);
    double rightCurrent(int x, int y);
    double downCurrent(int x, int y);
    double leftCurrent(int x, int y);
    
    double meanPressure();
    //These are slightly fancier accessors...    
    double currentThroughNode(int x, int y);
    double netFlow();
    double netXFlow();
    double netYFlow();
    double maxCurrent();
    double maxDivergence();
    
    
    
    //These are slightly fancier mutators
    void randomResistanceAndVoltage();
    void tubeResistanceAndVoltage();
    void periodicTubeResistAndVoltage();
    void unitResistance();    
    void randomUniformVoltage();
    void prohibitPeriodicVerticalFlow();
    
    
    
    // The actual solving for currents and checking of the solution. 
    void solve();
    void  checkSolution();
    
    
private:
    //These are the tranlsational methods, where I translate my representation to the representation that eigen uses and back again. 
    void constructResistanceMatrix();
    void constructDivergenceVectors();
    void constructCurrentCells();
    
};