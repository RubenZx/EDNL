#include "p8.hpp"

int main() {
    vector<coordCart> ciudT{{2, 4}, {-1, 5}, {2, 3}, {-8, 3}, {0, 1}, {4, -4}};
    Grafo gTombuctu("files/p8-1-tombuctu.txt");
    auto [vIslas, costeMin] = Tombuctu(ciudT, gTombuctu);
    cout << "\nTOMBUCTU:\n\t-> Islas: ";

    return 0;
}