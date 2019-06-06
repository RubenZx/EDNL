#include <iostream>
#include "materialesGrafos/grafoPMC.h"
#include "materialesGrafos/listaenla.h"

using std::cout;

template <typename tCoste>
using vertice = typename GrafoP<tCoste>::vertice;

// Función para imprimir una Lista
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