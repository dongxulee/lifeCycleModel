#include "functions.h"
#include "state.h"
#include "action.h"
#include "constants.h"
#include "tbb/tbb.h"
 
using namespace tbb;
using namespace std::chrono;

// Function used to write the results to txt file 
void saveSolution(vector<vector<vector<vector<vector<vector<double>>>>>>& data, string name);

int main() {
    // define value functions and policy functions
    // Create a 6D array that is 100 x 50 x 2 x 2 x 2 x70
    vector<vector<vector<vector<vector<vector<double>>>>>> Vgrid(w_grid_size,
            vector<vector<vector<vector<vector<double>>>>>(n_grid_size,
                    vector<vector<vector<vector<double>>>>(eSize,
                            vector<vector<vector<double>>>(sSize,
                                    vector<vector<double>>(aSize,
                                            vector<double>(T_max+1))))));

    vector<vector<vector<vector<vector<vector<double>>>>>> cgrid(w_grid_size,
            vector<vector<vector<vector<vector<double>>>>>(n_grid_size,
                    vector<vector<vector<vector<double>>>>(eSize,
                            vector<vector<vector<double>>>(sSize,
                                    vector<vector<double>>(aSize,
                                            vector<double>(T_max+1))))));

    vector<vector<vector<vector<vector<vector<double>>>>>> bgrid(w_grid_size,
            vector<vector<vector<vector<vector<double>>>>>(n_grid_size,
                    vector<vector<vector<vector<double>>>>(eSize,
                            vector<vector<vector<double>>>(sSize,
                                    vector<vector<double>>(aSize,
                                            vector<double>(T_max+1))))));

    vector<vector<vector<vector<vector<vector<double>>>>>> kgrid(w_grid_size,
            vector<vector<vector<vector<vector<double>>>>>(n_grid_size,
                    vector<vector<vector<vector<double>>>>(eSize,
                            vector<vector<vector<double>>>(sSize,
                                    vector<vector<double>>(aSize,
                                            vector<double>(T_max+1))))));


    // Define terminal condition
    for(int i=0; i < eSize; i++){
        for(int j=0; j < sSize; j++){
            for(int k=0; k < aSize; k++){
                for(int w=0; w < w_grid_size; w++){
                    for(int n=0; n < n_grid_size; n++){
                        Vgrid[w][n][i][j][k][T_max] = R(State(w_grid[w],n_grid[n],i,j,0), Action(0,0,0));
                        cgrid[w][n][i][j][k][T_max] = 0;
                        bgrid[w][n][i][j][k][T_max] = 0;
                        kgrid[w][n][i][j][k][T_max] = 0;
                    }
                }
            }
        }
    }


    auto start = high_resolution_clock::now();
    // Value iteration part
    for(int t = T_max-1; t > T_min-1; t--){
        cout << t << endl;
        for(int i=0; i < eSize; i++){
            for(int j=0; j < sSize; j++){
                for(int k=0; k < aSize; k++){
                    parallel_for (0, w_grid_size, [&](int w){
                    // for(int w=0; w < w_grid_size; w++){
                        for(int n=0; n < n_grid_size; n++){
                            double value;
                            Action a;
                            State x(w_grid[w],n_grid[n],i,j,k);
                            tie(value, a) = V(x, t, Prox(Vgrid));
                            Vgrid[w][n][i][j][k][t] = value;
                            cgrid[w][n][i][j][k][t] = a.c;
                            bgrid[w][n][i][j][k][t] = a.b;
                            kgrid[w][n][i][j][k][t] = a.k;
                        }
                    });
                }
            }
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    cout << "Time taken by function: "
         << duration.count() << "seconds" << endl;

    saveSolution(Vgrid, "Vgrid");
    saveSolution(cgrid, "cgrid");
    saveSolution(bgrid, "bgrid");
    saveSolution(kgrid, "kgrid");
}













void saveSolution(vector<vector<vector<vector<vector<vector<double>>>>>>& data, string name)
{
    ofstream outFile;
    outFile.open("output/" + name + ".txt"); 
    if (outFile.fail()){
        cout << "Unable to open output file" << endl;
        exit(1); 
    }
    else{
        // cout information
        for(int w = 0; w < w_grid_size; w++){
            for(int n=0; n < n_grid_size; n++){
                for(int e=0; e < eSize; e++){
                    for(int s=0; s < sSize; s++){
                        for(int a=0; a < aSize; a++){
                            for(int t=T_min; t<T_max+1; t++){
                                outFile << data[w][n][e][s][a][t] << "\n";
                            }
                        }
                    }
                }
            }
        }
        cout << "Data stored in " + name + ".txt" << endl;
        outFile.close();
    }
}
