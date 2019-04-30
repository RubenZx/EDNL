#include <iostream>
#include <algorithm>
#include "materialesGrafos/alg_grafoPMC.h"
#include "materialesGrafos/grafoPMC.h"
#include "materialesGrafos/matriz.h"
#include "materialesGrafos/alg_grafo_E-S.h"

using namespace std;

// PROBLEMA 2. Calcula el diametro de un grafo. 
// Se define como la suma de las distancias mínimas a los dos nodos más alejados
// del pseudocentro del grafo, el cual, es el nodo que minimiza la suma de las 
// distancias mínimas a sus dos nodos más alejados.
template <typename tCoste>
size_t diametro(const GrafoP<tCoste>& G)
{
    matriz<typename GrafoP<tCoste>::vertice> P{};    
    matriz<tCoste> F = Floyd(G, P);
    size_t n = F.dimension();
    
    vector<size_t> diam(n, 0);
    vector<size_t>::iterator maxElementIt;
    for(size_t i=0; i<n; ++i)
    {
        maxElementIt = max_element(F[i].begin(),F[i].end());
        diam[i] += *maxElementIt; 
        *maxElementIt = 0;
        diam[i] += *max_element(F[i].begin(), F[i].end());
    }
    return *min_element(diam.begin(), diam.end());
}

// PROBLEMA 3. Aplicamos Floydd, y vemos que ninguno de sus elementos sea inf.

// Problema 4. 
/*
    a) Es dirigido.
    b) Ciudades a inf.
    c) Caminos a inf.
    d) Dijkstra y DijkstraInv para ver como entrar y salir de la capital entre 
       dos caminos mínimos.
*/

int main()
{
    GrafoP<size_t> diam("diametro.txt");
    size_t d = diametro(diam);
    
    cout << "DIAMETRO: " << d << endl;
    return 0;
}