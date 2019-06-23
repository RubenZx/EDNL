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
    matriz<vertice<double>> mP;
    return {vIslas, Floyd(G, mP)};
}

// Problema 2:
// Encontrar como conectar las ciudades de las islas mediante aeropuertos
// minimizando el coste entre todas las ciudades
vector<arista<double>> Tombuctu2(const vector<coordCart>& ciudades,
                                 const Grafo& carreteras) {
    int nCiud = ciudades.size();
    Particion P{nCiud};
    GrafoP<double> G(nCiud);

    // Ponemos los costes de ir de todas las ciudades a todas las demas
    int repre1 = 0, repre2 = 0;
    for (size_t v = 0; v < nCiud; ++v) {
        repre1 = P.encontrar(v);
        for (size_t w = v; w < nCiud; ++w) {
            if (carreteras[v][w]) {
                repre2 = P.encontrar(w);
                P.unir(repre1, repre2);
            }
            G[v][w] = G[w][v] = ciudades[v].distEuclid(ciudades[w]);
        }
    }

    // Guardamos las islas en un vector, de vector en los cuales cada posición
    // corresponde a las ciudades de cada isla
    repre1 = P.encontrar(0);
    vector<vector<vertice<size_t>>> islas(1, vector<size_t>(1, 0));
    size_t nIslas = 0;
    for (size_t i = 1; i < nCiud; ++i) {
        if (repre1 == P.encontrar(i))
            islas[nIslas].push_back(i);
        else {
            ++nIslas;
            repre1 = P.encontrar(i);
            islas.push_back(vector<size_t>(1, i));
        }
    }

    // Calculamos Floyd para buscar lamejor ciudad de cada isla para poner ahhí
    // el aeropuerto
    matriz<vertice<double>> mP;
    matriz<double> floyd{Floyd(G, mP)};

    double costeAct, costesMin{GrafoP<double>::INFINITO};
    size_t ant = 0;
    // Vector de vectores de representantes por cada islas
    vector<vertice<double>> repre(nIslas);
    for (size_t i = 0; i < nIslas; ++i) {  // Bucle para recorrer cada isla
        size_t nCiud = islas[i].size();
        for (size_t j = ant; j < ant + nCiud; ++j) {
            // Hacemos la suma de cada fila y vemos si su coste es menor,
            // para asi elegir el representante de cada isla
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

    // Ahora rellenamos las aerolineas
    vector<arista<double>> aerolinea;

    for (size_t i = 0; i < nIslas; ++i) {
        for (size_t j = i + 1; j < nIslas; ++j) {
            size_t a = repre[i], b = repre[j];
            aerolinea.push_back(
                arista<double>{a, b, ciudades[a].distEuclid(ciudades[b])});
        }
    }

    return aerolinea;
}

// Problema 4:
// Dado un grafo de distancias entre ciudades, calcular qué canales y de qué
// longitud deben construirse partiendo del grafo
vector<arista<double>> emasajerSA(const GrafoP<double>& Jerte) {
    GrafoP<double> gJerte{KruskallMax(Jerte)};
    vector<arista<double>> vJerte;
    size_t n = gJerte.numVert();
    for (size_t i = 0; i < n; ++i)
        for (size_t j = i + 1; j < n; ++j)
            if (gJerte[i][j] != GrafoP<double>::INFINITO)
                vJerte.push_back(arista<double>{i, j, gJerte[i][j]});

    return vJerte;
}

// Problema 5:
// Calcular la longitud mínima de fibra dentro de un pais
double Reteuni3(const GrafoP<double>& pais) {
    matriz<vertice<double>> mP;
    matriz<double> floyd{Floyd(pais, mP)};

    double min = GrafoP<double>::INFINITO, minAct;
    for (size_t i = 0; i < floyd.dimension(); ++i) {
        minAct = accumulate(
            floyd[i].begin(), floyd[i].end(), 0, [](double a, double b) {
                return ((b != GrafoP<double>::INFINITO) ? a + b : a);
            });
        if (minAct < min) min = minAct;
    }

    return min;
}

// Problema 6:
// Calcular qué canales y de qué longitud y caudal debe construirse para
// minimizar el coste total de la red de canales
template <typename T>
struct canal {
    T longitud, caudal, coste;
    canal() = default;
    // Método para calcular el coste de un canal concreto
    void costeCanal(T subv, T km) { coste = longitud * km - subv * caudal; }
    T costeCanal() const { return coste; }
};

template <typename tCoste>
vector<arista<canal<tCoste>>> emasajerSA(const GrafoP<tCoste>& distJerte,
                                         const GrafoP<tCoste>& caudalJerte,
                                         tCoste subvCaudal, tCoste costeKm) {
    size_t n = distJerte.numVert();
    GrafoP<canal> G(n);
    GrafoP<tCoste> grafoF(n);

    // Calculamos los costes de crear los canales entre todas las ciudades
    vertice<tCoste> v, w;
    for (v = 0; v < n; ++v) {
        for (w = 0; w < n; ++w) {
            if (G[v][w] != GrafoP<tCoste>::INFINITO) {
                G[v][w].longitud = Fdist[v][w];
                G[v][w].caudal = caudalJerte[v][w];
                G[v][w].costeCanal(subvCaudal, costeKm);
                grafoF[v][w] = G[v][w].costeCanal();
            }
        }
    }

    // Caculamos Floyd para minimizar las distancias
    matriz<vertice<tCoste>> mP;
    matriz<tCoste> Fdist{Floyd(grafoF, mP)};

    // Aqui tomamos el arbol generador de coste mínimo, que será aquella fila de
    // Floyd que minimice el coste
    vertice<tCoste> nodo;
    double min = GrafoP<double>::INFINITO, minAct;
    for (vertice<tCoste> i = 0; i < G.dimension(); ++i) {
        minAct =
            accumulate(G[i].begin(), G[i].end(), 0, [](double a, double b) {
                return ((b != GrafoP<double>::INFINITO) ? a + b : a);
            });
        if (minAct < min) {
            min = minAct;
            nodo = i;
        }
    }

    // Ahora pasamos a crear el vector de aristas<caudal>
    vector<arista<canal<tCoste>>> vCanales;
    for (vertice<tCoste> i = 0; i < n; ++i) {
        vCanales.push_back(arista<canal<tCoste>>{
            G[nodo][i].longitud, G[nodo][i].caudal, G[nodo][i].coste});
    }

    return vCanales;
}