#include "Poblacion.h"

std::vector<int> Poblacion::get_ciudades_cercanas(int ciudad) {
    const auto& distancias_totales = lector_datos.getDistancias(); // Use reference to avoid copying
    const auto& distancias_a_ciudad = distancias_totales[ciudad]; // Use reference to avoid copying

    std::vector<std::pair<double, int>> distancias_indizadas;
    distancias_indizadas.reserve(distancias_a_ciudad.size());

    int tam = static_cast<int>(distancias_a_ciudad.size());
    for (int i = 0; i < tam; ++i) {
        if (i != ciudad) { // Omit the distance to the city itself
            distancias_indizadas.emplace_back(distancias_a_ciudad[i], i);
        }
    }

    // Sort by the first element of the pair (distance), keeping the second element (index) with it
    std::sort(distancias_indizadas.begin(), distancias_indizadas.end());

    std::vector<int> mejores_ciudades;
    mejores_ciudades.reserve(distancias_indizadas.size());

    for (const auto& par : distancias_indizadas) {
        mejores_ciudades.emplace_back(par.second);
    }

    return mejores_ciudades;
}

std::vector<int> Poblacion::camino_greedy() {
    // Verificar casos extremos
    if (num_ciudades <= 0) {
        // Manejar error o devolver un vector vacío
        return {};
    }

    std::vector<int> ruta;
    std::unordered_set<int> visitados;

    int indice = random.get_int(0, num_ciudades - 1);

    for (int i = 0; i < num_ciudades; i++) {
        if (visitados.find(indice) != visitados.end()) {
            // Todas las ciudades han sido visitadas
            break;
        }

        ruta.emplace_back(indice);
        visitados.insert(indice);

        std::vector<int> ciudades_mas_cercanas = get_ciudades_cercanas(indice);

        // Encontrar la primera ciudad no visitada
        bool se_encontro_nueva_ciudad = false;
        for (int ciudad : ciudades_mas_cercanas) {
            if (visitados.find(ciudad) == visitados.end()) {
                indice = ciudad;
                se_encontro_nueva_ciudad = true;
                break; // salir del bucle después de encontrar la primera ciudad no visitada
            }
        }

        // Si no se encuentra una nueva ciudad, salir del bucle
        if (!se_encontro_nueva_ciudad) {
            break;
        }
    }

    return ruta;
}

#include <mutex>

void Poblacion::crear_poblacion() {
    individuos.reserve(num_invididuos);

    std::mutex mtx; // Mutex for thread-safe access to 'individuos'

#pragma omp parallel for default(none) shared(random, lector_datos, mtx)
    for (int i = 0; i < num_invididuos; ++i) {
        std::vector<int> ruta_inicial;

        if (random.get_double(0.0, 1.0) < 0.8) {
            ruta_inicial = vector_aleatorio(num_ciudades);
        } else {
            ruta_inicial = camino_greedy();
        }

        // Thread-safe insertion into 'individuos'
        std::lock_guard<std::mutex> guard(mtx);
        individuos.emplace_back(ruta_inicial, lector_datos);
    }
}

bool Poblacion::condicion_parada() {

    reloj.finalizar();
    double tiempo = reloj.obtener_tiempo_transcurrido(SEGUNDOS);

    return (num_evuaciones > MAX_NUM_EVALUACIONES || tiempo > 60.0);
}

void Poblacion::evolucionar() {
    while (!condicion_parada()) {
        std::vector<Individuo> nueva_poblacion;
        nueva_poblacion.reserve(num_invididuos);
        std::vector<Individuo> elites;
        elites.reserve(NUMERO_ELITES);

        /// Aplicar elitismo: almacenar los "num_elites" mejores individuos
        if (NUMERO_ELITES > 0) {
            std::sort(individuos.begin(), individuos.end());
            elites.clear();
            elites.insert(elites.end(), individuos.begin(), individuos.begin() + NUMERO_ELITES);
        }

        /// Generar la nueva población
        while (nueva_poblacion.size() < num_invididuos) {
            Individuo padre_a = torneo_kbest();
            Individuo padre_b = torneo_kbest();
            Individuo hijo = cruzar(padre_a, padre_b);
            hijo.mutar();
            nueva_poblacion.push_back(hijo);
        }

        /// Buscar los elites en la nueva poblacion y reemplazarlos si no estan por uno malo
        for (const Individuo &elite: elites) {
            // Si no está en la poblacion lanzo torneo kworst
            if (std::find(nueva_poblacion.begin(), nueva_poblacion.end(), elite) == nueva_poblacion.end()) {
                Individuo *peor = torneo_kworst();
                *peor = elite;
            }
        }

        /// Reemplazar la población antigua con la nueva
        individuos = nueva_poblacion;

        /// Evaluar la nueva población
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
            return cruceMOC(padre1, padre2);
        }
    } else {
        return (random.get_double(0.0, 1.0) < 0.5) ? padre1 : padre2;
        /// Si no hay cruce, devuelve uno de los padres
        if (padre1 < padre2) {
            return padre1;
        } else {
            return padre2;
        }
    }
}

Individuo Poblacion::cruceOX2(const Individuo &padre_a, const Individuo &padre_b) {
    std::vector<int> hijo = padre_a.get_camino(); // Copia del camino del padre_a
    std::unordered_set<int> ciudadesSeleccionadas;
    std::vector<int> posicionesSeleccionadas;

    // Seleccionar aleatoriamente las posiciones a reordenar
    while (posicionesSeleccionadas.size() < num_ciudades / 2) {
        int pos = random.get_int(0, num_ciudades - 1);
        if (ciudadesSeleccionadas.find(padre_a.get_ciudad(pos)) == ciudadesSeleccionadas.end()) {
            ciudadesSeleccionadas.insert(padre_a.get_ciudad(pos));
            posicionesSeleccionadas.push_back(pos);
        }
    }

    // Eliminar las ciudades seleccionadas del hijo basado en las posiciones seleccionadas
    for (int pos: posicionesSeleccionadas) {
        hijo[pos] = -1; // Marcamos la posición como vacía
    }

    // Rellenar las posiciones vacías en el hijo con las ciudades en el mismo orden que aparecen en padre_b
    int indexParaInsertar = 0;
    for (int ciudad: padre_b.get_camino()) {
        if (ciudadesSeleccionadas.find(ciudad) != ciudadesSeleccionadas.end()) {
            // Encontrar la próxima posición vacía en el hijo
            while (hijo[indexParaInsertar] != -1) {
                indexParaInsertar++;
            }
            hijo[indexParaInsertar] = ciudad;
        }
    }

    return {hijo, lector_datos};
}

Individuo Poblacion::cruceMOC(const Individuo &padre_a, const Individuo &padre_b) {

    int punto_cruce = random.get_int(0, num_ciudades - 1);
    std::vector<int> camino_hijo(num_ciudades, -1);

    // Copiar las ciudades del padre_a hasta el punto de cruce
    for (int i = 0; i < punto_cruce; ++i) {
        camino_hijo[i] = padre_a.get_ciudad(i);
    }

    // Copiar las ciudades del padre_b si no estan ya
    for (int i = punto_cruce; i < num_ciudades; ++i) {
        int ciudad = padre_b.get_ciudad(i);
        if (std::find(camino_hijo.begin(), camino_hijo.begin() + punto_cruce, ciudad) ==
            camino_hijo.begin() + punto_cruce) {
            camino_hijo[i] = ciudad;
        }
    }

    // Completar con el resto de ciudades que faltan
    for (int i = punto_cruce; i < num_ciudades; ++i) {
        if (camino_hijo[i] == -1) { // Si falta
            for (int ciudad: padre_b.get_camino()) {
                if (std::find(camino_hijo.begin(), camino_hijo.end(), ciudad) == camino_hijo.end()) {
                    camino_hijo[i] = ciudad;
                    break;
                }
            }
        }
    }

    return {camino_hijo, lector_datos};
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

Individuo Poblacion::torneo_kbest() {
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

Individuo *Poblacion::torneo_kworst() {
    std::vector<int> indices;
    for (int i = 0; i < KWORST; ++i) {
        int indice;
        do {
            indice = random.get_int(0, num_invididuos - 1);
        } while (std::find(indices.begin(), indices.end(), indice) !=
                 indices.end()); // Asegurarse de que no esté repetido
        indices.push_back(indice);
    }

    int peor_indice = indices[0];
    for (int i = 1; i < KWORST; ++i) {
        if (individuos[peor_indice] > individuos[indices[i]]) {
            peor_indice = indices[i];
        }
    }

    return &individuos[peor_indice];
}

Poblacion::~Poblacion() = default;

Poblacion::Poblacion(LectorCiudades &lector_datos) : lector_datos(lector_datos) {
    // Individuos
    individuos.reserve(NUMERO_INDIVIDUOS);
    num_ciudades = lector_datos.get_num_ciudades();
    num_invididuos = NUMERO_INDIVIDUOS;
    crear_poblacion();
    num_evuaciones = 0;
    reloj.iniciar();
}

const std::vector<Individuo> &Poblacion::get_individuos() const {
    return individuos;
}
