//
// Created by Leo on 6/23/20.
//
#ifndef LIFECYCLE_CONSTANTS_H
#define LIFECYCLE_CONSTANTS_H

#include <bits/stdc++.h>
using namespace std;

// Define the timeline of the model
const int T_min  = 0;
const int T_max = 70;
// Define age of retirement
const int T_r = 45;
// Define discounted factor
const double beta = 1/(1+0.02);
// Define transition matrix of economical states
// GOOD -> GOOD 0.8, BAD -> BAD 0.6
const vector<vector<double>> Ps = {{0.6, 0.4},{0.2, 0.8}};
// A states
const vector<int> Astate = {0,1};
// e states
const vector<int> Estate = {0,1};
// s states
const vector<int> Sstate = {0,1};
// Uncertainty in the earning
const vector<double> earningShock = {0.8,1.2};
// Uncertainty in risk free return
const vector<double> r_f = {0.01 ,0.03};
// Uncertainty in stock return depends on current and future econ states
const vector<vector<double>> r_m =  {{-0.2, 0.15},{-0.15, 0.2}};
// Probability of employment transition Pe[s, s_next, e, e_next]
const vector<vector<vector<vector<double>>>> Pe = {{{{0.3 , 0.7 },{0.1 , 0.9 }},{{0.25, 0.75},{0.05, 0.95}}},
                               {{{0.25, 0.75},{0.05, 0.95}},{{0.2 , 0.8 },{0.01, 0.99}}}};
// Expected return on stock market
const double r_expect =  0.0667;
const double r_bar = 0.02;
// Tax rate before and after retirement
const double tau_L = 0.2;
const double tau_R = 0.1;
// Minimum consumption of the agent
double c_bar = 3;
// Probability of survival
vector<double> Pa = {0.98828207, 0.986881  , 0.98568157, 0.98477179, 0.98408444,
                 0.98346398, 0.98280699, 0.98214304, 0.98144266, 0.98070591,
                 0.97995497, 0.97921927, 0.97847673, 0.97772003, 0.97696388,
                 0.97609828, 0.97513816, 0.97418624, 0.97324249, 0.97224116,
                 0.97106564, 0.96963635, 0.96793982, 0.96591924, 0.96354757,
                 0.9608778 , 0.95785464, 0.95435201, 0.95031767, 0.94577225,
                 0.94083612, 0.93553634, 0.9297738 , 0.92353596, 0.91685984,
                 0.90969997, 0.90218345, 0.89454917, 0.88689476, 0.87906873,
                 0.87050953, 0.86129715, 0.85202726, 0.84281219, 0.83325994,
                 0.82248918, 0.8102755 , 0.79702273, 0.78268864, 0.76694275,
                 0.74895781, 0.72875588, 0.70698773, 0.68374187, 0.6586057 ,
                 0.6303344 , 0.59924801, 0.56669902, 0.53306456, 0.49787087,
                 0.45973063, 0.41937498, 0.37879578, 0.33867486, 0.29885913,
                 0.25929479, 0.22041799, 0.18307063, 0.14827566, 0.11698081,
                 0};
// Deterministic income
vector<double> detEarning = {35.  ,  37.17,  39.33,  41.5 ,  43.67,  45.83,  48.  ,  50.17,
                         52.33,  54.5 ,  56.67,  58.83,  61.  ,  63.17,  65.33,  67.5 ,
                         69.67,  71.83,  74.  ,  76.17,  78.33,  80.5 ,  82.67,  84.83,
                         87.  ,  89.17,  91.33,  93.5 ,  95.67,  97.83, 100.  ,  97.  ,
                         94.  ,  91.  ,  88.  ,  85.  ,  82.  ,  79.  ,  76.  ,  73.  ,
                         70.  ,  63.  ,  56.  ,  49.  ,  42.  ,  35.  ,  35.  ,  35.  ,
                         35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  ,
                         35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  ,
                         35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  ,  35.  };


// return an evenly spaced 1-d grid of doubles.
vector<double> powspace(double first, double last, int len) {
    vector<double> result(len);
    first = pow(first,1.0/2);
    last = pow(last, 1.0/2);
    double step = (last-first) / (len - 1);
    for (int i=0; i<len; i++) { result[i] = pow(first + i*step,2); }
    return result;
}

// wealth
const int w_grid_size = 40;
const double w_lower = 3;
const double w_upper = 2000;
// 401k amount
const int n_grid_size = 20;
const double n_lower = 3;
const double n_upper = 1000;
// discrete state size
const int eSize = 2;
const int sSize = 2;
const int aSize = 2;
// generating grid
const vector<double> w_grid = powspace(w_lower, w_upper, w_grid_size);
const vector<double> n_grid = powspace(n_lower, n_upper, n_grid_size);
#endif //LIFECYCLE_CONSTANTS_H
