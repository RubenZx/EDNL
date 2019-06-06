#include <algorithm>
#include <iostream>
#include <iterator>
#include <tuple>
#include "materialesGrafos/grafoPMC.h"
#include "materialesGrafos/listaenla.h"

using std::cout;
using std::tuple;

template <typename tCoste>
using vertice = typename GrafoP<tCoste>::vertice;

template <typename tCoste>
using arista = typename GrafoP<tCoste>::arista;

template <typename T>
tuple<size_t, T> minIndex(const vector<T>& v) {
    auto ind = min_element(v.begin(), v.end());
    return {distance(v.begin(), ind), *ind};
}

template <typename T>
tuple<size_t, T> maxIndex(const vector<T>& v) {
    auto ind = max_element(v.begin(), v.end());
    return {distance(v.begin(), ind), *ind};
}

// Función para imprimir una Lista dada, utilizada para ver el camino de ir de
// un nodo a otro en un grafo
template <typename T>
void imprimirLista(const Lista<T>& lista) {
    for (auto it = lista.primera(); it != lista.fin(); it = lista.siguiente(it))
        if (it == lista.anterior(lista.fin()))
            cout << lista.elemento(it);
        else
            cout << lista.elemento(it) << "-";
}

// Función para crear el grafo grande con los distintos costes de los
// transbordos entre cada nodo del grafo
template <typename tCoste>
GrafoP<tCoste> makeBigGraph(const vector<GrafoP<tCoste>>& vGraphs,
                            const vector<vector<tCoste>>& vCostes) {
    const size_t nGraphs{vGraphs.size()}, dimGraph{vGraphs[0].numVert()};
    GrafoP<tCoste> bigGraph(nGraphs * dimGraph);
    vertice<tCoste> v, w, bgv, bgw;

    for (size_t i = 0, g = 0, d = 0; i < nGraphs;
         ++i, g += (nGraphs - i), d = 0) {
        // Rellenar costes de las aristas
        for (v = 0; v < dimGraph; ++v) {
            bgv = v + dimGraph * i;
            for (w = v; w < dimGraph; ++w) {
                bgw = w + dimGraph * i;
                bigGraph[bgv][bgw] = bigGraph[bgw][bgv] = vGraphs[i][v][w];
                // Rellenar costes de los transbordos
                if (v == w && i < nGraphs - 1) {
                    for (size_t j = dimGraph * (i + 1) + w, k = g;
                         j < nGraphs * dimGraph; j += dimGraph, ++k) {
                        bigGraph[bgv][j] = bigGraph[j][bgv] = vCostes[k][d];
                    }
                    ++d;
                }
            }
        }
    }
    return bigGraph;
}

// Sobrecarga de la función anterior para cuando los costes de ir y volver en
// cada transbordo sean distintos
template <typename tCoste>
GrafoP<tCoste> makeBigGraph(const vector<GrafoP<tCoste>>& vGraphs,
                            const vector<vector<tCoste>>& vCostesSup,
                            const vector<vector<tCoste>>& vCostesInf) {
    const size_t nGraphs{vGraphs.size()}, dimGraph{vGraphs[0].numVert()};
    GrafoP<tCoste> bigGraph(nGraphs * dimGraph);
    vertice<tCoste> v, w, bgv, bgw;

    for (size_t i = 0, g = 0, d = 0; i < nGraphs;
         ++i, g += (nGraphs - i), d = 0) {
        // Rellenar costes de las aristas
        for (v = 0; v < dimGraph; ++v) {
            bgv = v + dimGraph * i;
            for (w = v; w < dimGraph; ++w) {
                bgw = w + dimGraph * i;
                bigGraph[bgv][bgw] = bigGraph[bgw][bgv] = vGraphs[i][v][w];
                // Rellenar costes de los transbordos
                if (v == w && i < nGraphs - 1) {
                    for (size_t j = dimGraph * (i + 1) + w, k = g;
                         j < nGraphs * dimGraph; j += dimGraph, ++k) {
                        bigGraph[bgv][j] = vCostesSup[k][d];
                        bigGraph[j][bgv] = vCostesInf[k][d];
                    }
                    ++d;
                }
            }
        }
    }
    return bigGraph;
}

// Sobrecarga de la función anterior, para un solo valor
template <typename tCoste>
GrafoP<tCoste> makeBigGraph(const vector<GrafoP<tCoste>>& vGrafos, tCoste cst) {
    size_t n{vGrafos[0].numVert()}, m{vGrafos.size()};
    GrafoP<tCoste> bigGraph(n * m);

    vertice<tCoste> v, w;
    for (size_t i = 0; i < m; ++i) {
        for (v = 0; v < n; ++v) {
            for (w = 0; w < n; ++w) {
                bigGraph[v + n * i][w + n * i] = vGrafos[i][v][w];
                if (v == w) {
                    for (size_t j = 0; j < m; ++j) {
                        if (size_t iv = v + n * i, jw = w + n * j; iv != jw)
                            bigGraph[iv][jw] = cst;
                    }
                }
            }
        }
    }

    return bigGraph;
}

// Función para crear el grafo grande a partir de grafos de distinto tamaños
template <typename tCoste>
GrafoP<tCoste> makeBGdifN(const vector<GrafoP<tCoste>>& vGrafos,
                          const vector<arista<tCoste>>& vCost) {
    size_t nGraphs{vGrafos.size()}, tot{0};
    vector<size_t> vTam(nGraphs, 0);

    // Guardamos el tamaño de cada grafo en un vector y el total en la var. tot
    for (size_t ind = 0; ind < nGraphs; ++ind) {
        vTam[ind] = vGrafos[ind].numVert();
        tot += vTam[ind];
    }

    GrafoP<tCoste> bG(tot);
    vector<arista<tCoste>> aristasI;
    vertice<tCoste> v, w, x, y;
    size_t sum = 0, tamAct, indbG;
    // Bucle hasta el número de grafos que tengamos
    for (size_t i = 0; i < nGraphs; ++i) {
        tamAct = vTam[i];      // Tomamos la dim. del grafo i-ésimo
        sum += tamAct;         // Acumulamos las dimensiones
        indbG = sum - tamAct;  // Contador para indexar el grafo grande
        for (v = 0, x = indbG; v < tamAct; ++v, ++x) {
            for (w = 0, y = indbG; w < tamAct; ++w, ++y) {
                bG[x][y] = vGrafos[i][v][w];
            }
        }
    }

    // for (size_t j = 0; j < vCost.size(); ++j) {
    //     bG[vCost[j].orig][vCost[j].dest] = bG[vCost[j].dest][vCost[j].orig] =
    //         vCost[j].coste;
    // }

    return bG;
}