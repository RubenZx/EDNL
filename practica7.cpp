#include <iostream>
#include <algorithm>
#include "materialesGrafos/alg_grafoPMC.h"
#include "materialesGrafos/alg_grafo_E-S.h"

using namespace std;

// PROBLEMA 1. Coste, origen y destino, del viaje más caro entre dos ciudades de
// un grafo acíclico.
template <typename tCoste>
typename GrafoP<tCoste>::arista otraVezUnGrafoSA(const GrafoP<tCoste>& G)
{
    typename GrafoP<tCoste>::vertice v, w;
    typename GrafoP<tCoste>::arista viaje;
    size_t n = G.numVert();

    matriz<typename GrafoP<tCoste>::vertice> P{};    
    matriz<tCoste> F = FloydMax(G, P);  // Floyd modificado para los caminos de coste máximo
    
    viaje.coste = 0;
    for(v = 0; v < n; v++)
        for(w = 0; w < n; w++)
            if(F[v][w] > viaje.coste && F[v][w] != GrafoP<tCoste>::INFINITO)
            {
                viaje.coste = F[v][w];
                viaje.orig  = v;
                viaje.dest  = w;
            }
            
    return viaje;
}

// PROBLEMA 2. 
template <typename tCoste>
typename GrafoP<tCoste>::tCamino laberinto(int N, 
                                  const vector<typename GrafoP<tCoste>::arista>& paredes, 
                                  typename GrafoP<tCoste>::vertice entrada, 
                                  typename GrafoP<tCoste>::vertice salida)
{
    GrafoP<tCoste> L(N*N);
    typename GrafoP<tCoste>::vertice v, w;
    int ind = 0;

    //==========================================================================
    // PREPROCESADO DE LOS DATOS DE ENTRADA (Construcción del laberinto):
    //==========================================================================
    // Ponemos todos los caminos posibles con el mismo coste
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
    
    // Ponemos cada pared como aristas a INFINITO, es decir, no podremos usarla 
    // para ir de un nodo a otro
    for(auto p : paredes)
    {
        L[p.orig][p.dest] = GrafoP<tCoste>::INFINITO;
        L[p.dest][p.orig] = GrafoP<tCoste>::INFINITO;
    }

    //==========================================================================
    // Ya tenemos el laberinto formado, ahora tenemos que buscar el camino de 
    // coste mínimo entre la entrada y la salida    
}

int main()
{
    GrafoP<unsigned> G("grafo.txt");
    GrafoP<unsigned>::arista viaje;
    int N = 3;
    vector<GrafoP<unsigned>::arista> paredes;
    GrafoP<unsigned>::vertice entrada, salida;
    
    viaje = otraVezUnGrafoSA(G);
    cout << "\n -> OTRAVEZUNGRAFOSA PROPONE COMO VIAJE..." << viaje.coste;
    cout << endl;
    
    return 0;
}