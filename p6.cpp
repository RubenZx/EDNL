#include "p6.hpp"

int main()
{
    typedef GrafoP<unsigned>::vertice vertice;
    typedef GrafoP<unsigned>::arista arista;

    GrafoP<unsigned> G("Grafo1.txt");
    string acicl;
    matriz<unsigned> Z(G.numVert());
    vector<unsigned> ciudades{6};
    vector<arista> carreteras{GrafoP<unsigned>::arista(2,5,10)};
    vertice capital{2};
    
    if(aciclico(G)) acicl = "Si";
    else acicl = "No";
    
    vector<GrafoP<unsigned>::vertice> P{};

    cout << "\n -> DIAMETRO.... " << diametro(G);
    cout << "\n -> ACICLICO?... " << acicl;
    cout << "\n -> Zuelandia antes de la rebelion.....\n" << G;
    
    Z = Zuelandia(G, ciudades, carreteras, capital);
    cout << "\n -> Zuelandia despues de la rebelion...\n" << Z << endl;

    return 0;
}