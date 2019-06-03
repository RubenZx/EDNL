#include <iostream>
#include <utility>
#include <algorithm>
#include <tuple>
#include "materialesGrafos/alg_grafoPMC.h"
#include "materialesGrafos/alg_grafo_E-S.h"

template <typename tCoste>
using arista = typename GrafoP<tCoste>::arista;
template <typename tCoste>
using vertice = typename GrafoP<tCoste>::vertice;
template <typename tCoste>
using tCamino = typename GrafoP<tCoste>::tCamino;

using namespace std;

// PROBLEMA 1. Coste, origen y destino, del viaje más caro entre dos ciudades de
// un grafo acíclico
template <typename tCoste>
arista<tCoste> otraVezUnGrafoSA(const GrafoP<tCoste>& G)
{
    vertice<tCoste> v, w;
    arista<tCoste> viaje;
    size_t n = G.numVert();

    matriz<vertice<tCoste>> P{};    
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
laberinto(int N, const Lista<arista<tCoste>>& paredes, 
          const vertice<tCoste>& entrada, const vertice<tCoste>& salida)
{
    typedef typename GrafoP<tCoste>::vertice vertice;

    vertice v, w;
    vector<vertice> P;
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
    typename GrafoP<tCoste>::arista pared;
    for(auto it = paredes.primera(); it != paredes.fin(); it = paredes.siguiente(it))
    {
        pared = paredes.elemento(it);
        L[pared.orig][pared.dest] = L[pared.dest][pared.orig] = GrafoP<tCoste>::INFINITO;
    }
    //==========================================================================
    // Ya tenemos el laberinto formado, ahora tenemos que buscar el camino de 
    // coste mínimo entre la entrada y la salida, realizamos Dijkstra para
    // hallar el vector P entre el nodo 'entrada' y todos los demás, calculamos
    // el camino de coste mínimo del leberinto mediante la función camino
    //==========================================================================
    vector<tCoste> D = Dijkstra(L, entrada, P);     
    return make_pair(camino<tCoste>(entrada, salida, P), D[salida]);
}

// PROBLEMA 3. Devolver la cantidad a almacenar en cada ciudad y el coste mínimo
// total de almacenar dichos productos
template <typename tCoste>
pair<vector<unsigned>, tCoste> distribucion(const GrafoP<tCoste>& ciudades,           
                  const vertice<tCoste>& centro_prod, unsigned cantidad, 
                  const vector<tCoste>& capacidad, const vector<double>& subv)
{
    size_t n = subv.size();
    vector<unsigned> cants_ciud(n, 0);
    vector<double> subvenciones(subv);
    tCoste coste = 0;
    
    vector<vertice<tCoste>> P;  
    vector<tCoste> D = Dijkstra(ciudades, centro_prod, P);

    int i = 0;
    while(i<n && cantidad>0)
    {
        auto it = max_element(subvenciones.begin(), subvenciones.end());
        int ind = distance(subvenciones.begin(), it);
        if(capacidad[ind] <= cantidad)
        {
            cants_ciud[ind] = capacidad[ind];
            coste += D[ind] - (tCoste)((*it)*D[ind]/100);
            cantidad -= capacidad[ind]; 
            subvenciones[ind] = 0;
        }
        i++;
    }

    return make_pair(cants_ciud, coste);
}

// PROBLEMA 4. Devolver la distancia total en kilometros que han de recorrer tus
// caminones en un día
template <typename tCoste>
tCoste cementosZuelandia(const GrafoP<tCoste>& Zuelandia, 
                        const vector<unsigned>& parte, vertice<tCoste> capital)
{
    size_t n = Zuelandia.numVert();
    
    vector<vertice<tCoste>> P;
    vector<tCoste> D = Dijkstra(Zuelandia, capital, P);
    vector<tCoste> Dinv = DijkstraInv(Zuelandia, capital, P);
    
    tCoste coste = 0;
    for(vertice<tCoste> v = 0; v < n; v++)
        coste += (D[v] + Dinv[v]) * parte[v];
    
    return coste;
}

// PROBLEMA 5. Devuelva las ciudades a las que podría viajar nuestro viajero
enum transporte{
    carretera, tren, avion
};

template <typename tCoste>
vector<vertice<tCoste>> viajeroAlergico(const GrafoP<tCoste>& carretera, 
                                        const GrafoP<tCoste>& tren, 
                                        const GrafoP<tCoste>& avion,
                                        short money, transporte alergia, 
                                        vertice<tCoste> origen)
{
    size_t n = carretera.numVert();
    GrafoP<tCoste> costesMin(n);

    // Guardamos los grafos en un vector y eliminamos del que se sea alérgico
    vector<const GrafoP<tCoste>*> vGrafos{&carretera, &tren, &avion};
    vGrafos.erase(vGrafos.begin()+alergia);

    auto transporte1 = *vGrafos[0], transporte2 = *vGrafos[1];
    for (vertice<tCoste> v = 0; v < n; ++v) 
        for (vertice<tCoste> w = 0; w < n; ++w) 
            costesMin[v][w] = min(transporte1[v][w], transporte2[v][w]);
    
    vector<vertice<tCoste>> P, ciud;
    vector<tCoste> D = Dijkstra(costesMin, origen, P);
    for (vertice<tCoste> v = 0; v < D.size(); ++v)
        if (D[v] <= money && v != origen)
            ciud.push_back(v);
    
    return ciud;
}

// PROBLEMA 6. Devuelva la matriz de costes mínimos de viajar entre cualquier 
// ciudad de una ciudad, usando dos tipos de transporte, y pudiendose cambiar 
// de transporte en una única ciudad
template <typename tCoste>
matriz<tCoste> transportesSinTaxi(const GrafoP<tCoste>& tren, 
                                  const GrafoP<tCoste>& bus,
                                  const vertice<tCoste>& estacion)
{
    size_t n = tren.numVert();
    matriz<tCoste> costesMin(n);
    matriz<vertice<tCoste>> M;
    matriz<tCoste> Ftren{Floyd(tren, M)}, Fbus{Floyd(bus, M)};
    
    vertice<tCoste> v, w;
    for(v=0; v<n; ++v)
        for(w=0; w<n; ++w)    
            costesMin[v][w] = min({Ftren[v][w], Fbus[v][w], 
            Ftren[v][estacion]+Fbus[estacion][w], Fbus[v][estacion]+Ftren[estacion][w]});

    return costesMin;
}

// PROBLEMA 7. Calcula la ruta y el coste mínimo para viajar entre las ciudades 
// origen y destino en las condiciones siguientes:
//    a) La ciudad origen solo dispone de tren.
//    b) La ciudad destino solo dispone de bus.
//    a) Solo se puede cambiar de medio de transporte en las estaciones dadas.
template <typename tCoste>
tuple<tCoste, tCamino<tCoste>> transportesSinTaxi2(
    const GrafoP<tCoste>& tren, const GrafoP<tCoste>& bus,
    const vertice<tCoste>& est1, const vertice<tCoste>& est2,
    const vertice<tCoste>& orig, const vertice<tCoste>& dest)
{
    vector<vertice<tCoste>> caminosT, caminosB;
    vector<tCoste> Dtren{Dijkstra(tren, orig, caminosT)};
    vector<tCoste> Dbus{DijkstraInv(bus, dest, caminosB)};
    
    tCoste cFinal; 
    tCamino<tCoste> pFinal;

    tCoste coste_est1 = Dtren[est1] + Dbus[est1];
    tCoste coste_est2 = Dtren[est2] + Dbus[est2];
    if(coste_est1 < coste_est2)
    {
        cFinal = coste_est1;
        pFinal = camino<tCoste>(orig, est1, caminosT);
        pFinal.eliminar(pFinal.anterior(pFinal.fin()));
        pFinal += caminoInv<tCoste>(dest, est1, caminosB);
    }
    else
    { 
        cFinal = coste_est2;
        pFinal = camino<tCoste>(orig, est2, caminosT);
        pFinal.eliminar(pFinal.anterior(pFinal.fin()));
        pFinal += caminoInv<tCoste>(dest, est2, caminosB);
    }

    return make_tuple(cFinal, pFinal);
}

/**
 * @todo PROBLEMA 8
 * @body 
 */

/**
 * @todo PROBLEMA 9
 * @body 
 */

/**
 * @todo PROBLEMA 10
 * @body 
 */

/**
 * @todo PROBLEMA 11
 * @body 
 */

/**
 * @todo PROBLEMA 12
 * @body 
 */

/**
 * @todo PROBLEMA 13
 * @body 
 */