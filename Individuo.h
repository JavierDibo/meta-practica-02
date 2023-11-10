#ifndef META_PRACTICA_02_INDIVIDUO_H
#define META_PRACTICA_02_INDIVIDUO_H

#include <utility>
#include <vector>
#include <fstream>
#include <unordered_set>

#include "LectorCiudades.h"
#include "globals.h"

class Individuo {

private:
    std::vector<int> camino;
    double coste{};
    LectorCiudades &lector_datos;

public:

    Individuo(std::vector<int> camino, LectorCiudades &lector_datos);

    Individuo(const Individuo &otro) : camino(otro.camino), coste(otro.coste), lector_datos(otro.lector_datos) {}

    virtual ~Individuo();

    double get_coste() const;

    int get_ciudad(int index) const;

    const std::vector<int> &get_camino() const;

    void mutar();

    bool operator<(const Individuo &otro) const;

    bool operator>(const Individuo &otro) const;

    bool operator<=(const Individuo &otro) const;

    bool operator>=(const Individuo &otro) const;

    bool operator==(const Individuo &otro) const;

    bool operator!=(const Individuo &otro) const;

    Individuo &operator=(const Individuo &otro);

    void evaluar();

    void set_ciudad(int index, int ciudad);

    void intercambiar(int posicion1, int posicion2);

};

#endif //META_PRACTICA_02_INDIVIDUO_H
