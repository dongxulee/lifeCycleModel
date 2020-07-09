//
// Created by Leo on 6/24/20.
//

#ifndef LIFECYCLE_ACTION_H
#define LIFECYCLE_ACTION_H

#include "constants.h"
class Action{
public:
    double c;
    double b;
    double k;
    Action(): c(0.0),b(0.0),k(0.0)
    {}
    Action(double inC, double inB, double inK){
        c = inC;
        b = inB;
        k = inK;
    }
    void update(double newC, double newB, double newK){
        c = newC;
        b = newB;
        k = newK;
    }
    void show(){
        cout << c << " "
             << b << " "
             << k << endl;
    }
};
#endif //LIFECYCLE_ACTION_H
