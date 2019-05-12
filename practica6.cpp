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
    int max1 = 0, max2 = 0;
    for(int i=0; i<n; ++i)
    {
        for(int j=0; j<n; ++j)
            if(F[i][j] > max1 && max1 <= max2) max1 = F[i][j];
            else if(F[i][j] > max2) max2 = F[i][j];               
        diam[i] = max1 + max2;
        max1 = max2 = 0;
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
matriz<tCoste> Zuelandia(const GrafoP<tCoste>& G, 
                         const vector<tCoste>& ciudades, 
                         const vector<typename GrafoP<tCoste>::arista>& carreteras,
                         const typename GrafoP<tCoste>::vertice& capital)
{
    GrafoP<tCoste> Z(G);
    typedef typename GrafoP<tCoste>::vertice vertice;
    size_t n = G.numVert();
    
    // Eliminamos las ciudades tomadas por los rebeldes de nuestro Grafo
    //typename vector<tCoste>::iterator it;
    for(auto it : ciudades)
        for(int j=0; j<n; ++j)
            Z[j][it] = GrafoP<tCoste>::INFINITO;
    
    // Eliminamos las carreteras tomadas por los rebeldes
    for(auto it : carreteras)
    {
        Z[it.orig][it.dest] = GrafoP<tCoste>::INFINITO ;
        Z[it.dest][it.orig] = GrafoP<tCoste>::INFINITO ;
    }

    // Llamamos a Dijkstra y a DijkstraInv para ver los caminos más óptimos de
    // entrada y salida de la capital
    vector<vertice> P{};
    vector<tCoste> D    = Dijkstra(Z, capital, P);
    vector<tCoste> Dinv = DijkstraInv(Z, capital, P);
    
    matriz<tCoste> mZuelandia(n);
    vertice v, w;
    for(v = 0; v < n ; v++)
        for(w = 0; w < n; w++)
            if(v != w)
                mZuelandia[v][w] = suma(D[w], Dinv[v]);
            else
                mZuelandia[v][w] = GrafoP<tCoste>::INFINITO;
    
    return mZuelandia;
}

int main()
{
    typedef GrafoP<unsigned>::vertice vertice;
    typedef GrafoP<unsigned>::arista arista;

    GrafoP<unsigned> G("Grafo1.txt");
    string acicl;
    matriz<unsigned> Z(G.numVert());
    vector<unsigned> ciudades{6};
    vector<arista> carreteras{GrafoP<unsigned>::arista(2,5,10)};
    vertice capital{2};
    
    if(aciclico(G)) acicl = "Si";
    else acicl = "No";
    
    vector<GrafoP<unsigned>::vertice> P{};

    cout << "\n -> DIAMETRO.... " << diametro(G);
    cout << "\n -> ACICLICO?... " << acicl;
    cout << "\n -> Zuelandia antes de la rebelion.....\n" << G;
    
    Z = Zuelandia(G, ciudades, carreteras, capital);
    cout << "\n -> Zuelandia despues de la rebelion...\n" << Z << endl;

    return 0;
}