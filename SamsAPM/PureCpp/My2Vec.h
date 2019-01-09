//
//  My2Vec.h
//  MyAPM
//
//  Created by Samuel Ocko on 6/11/11.
//  Copyright 2011 MIT. All rights reserved.
//



struct My2Vec {
    double x, y;
};

inline My2Vec Make2Vec(double x, double y) {
    My2Vec v;
    v.x = x;
    v.y = y;
    return v;
}

inline My2Vec operator+(const My2Vec& lhs, const My2Vec& rhs) {
    My2Vec p;
    p.x = lhs.x + rhs.x;
    p.y = lhs.y + rhs.y;
    return p;
}

inline My2Vec operator-(const My2Vec& lhs, const My2Vec& rhs) {
    My2Vec p;
    p.x = lhs.x - rhs.x;
    p.y = lhs.y - rhs.y;
    return p;
}


inline My2Vec operator*(const My2Vec& lhs, const My2Vec& rhs) {
    My2Vec p;
    p.x = lhs.x*rhs.x;
    p.y = lhs.y*rhs.y;
    return p;
}

inline My2Vec operator/(const My2Vec& lhs, const My2Vec& rhs) {
    My2Vec p;
    p.x = lhs.x/rhs.x;
    p.y = lhs.y/rhs.y;
    return p;
}



inline My2Vec operator*(const My2Vec& lhs, double rhs) {
    My2Vec p;
    p.x = lhs.x *rhs;
    p.y = lhs.y *rhs;
    return p;
}


inline My2Vec operator*( double lhs,const My2Vec& rhs) {
    My2Vec p;
    p.x = rhs.x *lhs;
    p.y = rhs.y *lhs;
    return p;
}

/*
My2Vec zeroVec(){
    My2Vec v = {0, 0};
    return v;
}

 */


/*
inline My2Vec operator+=(const My2Vec& lhs, const My2Vec& rhs) {
    My2Vec p;
    lhs.x += rhs.x;
    p.y = lhs.y - rhs.y;
    return p;
}*/