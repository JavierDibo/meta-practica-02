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
#include "Reloj.h"

class Poblacion {
private:

    std::vector<Individuo> individuos;
    int num_invididuos;
    int kbest;
    int num_ciudades;
    int num_generaciones = 0;
    int num_evaluaciones = 0;
    LectorCiudades &lector_datos;
    Reloj reloj;

public:

    Poblacion(LectorCiudades &lector_datos, int num_ind, int kb);

    virtual ~Poblacion();

    [[nodiscard]] const std::vector<Individuo> &get_individuos() const;

    void evolucion_generacional();

    void evolucion_diferencial();

private:

    void crear_poblacion();

    bool condicion_parada();

    std::vector<int> cruzar(const Individuo &padre1, const Individuo &padre2);

    [[nodiscard]] std::vector<int> cruceOX2(const Individuo &padre_a, const Individuo &padre_b) const;

    [[nodiscard]] std::vector<int> cruceMOC(const Individuo &padre_a, const Individuo &padre_b) const;

    Individuo *torneo_kworst();

    // Torneo kbest
    Individuo torneo_kbest(int kbest);

    void avanzar_poblacion_generacional(std::vector<Individuo> &nueva_poblacion);

    void avanzar_poblacion_diferencial(std::vector<Individuo> &nueva_poblacion);

    void introducir_elites(const std::vector<Individuo> &elites, const std::vector<Individuo> &nueva_poblacion);

    static void encontrar_elites(const std::vector<Individuo> &individuos, std::vector<Individuo> &elites);

    Individuo individuo_aleatorio();

    static void intercambiar(int posicion1, int posicion2, std::vector<int> camino);

    std::vector<int> cruce_ternario_diferencial(const Individuo &padre, Individuo &aleatorio_1, Individuo &aleatorio_2,
                                                Individuo &objetivo);

    [[nodiscard]] std::vector<int>
    camino_greedy(const std::vector<std::vector<float>> &distancias, int ciudad_actual) const;
};

#endif //META_PRACTICA_02_POBLACION_H
