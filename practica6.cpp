#include <iostream>
#include <algorithm>
#include "materialesGrafos/alg_grafoPMC.h"
#include "materialesGrafos/alg_grafo_E-S.h"

using namespace std;

// PROBLEMA 2. Calcula el diametro de un grafo. 
// Se define como la suma de las distancias mínimas a los dos nodos más alejados
// del pseudocentro del grafo, el cual, es el nodo que minimiza la suma de las 
// distancias mínimas a sus dos nodos más alejados.
template <typename tCoste>
int diametro(const GrafoP<tCoste>& G)
{
    matriz<typename GrafoP<tCoste>::vertice> P{};    
    matriz<tCoste> F = Floyd(G, P);
    size_t n = F.dimension();
    
    vector<unsigned> diam(n, 0);
    vector<unsigned>::iterator maxElementIt;
    for(int i=0; i<n; ++i)
    {
        maxElementIt = max_element(F[i].begin(),F[i].end());
        diam[i] += *maxElementIt; 
        *maxElementIt = 0;
        diam[i] += *max_element(F[i].begin(), F[i].end());
    }
    return *min_element(diam.begin(), diam.end());
}

// PROBLEMA 3. Dado un grafo función que nos calcula si éste es acíclico o no.
template <typename tCoste>
bool aciclico(const GrafoP<tCoste>& G)
{
    typedef typename GrafoP<tCoste>::vertice vertice; 

    matriz<vertice> mV(G.numVert());
    matriz<tCoste> mC = Floyd_Ej3(G, mV); // Flyd sin poner toda la diagonal a 0
    
    bool aciclo = true;
    vertice v,w;
    for(v = 0; v < mC.dimension() && aciclo ; v++)
        if(mC[v][v] != GrafoP<tCoste>::INFINITO)     
            aciclo = false;
    
    return aciclo;
}

// Problema 4. 
/*
    a) Es dirigido.
    b) Ciudades a inf.
    c) Caminos a inf.
    d) Dijkstra y DijkstraInv para ver como entrar y salir de la capital entre 
       dos caminos mínimos.
*/
template <typename tCoste>
matriz<tCoste> Zuelandia(GrafoP<tCoste>& G, 
                         const vector<tCoste>& ciudades, 
                         const vector<typename GrafoP<tCoste>::arista>& carreteras,
                         const typename GrafoP<tCoste>::vertice& capital)
{
    typedef typename GrafoP<tCoste>::vertice vertice;
    size_t n = G.numVert();

    // Eliminamos las ciudades tomadas por los rebeldes de nuestro Grafo
    vector<tCoste>::iterator it;
    for(it.begin() : ciudades)
        fill(G[*it].begin(), G[*it].end()) = GrafoP<tCoste>::INFINITO;
    
    // Eliminamos las carreteras tomadas por los rebeldes
    for(it.begin() : carreteras)
    {
        G[*it.orig][*it.dest] = GrafoP<tCoste>::INFINITO;
        G[*it.dest][*it.orig] = GrafoP<tCoste>::INFINITO;
    }

    vector<typename GrafoP<tCoste>::vertice> P{};
    vector<tCoste> D    = Dijkstra(G, capital, P);
    vector<tCoste> Dinv = DijkstraInv(G, capital, P);
    
    vertice v, w;
    for(v = 0; v < n ; v++)
        for(w = 0; w < n; w++)
            if(v != w)
                G[v][w] = suma(D[w], Dinv[v]);
    
    matriz<tCoste> mZuelandia(n);
    
    for(v = 0; v < n ; v++)
        for(w = 0; w < n; w++)
            mZuelandia[v][w] = G[v][w];
    
    return mZuelandia;
}


int main()
{
    GrafoP<unsigned> G("diametro.txt");
    string acicl;
    
    if(aciclico(G)) acicl = "Si";
    else acicl = "No";
    
    

    cout << "\n -> DIAMETRO.... " << diametro(G);
    cout << "\n -> ACICLICO?... " << acicl;
    cout << "\n -> Zuelandia..." << endl;
    return 0;
}