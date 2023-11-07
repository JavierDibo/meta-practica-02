#include <unordered_map>
#include "Poblacion.h"

void Poblacion::crear_poblacion() {
    // 80% de los individuos aleatorios
    for (int i = 0; i < num_invididuos; ++i) {
        std::vector<int> ruta_inicial = vector_aleatorio(num_ciudades);
        Individuo ind(ruta_inicial, lector_datos);
        individuos.push_back(ind);
    }

    /// TODO añadir greedy aleatorizado para el 20% restante
}

bool Poblacion::condicion_parada() const {
    /// TODO añadir condicion de parada de 60 segundos
    return (num_evuaciones > MAX_NUM_EVALUACIONES);
}

void Poblacion::evolucionar() {
    while (!condicion_parada()) {
        std::vector<Individuo> nueva_poblacion;
        nueva_poblacion.reserve(num_invididuos);

        /// Aplicar elitismo: conservar los "num_elites" mejores individuos
        if (NUMERO_ELITES > 0) {
            std::sort(individuos.begin(), individuos.end());
            nueva_poblacion.insert(nueva_poblacion.end(), individuos.begin(), individuos.begin() + NUMERO_ELITES);
        }

        /// Generar el resto de la nueva población
        while (nueva_poblacion.size() < num_invididuos) {
            Individuo padre_a = seleccionar();
            Individuo padre_b = seleccionar();
            Individuo hijo = cruzar(padre_a, padre_b);
            hijo.mutar();
            nueva_poblacion.push_back(hijo);
        }

        /// Reemplazar la población antigua con la nueva
        individuos = nueva_poblacion;

        // Evaluar la nueva población
#pragma omp parallel for default(none) if (PARALELIZACION)
        for (Individuo &individuo: individuos) {
            individuo.evaluar();
            num_evuaciones++;
        }
        generacion++;
    }
}

Individuo Poblacion::cruzar(const Individuo &padre1, const Individuo &padre2) {
    double probabilidad_cruzar = random.get_double(0.0, 1.0);

    if (probabilidad_cruzar < PROBABILIDAD_CRUCE) { // 70% de probabilidad_cruzar de cruce
        if (random.get_double(0.0, 1.0) < 0.5) { // Elige uno de los dos operadores de cruce
            return cruceOX2(padre1, padre2);
        } else {
            return crucePMX(padre1, padre2);
        }
    } else {
        // return (random.get_double(0.0, 1.0) < 0.5) ? padre1 : padre2;
        /// Si no hay cruce, devuelve uno de los padres
        if (padre1 < padre2) {
            return padre1;
        } else {
            return padre2;
        }
    }
}

Individuo Poblacion::cruceOX2(const Individuo &padre_a, const Individuo &padre_b) {
    int punto_cruce_a = random.get_int(0, num_invididuos - 1);
    int punto_cruce_b = random.get_int(0, num_invididuos - 1);

    // Asegurarse de que punto_cruce_a < punto_cruce_b
    if (punto_cruce_a > punto_cruce_b) {
        std::swap(punto_cruce_a, punto_cruce_b);
    }

    std::vector<int> hijo(num_invididuos);
    std::unordered_set<int> mapeo;

    // Copiar la sección del primer padre a la descendencia
    for (int i = punto_cruce_a; i <= punto_cruce_b; ++i) {
        hijo[i] = padre_a.get_ciudad(i);
        mapeo.insert(hijo[i]);
    }

    // Completar la descendencia con las ciudades del segundo padre
    for (int i = 0, j = 0; i < num_invididuos; ++i) {
        if (i >= punto_cruce_a && i <= punto_cruce_b) continue;
        while (mapeo.count(padre_b.get_ciudad(j))) {
            ++j;
        }
        hijo[i] = padre_b.get_ciudad(j++);
    }

    return {hijo, lector_datos};
}

Individuo Poblacion::crucePMX(const Individuo &padre_a, const Individuo &padre_b) {

    int punto_cruce_a = random.get_int(0, num_invididuos - 1);
    int punto_cruce_b = random.get_int(0, num_invididuos - 1);

    // Asegurarse de que punto_cruce_a < punto_cruce_b
    if (punto_cruce_a > punto_cruce_b) {
        std::swap(punto_cruce_a, punto_cruce_b);
    }

    std::vector<int> hijo(num_invididuos);
    std::unordered_map<int, int> mapeo;

    // Intercambiar los segmentos entre los puntos de cruce
    for (int i = punto_cruce_a; i <= punto_cruce_b; ++i) {
        hijo[i] = padre_a.get_ciudad(i);
        mapeo[padre_a.get_ciudad(i)] = padre_b.get_ciudad(i);
    }

    // Completar la descendencia resolviendo los conflictos de duplicados
    for (int i = 0; i < num_invididuos; ++i) {
        if (i >= punto_cruce_a && i <= punto_cruce_b) continue;
        int ciudad = padre_b.get_ciudad(i);
        while (mapeo.count(ciudad)) {
            ciudad = mapeo[ciudad];
        }
        hijo[i] = ciudad;
    }

    return {hijo, lector_datos};
}

Individuo Poblacion::seleccionar() {
    std::vector<int> indices;
    for (int i = 0; i < KBEST; ++i) {
        int indice;
        do {
            indice = random.get_int(0, num_invididuos - 1);
        } while (std::find(indices.begin(), indices.end(), indice) !=
                 indices.end()); // Asegurarse de que no esté repetido
        indices.push_back(indice);
    }

    int mejorIndice = indices[0];
    for (int i = 1; i < KBEST; ++i) {
        if (individuos[mejorIndice] < individuos[indices[i]]) {
            mejorIndice = indices[i];
        }
    }

    return individuos[mejorIndice];
}

Poblacion::~Poblacion() = default;

Poblacion::Poblacion(LectorCiudades &lector_datos) : lector_datos(lector_datos) {
    // Individuos
    individuos.reserve(NUMERO_INDIVIDUOS);
    num_ciudades = lector_datos.get_num_ciudades();
    num_invididuos = NUMERO_INDIVIDUOS;
    crear_poblacion();
    num_evuaciones = 0;
}

const std::vector<Individuo> &Poblacion::get_individuos() const {
    return individuos;
}
