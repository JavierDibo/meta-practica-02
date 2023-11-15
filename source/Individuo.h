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
    std::vector<int> camino{};
    double coste = INFINITO_POSITIVO;
    LectorCiudades &lector_ciudades;

public:

    Individuo(std::vector<int> camino, LectorCiudades &lector_datos);

    Individuo(const Individuo &otro);

    virtual ~Individuo();

    [[nodiscard]] double get_coste() const;

    [[nodiscard]] int get_ciudad(int index) const;

    [[nodiscard]] const std::vector<int> &get_camino() const;

    void mutar();

    bool operator<(const Individuo &otro) const;

    bool operator>(const Individuo &otro) const;

    bool operator<=(const Individuo &otro) const;

    bool operator>=(const Individuo &otro) const;

    bool operator==(const Individuo &otro) const;

    bool operator!=(const Individuo &otro) const;

    Individuo &operator=(const Individuo &otro);

    [[maybe_unused]] void set_ciudad(int index, int ciudad);

    void intercambiar(int posicion1, int posicion2);

    void evaluar();
};

#endif //META_PRACTICA_02_INDIVIDUO_H
