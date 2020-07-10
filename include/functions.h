//
// Created by Leo on 6/24/20.
//

#ifndef LIFECYCLE_FUNCTIONS_H
#define LIFECYCLE_FUNCTIONS_H

#include "constants.h"
#include "state.h"
#include "action.h"
#include "approximation.h"
#include <nlopt.hpp>

using namespace std;

// Define the utility function
double u(const double c) {
    int gamma = 2;
    // Minimum consumption of the agent
    double c_bar = 3;
    return (pow(max(c-c_bar,0.0),1-gamma) - 1)/(1 - gamma);
}

// Define the bequeath function, which is a function of wealth
double uB(const double w) {
    double c_bar = 3;
    double B = 2;
    return B * u(w + c_bar + 1);
}

// Define the earning function, which applies for both employment and unemployment
double y(const int t, const State& x) {
    // state contains values: w, n, s, e, A
    if (x.A == 0) {
        return 0;
    } else {
        if (t < T_r) {
            return detEarning[t] * earningShock[x.s] * x.e + (1 - x.e) * 5;
        } else {
            return detEarning[t];
        }
    }
}


// Define the reward function depends on both state and action.
double R(const State& x, const Action& a){
    if (x.A == 0){
        return uB(x.w+(1+r_bar)*x.n);
    }
    else{
        return u(a.c);
    }
}

// Define the transition of state
/*
        Input: x current state: (w, n, s, e, A)
        a action taken: (c, b, k)
        Output: the next possible states with corresponding probabilities
*/
tuple<vector<State>, vector<double>> transition(const State& x, const Action& a, const int t){
    // Define placeholder
    vector<State> x_next;
    vector<double> prob_next;
    vector<int> S_next = {0,1};
    vector<int> E_next = {0,1};
    vector<int> A_next = {0,1};
    // Agent is dead at the end of last period
    if (x.A == 0){
        for(int s_next : S_next){
            x_next.emplace_back(0.0,0.0,s_next,0,0);
            prob_next.push_back(Ps[x.s][s_next]);
        }
    // Agent is alive
    } else{
        //variables needed
        vector<double> Paa = vector<double>(Pa.begin() + t, Pa.end());
        double N = accumulate(Paa.begin(), Paa.end(), 0.0);
        double discounting = (pow(1+r_bar,N)-1)/(r_bar*pow(1+r_bar,N));
        vector<double> Pat = {1-Pa[t], Pa[t]};
        double r_bond = r_f[x.s];
        // calculate n_next
        if (t < T_r){
            double n_next;
            if (x.e == 1){
                n_next = (x.n+0.05*y(t,x))*(1+r_bar);
            }
            else{
                n_next = x.n*(1+r_bar);
            }
            // for potential s_next, e_next and A_next
            for (int s_next:S_next){
                double r_stock = r_m[x.s][s_next];
                double w_next = a.b*(1+r_bond) + a.k*(1+r_stock);
                for (int e_next:E_next){
                    for (int a_next:A_next){
                        x_next.emplace_back(w_next,n_next,s_next,e_next,a_next);
                        prob_next.push_back(Ps[x.s][s_next] *  Pat[a_next]*Pe[x.s][s_next][x.e][e_next]);
                    }
                }
            }
        }
        else{
            // After retirement agents withdraw cash from 401k
            double n_next = x.n*(1+r_bar)-x.n/discounting;
            int e_next = 0;
            // for potential s_next and A_next
            for (int s_next:S_next){
                double r_stock = r_m[x.s][s_next];
                double w_next = a.b*(1+r_bond) + a.k*(1+r_stock);
                for (int a_next: A_next){
                    x_next.emplace_back(w_next,n_next,s_next,e_next,a_next);
                    prob_next.push_back(Ps[x.s][s_next] *  Pat[a_next]);
                }
            }
        }
    }
    return make_tuple(x_next, prob_next);
}

// additional data needed for the objective function
typedef struct {
    State x;
    int t;
    double ytx;
    double n_discount;
    Prox V_model;
} info;

double obj(const vector<double> &theta, vector<double> &grad, void *data){
    // passing in the information here
    info *in = reinterpret_cast<info*>(data);
    State x = in->x;
    int t = in->t;
    double ytx = in->ytx;
    double n_discount = in->n_discount;
    Prox V_model = in->V_model;
    // define the objective function
    double theta1 = theta[0];
    double theta2 = theta[1];
    double bk;
    double c;
    if (t < T_r){
        if (x.e == 1){
            bk = ((1-tau_L)*(ytx * 0.95) + x.w) * theta1;
            c = ((1-tau_L)*(ytx * 0.95) + x.w) * (1-theta1);
        }
        else{
            bk = ((1-tau_L)*ytx + x.w) * theta1;
            c = ((1-tau_L)*ytx + x.w) * (1-theta1);
        }
    }
    else{
        bk = ((1-tau_R)*ytx + x.w + n_discount) * theta1;
        c = ((1-tau_R)*ytx + x.w + n_discount) * (1-theta1);
    }
    double b = bk * theta2;
    double k = bk * (1-theta2);
    Action a(c,b,k);
    vector<State> x_next;
    vector<double> prob_next;
    tie(x_next, prob_next) = transition(x, a, t);
    // calculate possible value_next
    vector<double> V_tilda;
    for (State xx:x_next){
        V_tilda.push_back(V_model.ev(xx,t));
    }
    return R(x, a) + beta * inner_product(begin(prob_next), end(prob_next), begin(V_tilda), 0.0);
}


// Value function is a function of state and time t and a function approximate
tuple<double, Action> V(const State& x, int t, const Prox &V_model){
    if (x.A == 0){
        return make_tuple(R(x, Action(0,0,0)), Action(0,0,0));
    }
    else{
        // variables needed
        vector<double> Paa = vector<double>(Pa.begin() + t, Pa.end());
        double N = accumulate(Paa.begin(), Paa.end(), 0.0);
        double discounting = (pow(1+r_bar,N)-1)/(r_bar*pow(1+r_bar,N));
        double n_discount = x.n/discounting;
        double ytx = y(t,x);
        // optimization part
        info data = {x,t,ytx,n_discount,V_model};
        nlopt::opt opt(nlopt::LN_NELDERMEAD, 2);
        vector<double> lb = {0,0};
        vector<double> ub = {1,1};
        opt.set_lower_bounds(lb);
        opt.set_upper_bounds(ub);
        opt.set_max_objective(obj, &data);
        opt.set_xtol_rel(1e-5);
        vector<double> optX={0.5,0.5};
        double optF;
        opt.optimize(optX, optF);
        // Construct the return values
        double theta1 = optX[0];
        double theta2 = optX[1];
        double bk;
        double c;
        if (t < T_r){
            if (x.e == 1){
                bk = ((1-tau_L)*(ytx * 0.95) + x.w) * theta1;
                c = ((1-tau_L)*(ytx * 0.95) + x.w) * (1-theta1);
            }
            else{
                bk = ((1-tau_L)*ytx + x.w) * theta1;
                c = ((1-tau_L)*ytx + x.w) * (1-theta1);
            }
        }
        else{
            bk = ((1-tau_R)*ytx + x.w + n_discount) * theta1;
            c = ((1-tau_R)*ytx + x.w + n_discount) * (1-theta1);
        }
        double b = bk * theta2;
        double k = bk * (1-theta2);
        return make_tuple(optF, Action(c,b,k));
    }
}

#endif //LIFECYCLE_FUNCTIONS_H