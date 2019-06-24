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
    bool operator==(const coordCart& c) const { return (x == c.x && y == c.y); }
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
// minimizando el coste entre los aeropuertos de las islas
struct ciudad {
    double x, y;
    ciudad() = default;
    double distEuclid(const ciudad& c) const {
        return sqrt(pow(x - c.x, 2) + pow(y - c.y, 2));
    }
};

vector<typename GrafoP<double>::arista> Tombuctu2(
    const vector<ciudad>& ciudades, const Grafo& conexiones) {
    size_t nIslas = nCiud = ciudades.size();
    GrafoP<double> G(nCiud);
    Particion P(nCiud);

    int r1, r2;
    typename GrafoP<double>::vertice v, w;
    for (v = 0; v < nCiud; ++v) {
        r1 = P.encontrar(v);
        for (w = v; w < nCiud; ++w) {
            r2 = P.encontrar(w);
            if (conexiones[v][w] && r1 != r2) P.unir(r1, r2);
        }
    }

    // Cada key será la isla i, y el vector contendrá las ciudades de la isla i
    map<int, vector<typename GrafoP<double>::vertice>> islas;
    for (size_t i = 0; i < nCiud; ++i) {
        islas[P.encontrar(i)].push_back(i);
    }

    vector<typename GrafoP<double>::arista> aerolineas;
    vector<typename GrafoP<double>::vertice> islaAct, isla2;
    double costeAct, costeMin{GrafoP<double>::INFINITO};
    typename GrafoP<double>::arista aerolineaAct;
    for (auto isla_i = islas.begin(); isla_i != islas.end(); ++isla_i) {
        islaAct = isla_i->second;
        for (auto islaSig = next(isla_i); islaSig != islas.end(); ++islaSig) {
            isla2 = islaSig->second;
            for (size_t i = 0; i < islaAct.size(); ++i) {
                for (size_t j = 0; j < isla2.size(); ++j) {
                    costeAct =
                        ciudades[islaAct[i]].distEuclid(ciudades[isla2[j]]);
                    if (costeAct < costeMin) {
                        aerolineaAct.orig = islaAct[i];
                        aerolineaAct.dest = isla2[j];
                        aerolineaAct.coste = costeAct;
                    }
                }
            }
            aerolineas.push_back(aerolineaAct);
            costeMin = GrafoP<double>::INFINITO;
        }
    }

    return aerolineas;
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

// Problema 7:
// Implementar un subprograma que calcule el coste mínimo de viajar entre dos
// ciudades, orig y dest, despueés de reconstruir el archipiélago, sabiendo que:
// a) El coste de construir cualquier carretera o puente es su dist. eucl.
// b) Cualquier puente siempre será más caro que cualquier carretera.
double reconstruirZuelandia(const vector<coordCart>& Fobos,
                            const vector<coordCart>& Deimos,
                            const vector<coordCart>& cFobos,
                            const vector<coordCart>& cDeimos,
                            vertice<double> orig, vertice<double> dest) {
    // Construimos un grafo uniendo ambas islas
    size_t tamF{Fobos.size()}, tamD{Deimos.size()};
    GrafoP<double> GFobos{tamF}, GDeimos{tamD}, bG{tamF + tamD};

    // A continuación pasamos a calcular la distancia entre todas y cada una de
    // las ciudades dadas para cada isla
    vertice<double> v, w;
    for (v = 0; v < tamF + tamD; ++v) {
        for (w = v + 1; w < tamF + tamD; ++w) {
            if (v < tamF && w < tamF)
                GFobos[v][w] = GFobos[w][v] = Fobos[v].distEuclid(Fobos[w]);
            else
                GDeimos[v - tamF][w - tamF] = GDeimos[w - tamF][v - tamF] =
                    Deimos[v - tamF].distEuclid(Deimos[w - tamF]);
        }
    }

    // Calculamos Kruskall para quedarnos con las carreteras entre las ciudades
    // de las islas que minimicen la distancia entre todas
    GFobos = Kruskall(GFobos);
    GDeimos = Kruskall(GDeimos);

    // Ahora pasamos a conectar las islas usando las costeras, de tal manera que
    // conectaremos todas las costeras de una con todas las costeras de otra
    size_t n = bG.numVert();
    for (v = 0; v < n; ++v) {
        for (w = v; w < n; ++w) {
            if (v < tamF && w < tamF)  // Grafo superior
                bG[v][w] = bG[w][v] = GFobos[v][w];
            else if (v >= tamF && w >= tamF)  // Grafo inferior
                bG[v][w] = bG[w][v] = GDeimos[v - tamF][w - tamF];
            else if (v < tamF && w >= tamF &&
                     find(cFobos.begin(), cFobos.end(), Fobos[v]) !=
                         cFobos.end() &&
                     find(cDeimos.begin(), cDeimos.end(), Deimos[w - tamF]) !=
                         cDeimos.end())  // Conexión entre costeras
                bG[v][w] = bG[w][v] = Fobos[v].distEuclid(Deimos[w - tamF]);
        }
    }

    // Calculamos Floyd para ver que cual es la mejor costera, para conectar
    // ambas islas
    matriz<vertice<double>> mP;
    matriz<double> floyd{Floyd(bG, mP)};
}
