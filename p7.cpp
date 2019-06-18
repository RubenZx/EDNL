#include "p7.hpp"

int main() {
    unsigned inf = GrafoP<unsigned>::INFINITO;
    //==========================================================================
    // PROBLEMA 1
    //==========================================================================
    GrafoP<unsigned> G("files/p7-1-grafo.txt");
    GrafoP<unsigned>::arista viaje = otraVezUnGrafoSA(G);

    cout << "\n -> 1_OTRAVEZUNGRAFOSA PROPONE COMO VIAJE:";
    cout << "\n\t- ORIGEN.... " << viaje.orig;
    cout << "\n\t- DESTINO... " << viaje.dest;
    cout << "\n\t- PRECIO$... " << viaje.coste;
    //==========================================================================
    // PROBLEMA 2
    //==========================================================================
    int N = 3;
    Lista<GrafoP<unsigned>::arista> paredes;
    paredes.insertar(GrafoP<unsigned>::arista(0, 1, inf), paredes.fin());
    paredes.insertar(GrafoP<unsigned>::arista(0, 1, inf), paredes.fin());
    GrafoP<unsigned>::vertice entrada{3}, salida{8};
    pair<GrafoP<unsigned>::tCamino, unsigned> lab;
    lab = (laberinto<unsigned>(N, paredes, entrada, salida));

    cout << "\n\n -> 2_SALIDA DEL LABERINTO...............   ";
    imprimirLista(lab.first);
    //==========================================================================
    // PROBLEMA 3:
    //==========================================================================
    GrafoP<unsigned> Dist("files/p7-3-dist.txt");
    pair<vector<unsigned>, unsigned> p;
    GrafoP<unsigned>::vertice centro{1};
    unsigned cantidad{80};
    vector<unsigned> capacidad{20, 0, 10, 50, 40, 10};
    vector<double> subv{30, 0, 25, 15, 10, 20};
    p = distribucion(Dist, centro, cantidad, capacidad, subv);

    cout << "\n\n -> 3_DISTRIBUCION: ";
    cout << "\t\t*** COSTE *** " << p.second << endl;
    cout << "\t- CIUDADES-CANTIDAD:" << endl;
    for (int i = 0; i < p.first.size(); ++i)
        cout << "\t- Ciudad: " << i << " Cantidad: " << p.first[i] << endl;
    //==========================================================================
    // PROBLEMA 4:
    //==========================================================================
    GrafoP<unsigned> Zuel("files/p7-4-Zuelandia.txt");
    vector<unsigned> v = {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    unsigned km = cementosZuelandia(Zuel, v, 0);

    cout << "\n -> 4_CEMENTOS DE ZUELANDIA.............   " << km;
    //==========================================================================
    // PROBLEMA 5:
    //==========================================================================
    GrafoP<short> A("files/p7-5-a.txt");  // Avión
    GrafoP<short> C("files/p7-5-c.txt");  // Carreteras
    GrafoP<short> T("files/p7-5-t.txt");  // Tren
    short presupuesto{50};
    vector<vector<size_t>> got(3);
    got[0] = viajeroAlergico(C, T, A, presupuesto, carretera, 0);
    got[1] = viajeroAlergico(C, T, A, presupuesto, tren, 0);
    got[2] = viajeroAlergico(C, T, A, presupuesto, avion, 0);

    cout << "\n\n -> 5_VIAJERO ALERGICO:";
    cout << "\n\t- VIAJES SIN CARRETERA............" << got[0];
    cout << "\n\t- VIAJES SIN TREN................." << got[1];
    cout << "\n\t- VIAJES SIN AVION................" << got[2];
    //==========================================================================
    // PROBLEMA 6:
    //==========================================================================
    GrafoP<int> Bus("files/p7-6...10-bus.txt");
    GrafoP<int> Tren("files/p7-6...10-tren.txt");
    matriz<int> viajeSinTaxi = transportesSinTaxi(Bus, Tren, 4);

    cout << "\n\n -> 6_TRANSPORTE SIN TAXI...............   \n" << viajeSinTaxi;
    //==========================================================================
    // PROBLEMA 7:
    //==========================================================================
    auto [cTaxi2, pTaxi2] = transportesSinTaxi2(Tren, Bus, 4, 3, 0, 7);
    auto [cTaxi2_1, pTaxi2_1] = transportesSinTaxi2(Tren, Bus, 4, 3, 1, 6);

    cout << "\n -> 7_TRANSPORTE SIN TAXI2..............";
    cout << "\n\t __TRANSPORTE 1__  ";
    cout << "\n\t- COSTE:  " << cTaxi2;
    cout << "\n\t- CAMINO: ";
    imprimirLista(pTaxi2);
    cout << "\n\n\t __TRANSPORTE 2__  ";
    cout << "\n\t- COSTE:  " << cTaxi2_1;
    cout << "\n\t- CAMINO: ";
    imprimirLista(pTaxi2_1);
    //==========================================================================
    // PROBLEMA 8:
    //==========================================================================
    int costeViaje = unSoloTransbordo(Tren, Bus, 0, 7);
    int costeViaje2 = unSoloTransbordo(Tren, Bus, 4, 3);

    cout << "\n\n -> 8_UN SOLO TRANSBORDO................";
    cout << "\n\t- COSTE 0-7: " << costeViaje;
    cout << "\n\t- COSTE 4-3: " << costeViaje2;
    //==========================================================================
    // PROBLEMA 9:
    //==========================================================================
    auto [costeTaxi, caminoTaxi] = transporteConTaxi(Tren, Bus, 0, 1);
    cout << "\n\n -> 9_TRANSPORTE CON TAXI...............";
    cout << "\n\t- COSTE  0-1: " << costeTaxi;
    cout << "\n\t- CAMINO 0-1: ";
    imprimirLista(caminoTaxi);
    //==========================================================================
    // PROBLEMA 10:
    //==========================================================================
    GrafoP<int> Avion("files/p7-10-avion.txt");
    auto [costeT2, caminoT2] = transporteConTaxi2(Tren, Bus, Avion, 4, 3, 2, 3);
    cout << "\n\n -> 10_TRANSPORTE CON TAXI2.............";
    cout << "\n\t- COSTE  4-3: " << costeT2;
    cout << "\n\t- CAMINO 4-3: ";
    imprimirLista(caminoT2);
    //==========================================================================
    // PROBLEMA 11:
    //==========================================================================
    GrafoP<int> io("files/p7-11-io.txt");
    GrafoP<int> eu("files/p7-11-europa.txt");
    GrafoP<int> eik("files/p7-11-eikos.txt");
    vector<arista<int>> vAristas{arista<int>{3, 0, 0}, arista<int>{6, 7, 0},
                                 arista<int>{2, 8, 0}};
    matriz<int> M_Islas =
        archipielagoHuries(vector<GrafoP<int>>{eik, io, eu}, vAristas);
    cout << "\n\n -> 11_ARCHIPIELAGO HURIES.............\n" << M_Islas;

    //==========================================================================
    // PROBLEMA 12:
    //==========================================================================
    arista<int> puente{
        archipielagoGrecoland(vector<GrafoP<int>>{eik, io},
                              vector<GrafoP<int>::vertice>{1, 2, 3, 4})};
    cout << "\n\n -> 12_ARCHIPIELAGO GRECOLAND..........\n";
    cout << "\t- PUENTE: " << puente.orig << " - " << puente.dest;

    //==========================================================================
    // PROBLEMA 13:
    //==========================================================================
    vector<arista<int>> puentes{
        HuriesPuentes(vector<GrafoP<int>>{eu, eik, io},
                      vector<GrafoP<int>::vertice>{1, 2, 3, 5, 7})};
    cout << "\n\n -> 13_ARCHIPIELAGO HURIES PUENTES....\n";
    for (size_t i = 0; i < puentes.size(); ++i) {
        cout << "\t- PUENTE " << i << ": " << puentes[i].orig << " - "
             << puentes[i].dest << endl;
    }

    return 0;
}