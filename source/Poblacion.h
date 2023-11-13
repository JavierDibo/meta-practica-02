#ifndef META_PRACTICA_02_POBLACION_H
#define META_PRACTICA_02_POBLACION_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <mutex>

#include "Individuo.h"
#include "LectorCiudades.h"
#include "globals.h"
#include "GeneradorAleatorio.h"
#include "Reloj.h"

class Poblacion {
private:
    std::vector<Individuo> individuos;
    int num_invididuos = NUMERO_INDIVIDUOS;
    int num_ciudades;
    int num_generaciones = 0;
    [[maybe_unused]] int num_evaluaciones = 0;
    LectorCiudades &lector_datos;
    Reloj reloj;

public:

    explicit Poblacion(LectorCiudades &lector_datos);

    virtual ~Poblacion();

    void evolucionar();

    [[nodiscard]] const std::vector<Individuo> &get_individuos() const;

private:

    void crear_poblacion();

    bool condicion_parada();

    std::vector<int> cruzar(const Individuo &padre1, const Individuo &padre2);

    [[nodiscard]] std::vector<int> cruceOX2(const Individuo &padre_a, const Individuo &padre_b) const;

    [[nodiscard]] std::vector<int> cruceMOC(const Individuo &padre_a, const Individuo &padre_b) const;

    [[maybe_unused]] [[nodiscard]] std::vector<int> crucePMX(const Individuo &padre_a, const Individuo &padre_b) const;

    [[maybe_unused]] std::vector<int> cruceSimple(const Individuo &padre_a, const Individuo &padre_b) const;

    Individuo *torneo_kworst();

    Individuo mejor_entre_random();

    std::vector<int> camino_greedy();
};

#endif //META_PRACTICA_02_POBLACION_H
