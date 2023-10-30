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
    int num_elites;
    int num_evuaciones;
    LectorCiudades &lector_datos;

public:

    Poblacion(int num_individuos, LectorCiudades &lector_datos);

    virtual ~Poblacion();

    void evolucion_generacional();

private:

    void crear_poblacion();

    Individuo seleccionar();

    bool condicion_parada() const;

    Individuo cruzar(const Individuo &padre1, const Individuo &padre2);

    Individuo cruceOX2(const Individuo &padre1, const Individuo &padre2);

    Individuo crucePMX(const Individuo &padre1, const Individuo &padre2);

    void mutar(Individuo &individuo) const;
};

#endif //META_PRACTICA_02_POBLACION_H
