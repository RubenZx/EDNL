#include <cmath>
#include <iostream>
#include "GraphFiles/alg_grafoMA.h"
#include "GraphFiles/alg_grafoPMC.h"
#include "GraphFiles/alg_grafo_E-S.h"
#include "GraphFiles/particion.h"

template <typename tCoste>
using arista = typename GrafoP<tCoste>::arista;
template <typename tCoste>
using vertice = typename GrafoP<tCoste>::vertice;
template <typename tCoste>
using tCamino = typename GrafoP<tCoste>::tCamino;

using namespace std;

struct coordCart {
    double x, y;
    double distEuclid(const coordCart& c) const {
        return sqrt(pow(x - c.x, 2) + pow(y - c.y, 2));
    }
};

// Problema 1:
// Encontrar qué ciudades de Tombuctú pertenecen a cada isla y cual es el coste
// mínimo de viajar entre cualesquiera dos ciudades de una misma isla
tuple<vector<vector<vertice<size_t>>>, matriz<double>> Tombuctu(
    const vector<coordCart>& ciudades, const Grafo& carreteras) {
    int nCiud = ciudades.size();
    Particion P{nCiud};
    GrafoP<double> G(nCiud);

    int repre1 = 0, repre2 = 0;
    for (size_t v = 0; v < nCiud; ++v) {
        repre1 = P.encontrar(v);
        for (size_t w = v; w < nCiud; ++w) {
            if (carreteras[v][w]) {
                repre2 = P.encontrar(w);
                P.unir(repre1, repre2);
                G[v][w] = G[w][v] = ciudades[v].distEuclid(ciudades[w]);
            }
        }
    }

    matriz<vertice<double>> mP;
    matriz<double> F{Floyd(G, mP)};

    repre1 = P.encontrar(0);
    vector<vector<vertice<size_t>>> vIslas(1, vector<size_t>(1, 0));
    size_t numIslas = 0;
    for (size_t i = 1; i < nCiud; ++i) {
        if (repre1 == P.encontrar(i))
            vIslas[numIslas].push_back(i);
        else {
            ++numIslas;
            repre1 = P.encontrar(i);
            vIslas.push_back(vector<size_t>(1, i));
        }
    }

    return {vIslas, F};
}

// Problema 2:
