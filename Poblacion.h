#ifndef META_PRACTICA_02_POBLACION_H
#define META_PRACTICA_02_POBLACION_H

#include <vector>
#include <unordered_set>
#include <algorithm>

#include "Individuo.h"
#include "LectorCiudades.h"
#include "globals.h"
#include "GeneradorAleatorio.h"

class Poblacion {
private:
    std::vector<Individuo> individuos;
    int num_invididuos;
    int num_ciudades;
    int generacion = 0;
    int num_evuaciones;
    LectorCiudades &lector_datos;

public:

    explicit Poblacion(LectorCiudades &lector_datos);

    virtual ~Poblacion();

    void evolucionar();

    const std::vector<Individuo> &get_individuos() const;

private:

    void crear_poblacion();

    Individuo seleccionar();

    bool condicion_parada() const;

    Individuo cruzar(const Individuo &padre1, const Individuo &padre2);

    Individuo cruceOX2(const Individuo &padre_a, const Individuo &padre_b);

    Individuo crucePMX(const Individuo &padre_a, const Individuo &padre_b);

};

#endif //META_PRACTICA_02_POBLACION_H
