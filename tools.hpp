#include <iostream>
#include "materialesGrafos/listaenla.h"

using std::cout;

template <typename T>
void imprimirLista(const Lista<T>& lista) {
    for (auto it = lista.primera(); it != lista.fin(); it = lista.siguiente(it))
        if (it == lista.anterior(lista.fin()))
            cout << lista.elemento(it);
        else
            cout << lista.elemento(it) << "-";
}