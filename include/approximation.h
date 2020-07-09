//
// Created by Leo on 6/25/20.
//

#ifndef LIFECYCLE_APPROXIMATION_H
#define LIFECYCLE_APPROXIMATION_H
#include "mlinterp.h"
#include "constants.h"
#include "state.h"

using namespace mlinterp;

class Prox{
public:
    vector<vector<vector<vector<vector<vector<double>>>>>>* Data;
    double ev(State x, int t);
    Prox(vector<vector<vector<vector<vector<vector<double>>>>>>& V);
};


double Prox::ev(State x, int t) {
    int time = t+1;
    constexpr int nxd = w_grid_size, nyd = n_grid_size, nd[] = { nxd, nyd };
    double zd[nxd * nyd];
    for(int i = 0; i < nxd; ++i) {
        for(int j = 0; j < nyd; ++j) {
            const int n = j + i * nyd;
            zd[n] = (*Data)[i][j][x.e][x.s][x.A][time];
        }
    }
    double xd[nxd];
    double yd[nyd];
    copy(w_grid.begin(), w_grid.end(), xd);
    copy(n_grid.begin(), n_grid.end(), yd);
    constexpr int ni = 1;
    double xi[1] = {x.w};
    double yi[1] = {x.n};
    double zi[1];
    interp(
            nd, ni,        // Number of points
            zd, zi,        // Output axis (z)
            xd, xi, yd, yi // Input axes (x and y)
    );
    return zi[0];
}

Prox::Prox(vector<vector<vector<vector<vector<vector<double>>>>>>& V) {
    Data = &V;
}

#endif //LIFECYCLE_APPROXIMATION_H
