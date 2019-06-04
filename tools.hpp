#include <iostream>
#include "materialesGrafos/grafoPMC.h"
#include "materialesGrafos/listaenla.h"

using std::cout;

template <typename tCoste>
using vertice = typename GrafoP<tCoste>::vertice;

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
GrafoP<tCoste> makeBigGraph(const vector<GrafoP<tCoste>>& vGrafos,
                            const vector<vector<tCoste>>& vCostes) {
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
                            bigGraph[iv][jw] = vCostes[i][jw % n];
                    }
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