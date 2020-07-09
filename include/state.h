//
// Created by Leo on 6/24/20.
//

#ifndef LIFECYCLE_STATE_H
#define LIFECYCLE_STATE_H

#include "constants.h"
class State{
public:
    double w;
    double n;
    int e;
    int s;
    int A;
    State(): w(0.0),n(0.0),e(1),s(1),A(1)
    {}
    State(double inW, double inN, int inE, int inS, int inA){
        w = inW;
        n = inN;
        e = inE;
        s = inS;
        A = inA;
    }
    void update(double newW, double newN, int newE, int newS, int newA){
        w = newW;
        n = newN;
        e = newE;
        s = newS;
        A = newA;
    }
    void show(){
        cout << w << " "
        << n << " "
        << e << " "
        << s << " "
        << A << endl;
    }
};

#endif //LIFECYCLE_STATE_H
