#ifndef META_PRACTICA_02_POBLACION_H
#define META_PRACTICA_02_POBLACION_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "Individuo.h"
#include "LectorCiudades.h"
#include "globals.h"
#include "GeneradorAleatorio.h"
#include "Reloj.h"

class Poblacion {
private:
    std::vector<Individuo> individuos;
    int num_invididuos;
    int num_ciudades;
    int generacion = 0;
    int num_evuaciones;
    LectorCiudades &lector_datos;
    Reloj reloj;

public:

    explicit Poblacion(LectorCiudades &lector_datos);

    virtual ~Poblacion();

    void evolucionar();

    const std::vector<Individuo> &get_individuos() const;

private:

    void crear_poblacion();

    Individuo torneo_kbest();

    bool condicion_parada();

    Individuo cruzar(const Individuo &padre1, const Individuo &padre2);

    Individuo cruceOX2(const Individuo &padre_a, const Individuo &padre_b);

    Individuo crucePMX(const Individuo &padre_a, const Individuo &padre_b);

    Individuo * torneo_kworst();

    Individuo cruceMOC(const Individuo &padre_a, const Individuo &padre_b);

    std::vector<int> camino_greedy();

    std::vector<int> get_ciudades_cercanas(int ciudad);
};

#endif //META_PRACTICA_02_POBLACION_H
