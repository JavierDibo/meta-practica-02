#ifndef META_PRACTICA_02_INDIVIDUO_H
#define META_PRACTICA_02_INDIVIDUO_H

#include <utility>
#include <vector>
#include "LectorCiudades.h"

class Individuo {
private:
    std::vector<int> camino;
    double coste{};
    LectorCiudades &lector_datos;

public:
    Individuo(std::vector<int> camino, LectorCiudades &lector_datos) : camino(std::move(camino)),
                                                                       lector_datos(lector_datos) {
        calcular_coste_camino();
    }

    Individuo(const Individuo &otro) : camino(otro.camino), coste(otro.coste), lector_datos(otro.lector_datos) {
    }

    void calcular_coste_camino() {
        coste = 0.0;
        for (size_t i = 0; i < camino.size() - 1; ++i) {
            coste += lector_datos.calcular_distancia(camino[i], camino[i + 1]);
        }
        coste += lector_datos.calcular_distancia(camino.back(), camino.front()); // Cerrar el ciclo
    }

    double get_coste() const {
        return coste;
    }

    int get_ciudad(int index) const {
        return camino[index];
    }

    void set_ciudad(int index, int ciudad) {
        camino[index] = ciudad;
    }


    bool operator<(const Individuo &otro) const {
        return coste < otro.get_coste();
    }

    bool operator>(const Individuo &otro) const {
        return coste > otro.get_coste();
    }

    bool operator<=(const Individuo &otro) const {
        return coste <= otro.get_coste();
    }

    bool operator>=(const Individuo &otro) const {
        return coste >= otro.get_coste();
    }

    bool operator==(const Individuo &otro) const {
        return coste == otro.get_coste();
    }

    bool operator!=(const Individuo &otro) const {
        return coste != otro.get_coste();
    }

    Individuo &operator=(const Individuo &otro) {
        if (this != &otro) {
            camino = otro.camino;
            coste = otro.coste;
        }
        return *this;
    }

    void intercambiar(int posicion1, int posicion2) {
        if (posicion1 >= 0 && posicion1 < camino.size() && posicion2 >= 0 && posicion2 < camino.size()) {
            std::swap(camino[posicion1], camino[posicion2]);
            calcular_coste_camino();
        } else {
            throw std::out_of_range("Individuo::intercambiar::fuera de rango");
        }
    }

    // Aquí irían los otros métodos como mutar(), get_ciudad(), set_ciudad(), etc.
};

#endif //META_PRACTICA_02_INDIVIDUO_H
