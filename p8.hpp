#include <cmath>
#include <iostream>
#include <numeric>
#include "GraphFiles/alg_grafoMA.h"
#include "GraphFiles/alg_grafoPMC.h"
#include "GraphFiles/particion.hpp"
// #include "GraphFiles/alg_grafo_E-S.h"

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
vector<arista<double>> Tombuctu2(const vector<coordCart>& ciudades,
                                 const Grafo& carreteras) {
    auto [islas, floyd] = Tombuctu(ciudades, carreteras);

    size_t nIslas = islas.size(), ant = 0;
    double costeAct, costesMin{GrafoP<double>::INFINITO};
    vector<vertice<double>> repre(nIslas);
    for (size_t i = 0; i < nIslas; ++i) {  // Bucle para recorrer cada isla
        for (size_t j = ant; j < islas[i].size(); ++j) {
            // Hacemos la suma de cada fila y vemos si su coste es menor, para
            // asi elegir el representante de cada isla
            costeAct = accumulate(
                floyd[j].begin(), floyd[j].end(), 0, [](double a, double b) {
                    return ((b != GrafoP<double>::INFINITO) ? a + b : a);
                });
            if (costeAct < costesMin) {
                costesMin = costeAct;
                repre[i] = j;
            }
        }
        costesMin = GrafoP<double>::INFINITO;
        ant += islas[i].size();
    }

    // A continuación pasamos a crear los aeropuertos entre las islas
    vector<arista<double>> aeropuerto;

    return aeropuerto;
}