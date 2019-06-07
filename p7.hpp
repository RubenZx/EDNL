#include <algorithm>
#include <iostream>
#include <tuple>
#include <utility>
#include "materialesGrafos/alg_grafoPMC.h"
#include "materialesGrafos/alg_grafo_E-S.h"
#include "tools.hpp"

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
arista<tCoste> otraVezUnGrafoSA(const GrafoP<tCoste>& G) {
    vertice<tCoste> v, w;
    arista<tCoste> viaje;
    size_t n = G.numVert();

    matriz<vertice<tCoste>> P{};
    matriz<tCoste> F =
        FloydMax(G, P);  // Floyd modificado para los caminos de coste máximo

    viaje.coste = 0;
    for (v = 0; v < n; v++)
        for (w = 0; w < n; w++)
            if (F[v][w] > viaje.coste && F[v][w] != GrafoP<tCoste>::INFINITO) {
                viaje.coste = F[v][w];
                viaje.orig = v;
                viaje.dest = w;
            }

    return viaje;
}

// PROBLEMA 2. Laberinto, calcular el camino para ir de la entrada a la salida y
// su longitud
template <typename tCoste>
pair<typename GrafoP<tCoste>::tCamino, tCoste> laberinto(
    int N, const Lista<arista<tCoste>>& paredes, const vertice<tCoste>& entrada,
    const vertice<tCoste>& salida) {
    typedef typename GrafoP<tCoste>::vertice vertice;

    vertice v, w;
    vector<vertice> P;
    GrafoP<tCoste> L(N * N);
    //==========================================================================
    // PREPROCESADO DE LOS DATOS DE ENTRADA (Construcción del laberinto):
    //==========================================================================
    // Ponemos todos los caminos posibles con el mismo coste(suponemos que cada
    // arista entre dos nodos tiene coste 1)
    for (int ind = 0; ind < N * N; ind++) {
        if (ind < N * (N - 1)) L[ind][ind + N] = L[ind + N][ind] = 1;
        if (ind % N != N - 1) L[ind][ind + 1] = L[ind + 1][ind] = 1;
    }
    // Ponemos cada pared como aristas a INFINITO, es decir, no podremos usarla
    // para ir de un nodo a otro´
    typename GrafoP<tCoste>::arista pared;
    for (auto it = paredes.primera(); it != paredes.fin();
         it = paredes.siguiente(it)) {
        pared = paredes.elemento(it);
        L[pared.orig][pared.dest] = L[pared.dest][pared.orig] =
            GrafoP<tCoste>::INFINITO;
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
                                            const vertice<tCoste>& centro_prod,
                                            unsigned cantidad,
                                            const vector<tCoste>& capacidad,
                                            const vector<double>& subv) {
    size_t n = subv.size();
    vector<unsigned> cants_ciud(n, 0);
    vector<double> subvenciones(subv);
    tCoste coste = 0;

    vector<vertice<tCoste>> P;
    vector<tCoste> D = Dijkstra(ciudades, centro_prod, P);

    int i = 0;
    while (i < n && cantidad > 0) {
        auto it = max_element(subvenciones.begin(), subvenciones.end());
        int ind = distance(subvenciones.begin(), it);
        if (capacidad[ind] <= cantidad) {
            cants_ciud[ind] = capacidad[ind];
            coste += D[ind] - (tCoste)((*it) * D[ind] / 100);
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
                         const vector<unsigned>& parte,
                         vertice<tCoste> capital) {
    size_t n = Zuelandia.numVert();

    vector<vertice<tCoste>> P;
    vector<tCoste> D = Dijkstra(Zuelandia, capital, P);
    vector<tCoste> Dinv = DijkstraInv(Zuelandia, capital, P);

    tCoste coste = 0;
    for (vertice<tCoste> v = 0; v < n; v++)
        coste += (D[v] + Dinv[v]) * parte[v];

    return coste;
}

// PROBLEMA 5. Devuelva las ciudades a las que podría viajar nuestro viajero
enum transporte { carretera, tren, avion };

template <typename tCoste>
vector<vertice<tCoste>> viajeroAlergico(const GrafoP<tCoste>& carretera,
                                        const GrafoP<tCoste>& tren,
                                        const GrafoP<tCoste>& avion,
                                        short money, transporte alergia,
                                        vertice<tCoste> origen) {
    size_t n = carretera.numVert();
    GrafoP<tCoste> costesMin(n);

    // Guardamos los grafos en un vector y eliminamos del que se sea alérgico
    vector<const GrafoP<tCoste>*> vGrafos{&carretera, &tren, &avion};
    vGrafos.erase(vGrafos.begin() + alergia);

    auto transporte1 = *vGrafos[0], transporte2 = *vGrafos[1];
    for (vertice<tCoste> v = 0; v < n; ++v)
        for (vertice<tCoste> w = 0; w < n; ++w)
            costesMin[v][w] = min(transporte1[v][w], transporte2[v][w]);

    vector<vertice<tCoste>> P, ciud;
    vector<tCoste> D = Dijkstra(costesMin, origen, P);
    for (vertice<tCoste> v = 0; v < D.size(); ++v)
        if (D[v] <= money && v != origen) ciud.push_back(v);

    return ciud;
}

// PROBLEMA 6. Devuelva la matriz de costes mínimos de viajar entre cualquier
// ciudad de una ciudad, usando dos tipos de transporte, y pudiendose cambiar
// de transporte en una única ciudad
template <typename tCoste>
matriz<tCoste> transportesSinTaxi(const GrafoP<tCoste>& tren,
                                  const GrafoP<tCoste>& bus,
                                  const vertice<tCoste>& estacion) {
    size_t n = tren.numVert();
    matriz<tCoste> costesMin(n);
    matriz<vertice<tCoste>> M;
    matriz<tCoste> Ftren{Floyd(tren, M)}, Fbus{Floyd(bus, M)};

    vertice<tCoste> v, w;
    for (v = 0; v < n; ++v)
        for (w = 0; w < n; ++w)
            costesMin[v][w] = min({Ftren[v][w], Fbus[v][w],
                                   Ftren[v][estacion] + Fbus[estacion][w],
                                   Fbus[v][estacion] + Ftren[estacion][w]});

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
    const vertice<tCoste>& orig, const vertice<tCoste>& dest) {
    vector<vertice<tCoste>> caminosT, caminosB;
    vector<tCoste> Dtren{Dijkstra(tren, orig, caminosT)};
    vector<tCoste> Dbus{DijkstraInv(bus, dest, caminosB)};

    tCoste cFinal;
    tCamino<tCoste> pFinal;

    tCoste coste_est1 = Dtren[est1] + Dbus[est1];
    tCoste coste_est2 = Dtren[est2] + Dbus[est2];
    if (coste_est1 < coste_est2) {
        cFinal = coste_est1;
        pFinal = camino<tCoste>(orig, est1, caminosT);
        pFinal.eliminar(pFinal.anterior(pFinal.fin()));
        pFinal += caminoInv<tCoste>(dest, est1, caminosB);
    } else {
        cFinal = coste_est2;
        pFinal = camino<tCoste>(orig, est2, caminosT);
        pFinal.eliminar(pFinal.anterior(pFinal.fin()));
        pFinal += caminoInv<tCoste>(dest, est2, caminosB);
    }

    return make_tuple(cFinal, pFinal);
}

/**
 * @todo PROBLEMA 8
 * @body Subprograma que calcule la tarifa mínima de un viaje con las siguientes
 * condiciones: a) En todos tus viajes se hará como máximo UN solo transbordo b)
 * Tenemos combinaciones de viajes entre TREN y/o BUS, en N ciudades
 */
template <typename tCoste>
tCoste unSoloTransbordo(const GrafoP<tCoste>& tren, const GrafoP<tCoste>& bus,
                        vertice<tCoste> origen, vertice<tCoste> destino) {
    size_t n = bus.numVert();
    vector<vertice<tCoste>> P;
    vector<tCoste> DTren{Dijkstra(tren, origen, P)};
    vector<tCoste> DBus{Dijkstra(bus, origen, P)};
    vector<tCoste> DinvTren{DijkstraInv(tren, destino, P)};
    vector<tCoste> DinvBus{DijkstraInv(bus, destino, P)};

    // Vector para sumar los elementos y ver cual es el coste minimo
    vector<tCoste> vAux(2 * n);
    for (size_t i = 0; i < n; ++i) {
        vAux[i] = DTren[i] + DinvBus[i];
        vAux[n + i] = DBus[i] + DinvTren[i];
    }

    return *min_element(vAux.begin(), vAux.end());
}

/**
 * @todo PROBLEMA 9
 * @body Realiza un subprograma calcule el camino y el coste mínimo para ir de
 * la ciudad origen a la ciudad destino. Teniendo en cuenta que el coste del
 * transbordo de un medio de transporte a otor es constante e igual para todas
 * las ciudades, y que tenemos dos grafos (tren y bus).
 */
template <typename tCoste>
tuple<tCoste, tCamino<tCoste>> transporteConTaxi(const GrafoP<tCoste>& tren,
                                                 const GrafoP<tCoste>& bus,
                                                 vertice<tCoste> origen,
                                                 vertice<tCoste> destino) {
    size_t n = tren.numVert();

    tCamino<tCoste> caminito;
    vector<vertice<tCoste>> P;
    GrafoP<tCoste> bigGraph{makeBigGraph({tren, bus}, 1)};
    vector<tCoste> D{Dijkstra(bigGraph, origen, P)};

    if (D[destino] < D[destino + n])  // Nos quedamos con el coste mínimo
        caminito = camino<tCoste>(origen, destino, P);
    else
        caminito = camino<tCoste>(origen, destino + n, P);

    for (auto it = caminito.primera(); it != caminito.fin();
         it = caminito.siguiente(it)) {
        if (caminito.elemento(it) > n - 1) caminito.elemento(it) -= n;
    }

    // Cuando en un nodo hay un transborodo, en el camino se verán dos nodos
    // iguales seguidos.
    return {min(D[destino], D[destino + n]), caminito};
}

/**
 * @todo PROBLEMA 10
 * @body Subprograma que entre 3 grafos distintos y varios tipos de costes,
 * calcule el camino y el coste mínimo para ir de la ciudad origen a la ciudad
 * destino
 */
template <typename tCoste>
tuple<tCoste, tCamino<tCoste>> transporteConTaxi2(const GrafoP<tCoste>& tren,
                                                  const GrafoP<tCoste>& bus,
                                                  const GrafoP<tCoste>& avion,
                                                  vertice<tCoste> origen,
                                                  vertice<tCoste> destino,
                                                  tCoste cTB, tCoste cATB) {
    size_t n = tren.numVert();
    tCamino<tCoste> caminito;
    vector<tCoste> v_cTB(n, cTB), v_cATB(n, cATB);
    GrafoP<tCoste> bigGraph{
        makeBigGraph<tCoste>({tren, bus, avion}, {v_cTB, v_cATB, v_cATB})};
    vector<vertice<tCoste>> P;
    vector<tCoste> D = Dijkstra(bigGraph, origen, P);

    // Nos quedamos con el coste mínimo y el indice del transporte
    auto [ind, vMin] = minIndex(
        vector<tCoste>{D[destino], D[destino + n], D[destino + 2 * n]});

    // Buscamos el camino y sumamos al destino en función del transporte en el
    // que se enceuntre
    bool salir = false;
    size_t cont = 0;
    while (!salir) {
        if (ind == cont) {
            caminito = camino<tCoste>(origen, destino + ind * n, P);
            salir = true;
        }
        ++cont;
    }

    // A continuación dejamos los valores de los nodos iniciales 0-n
    for (auto it = caminito.primera(); it != caminito.fin();
         it = caminito.siguiente(it)) {
        if (caminito.elemento(it) > 2 * n - 1) caminito.elemento(it) -= 2 * n;
        if (caminito.elemento(it) > n - 1) caminito.elemento(it) -= n;
    }

    return {vMin, caminito};
}

/**
 * @todo PROBLEMA 11
 * @body Subprograma que calcule los costes mínimos de viajar entre cualesquiera
 * dos ciudades de entre 3 islas
 */
template <typename tCoste>
matriz<tCoste> archipielagoHuries(const vector<GrafoP<tCoste>>& islas,
                                  const vector<arista<tCoste>>& puentes) {
    GrafoP<tCoste> bG{makeBGdifN(islas, puentes)};
    matriz<vertice<tCoste>> P;
    matriz<tCoste> F{Floyd(bG, P)};

    return F;
}

/**
 * @todo PROBLEMA 12
 * @body
 */

/**
 * @todo PROBLEMA 13
 * @body
 */