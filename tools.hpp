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
        for (v = 0; v < n; ++v) {  // n = dim. del grafo que recibimos
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

// Hay que arreglar la función anterior, no funciona correctamente
// template <typename tCoste>
// GrafoP<tCoste> makeBigGraph(const vector<GrafoP<tCoste>>& vGrafos,
//                             const vector<vector<tCoste>>& vCostes) {
//     size_t dimGraph{vGrafos[0].numVert()}, numGraphs{vGrafos.size()};
//     size_t dimBigGraph{dimGraph*numGraphs};
//     GrafoP<tCoste> bigGraph(dimBigGraph);

//     vertice<tCoste> v, w;
//     for (size_t i = 0; i < numGraphs; ++i) {
//         for (size_t j = 0; j < numGraphs; ++j) {
//             if(i == j){     // Rellenamos los costes de los grafos
//                 for (v = 0; v < dimGraph; ++v) {
//                     for (w = 0; w < dimGraph; ++w) {
//                         bigGraph[v + dimGraph * i][w + dimGraph * i] =
//                         vGrafos[i][v][w];
//                     }
//                 }

//             }else{
//                 if(i < j){  // Rellenamos el coste del transbordo en la diag.
//                 sup.
//                     for (v = i; v < dimBigGraph; ++v) {
//                         for (w = j; v < dimBigGraph; ++v) {
//                             bigGraph[i][j] = vCostes[i][v];
//                         }
//                     }
//                 } // else{      // Rellenamos el coste del transbordo de la
//                 diag. inf.

//                 // }
//             }

//         }
//     }

//     return bigGraph;
// }

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