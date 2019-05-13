#include <iostream>
#include <utility>
#include <algorithm>
#include "materialesGrafos/alg_grafoPMC.h"
#include "materialesGrafos/alg_grafo_E-S.h"

using namespace std;

// PROBLEMA 1. Coste, origen y destino, del viaje más caro entre dos ciudades de
// un grafo acíclico
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

// PROBLEMA 2. Laberinto, calcular el camino para ir de la entrada a la salida y
// su longitud
template <typename tCoste>
pair<typename GrafoP<tCoste>::tCamino, tCoste> 
laberinto(int N, const Lista<typename GrafoP<tCoste>::arista>& paredes, 
          const typename GrafoP<tCoste>::vertice& entrada, 
          const typename GrafoP<tCoste>::vertice& salida)
{
    typedef typename GrafoP<tCoste>::vertice vertice;
    typedef GrafoP<tCoste>::INFINITO inf;
    
    vertice v, w;
    vector<vertice> P;
    pair<typename GrafoP<tCoste>::tCamino , tCoste> salidaLab;
    GrafoP<tCoste> L(N*N);
    //==========================================================================
    // PREPROCESADO DE LOS DATOS DE ENTRADA (Construcción del laberinto):
    //==========================================================================
    // Ponemos todos los caminos posibles con el mismo coste(suponemos que cada 
    // arista entre dos nodos tiene coste 1)
    for(int ind = 0; ind < N*N; ind++)
    {
        if(ind < N*(N-1))
            L[ind][ind+N] = L[ind+N][ind] = 1;
        if(ind % N != N-1)
            L[ind][ind+1] = L[ind+1][ind] = 1;
    }
    // Ponemos cada pared como aristas a INFINITO, es decir, no podremos usarla 
    // para ir de un nodo a otro´
    arista<tCoste> pared;
    for(auto it = paredes.primera(); it != paredes.fin(); it = paredes.siguiente(it))
    {
        pared = paredes.elemento(it);
        L[pared.orig][pared.dest] = L[pared.dest][pared.orig] = inf;
    }
    //==========================================================================
    // Ya tenemos el laberinto formado, ahora tenemos que buscar el camino de 
    // coste mínimo entre la entrada y la salida, realizamos Dijkstra para
    // hallar el vector P entre el nodo 'entrada' y todos los demás, calculamos
    // el camino de coste mínimo del leberinto mediante la función camino
    //==========================================================================
    vector<tCoste> D = Dijkstra(L, entrada, P);     
    salidaLab.second = D[salida];                   // Coste mín, de salir
    salidaLab.first  = camino(entrada, salida, P);  // Camino más óptimo
    
    return salidaLab;
}

// PROBLEMA 3. Devolver la cantidad a almacenar en cada ciudad y el coste mínimo
// total de almacenar dichos productos
template <typename tCoste>
pair<vector<unsigned>, tCoste> distribucion(const GrafoP<tCoste>& ciudades,           
                  const typename GrafoP<tCoste>::vertice& centro_prod, 
                  unsigned cantidad, // Cantidad de producto total
                  const vector<tCoste>& capacidad,
                  const vector<double>& subv)
{
    size_t n = subv.size();
    vector<unsigned> cants_ciud(n, 0);
    vector<double> subvenciones(subv);
    tCoste coste = 0;
    
    vector<typename GrafoP<tCoste>::vertice> P;  
    vector<tCoste> D = Dijkstra(ciudades, centro_prod, P);

    int i = 0;
    while(i<n || cantidad!=0)
    {
        auto it = max_element(subvenciones.begin(), subvenciones.end());
        int ind = distance(subvenciones.begin(), it);
        if(capacidad[ind] <= cantidad)
        {
            cants_ciud[ind] = capacidad[ind];
            coste += D[ind] - (tCoste)((*it)*D[ind]/100);
            cantidad -= capacidad[ind]; 
            subvenciones.erase(it);
        }
        i++;
    }

    return make_pair(cants_ciud, coste);
}

int main()
{
    unsigned inf = GrafoP<unsigned>::INFINITO;

    GrafoP<unsigned> G("grafo.txt");
    GrafoP<unsigned> Dist("dist.txt");
    //==========================================================================
    // PROBLEMA 1
    //==========================================================================
    GrafoP<unsigned>::arista viaje;
    //==========================================================================
    // PROBLEMA 2
    //==========================================================================
    int N = 3;
    vector<GrafoP<unsigned>::arista> paredes{GrafoP<unsigned>::arista(0,1,inf),
                                             GrafoP<unsigned>::arista(4,5,inf)};
    GrafoP<unsigned>::vertice entrada{3}, salida{8};
    // pair<GrafoP<unsigned>::tCamino , unsigned> lab(laberinto(N, paredes, entrada, salida));
    
    //==========================================================================
    // PROBLEMA 3:
    //==========================================================================
    pair<vector<unsigned>, unsigned> p;
    GrafoP<unsigned>::vertice centro{1};
    unsigned cantidad{80};
    vector<unsigned> capacidad{20,0,10,50,40,10};
    vector<double> subv{25,0,30,15,10,20};

    p = distribucion(Dist, centro, cantidad, capacidad, subv);

    viaje = otraVezUnGrafoSA(G);
    cout << "\n -> OTRAVEZUNGRAFOSA PROPONE COMO VIAJE..." << viaje.coste;
    // cout << "\n -> SALIDA DEL LABERINTO... ";
    // Imprimir aqui los elementos de la lista tCamino que devuelve laberinto
    
    cout << "\n -> DISTRIBUCION ..." << endl;
    cout << "   *** COSTE *** " << p.second << endl;
    cout << "      - CIUDADES-CANTIDAD:" << endl;
    for(int i=0; i<p.first.size(); ++i)
        cout << "      - Ciudad: " << i << " Cantidad: " << p.first[i] << endl;
    
    cout << endl;

    return 0;
}