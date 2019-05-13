#include <iostream>
#include <utility>
#include <algorithm>
#include "materialesGrafos/alg_grafoPMC.h"
#include "materialesGrafos/alg_grafo_E-S.h"

using namespace std;


int main(int  args, char ** argv)
{
    int N = atoi(argv[1]);
    GrafoP<unsigned> L(N*N);
    cout << L;
/*
    int ind = 0;
    while(ind < N*N)
    {
        if(ind % N < N - 1) 
            L[ind][ind + 1] = 1;    // Derecha
        if((ind+1) % N > 1) 
            L[ind][ind - 1] = 1;    // Izquierda
        if(ind > N - 1) 
            L[ind][ind-N] = 1;      // Arriba
        if(ind < N*N - N) 
            L[ind][ind+N] = 1;      // Abajo
        ind++;
    }
*/
    
    for(int ind = 0; ind < N*N; ind++)
    {
        if(ind < N*(N-1))
            L[ind][ind+N] = L[ind+N][ind] = 1;
        if(ind % N != N-1)
            L[ind][ind+1] = L[ind+1][ind] = 1;
    }
    cout << endl;
    cout << L;
}