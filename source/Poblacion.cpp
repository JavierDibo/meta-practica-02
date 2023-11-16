#include "Poblacion.h"

void Poblacion::encontrar_elites(const std::vector<Individuo> &ind, std::vector<Individuo> &elites) const {
    if (num_elites > 0) {
        std::vector<Individuo> aux = ind;
        std::sort(aux.begin(), aux.end());
        elites.clear();
        elites.insert(elites.end(), aux.begin(), aux.begin() + num_elites);
    }
}

void Poblacion::avanzar_poblacion_generacional(std::vector<Individuo> &nueva_poblacion) {

    for (int i = 0; i < num_invididuos; i++) {
        Individuo padre_a = torneo_kbest(kbest);
        Individuo padre_b = torneo_kbest(kbest);
        std::vector<int> camino_hijo = cruzar(padre_a, padre_b);
        Individuo hijo(camino_hijo, lector_datos);
        hijo.mutar();
        num_evaluaciones++;
        hijo.evaluar();
        nueva_poblacion.push_back(hijo);
    }
}

void Poblacion::evolucion_generacional() {

    reloj.iniciar();

    while (!condicion_parada()) {
        /// Aplicar encontrar_elites: almacenar los "num_elites" mejores individuos
        std::vector<Individuo> elites;
        elites.reserve(num_elites);
        encontrar_elites(individuos, elites);

        /// Generar la nueva población
        std::vector<Individuo> nueva_poblacion;
        nueva_poblacion.reserve(num_invididuos);
        avanzar_poblacion_generacional(nueva_poblacion);

        /// Reemplazar los peores individuos por los elites
        introducir_elites(elites, nueva_poblacion);

        /// Reemplazar la población antigua con la nueva
        individuos = nueva_poblacion;
        num_generaciones++;
    }

    NUM_GENERACIONES_SEMILLA = num_generaciones;
    NUM_EVALUACIONES_SEMILLA = num_evaluaciones;
    reloj.finalizar();
}

std::vector<int> Poblacion::camino_greedy(const std::vector<std::vector<float>> &distancias, int ciudad_actual) const {
    std::vector<bool> visitadas(num_ciudades, false);
    std::vector<int> camino;
    camino.reserve(num_ciudades + 1);

    visitadas[ciudad_actual] = true;
    camino.push_back(ciudad_actual);

    for (int i = 1; i < num_ciudades; ++i) {
        int siguiente_ciudad = -1;
        double min_distancia = INFINITO_POSITIVO;

        int local_siguiente_ciudad = -1;
        double local_min_distancia = INFINITO_POSITIVO;

        for (int j = 0; j < num_ciudades; ++j) {
            if (!visitadas[j] && distancias[ciudad_actual][j] < local_min_distancia) {
                local_min_distancia = distancias[ciudad_actual][j];
                local_siguiente_ciudad = j;
            }
        }

        if (local_min_distancia < min_distancia) {
            min_distancia = local_min_distancia;
            siguiente_ciudad = local_siguiente_ciudad;
        }

        if (siguiente_ciudad != -1) {
            visitadas[siguiente_ciudad] = true;
            camino.push_back(siguiente_ciudad);
            ciudad_actual = siguiente_ciudad;
        }
    }

    return camino;
}

const std::string GREEN = "\033[32m";   // Green
const std::string RESET = "\033[0m";    // Reset to default color

void Poblacion::crear_poblacion() {

    Reloj reloj_creacion;
    reloj_creacion.iniciar();

    individuos.reserve(num_invididuos);

    std::cout << GREEN;

    if (ECHO) {
        std::cout << std::endl;
        std::cout << "Generando poblacion inicial..." << std::endl;
    }

    int num_random = static_cast<int>(num_invididuos * (1 - PROBABILIDAD_GREEDY));

    for (int i = 0; i < num_random; ++i) {
        std::vector<int> ruta_inicial;
        ruta_inicial = vector_aleatorio(num_ciudades);

        Individuo individuo(ruta_inicial, lector_datos);
        individuo.evaluar();
        individuos.emplace_back(individuo);
    }

    int num_greedy = num_invididuos - num_random;
    individuos.resize(individuos.size() + num_greedy, Individuo({}, lector_datos));
    std::vector<std::vector<float>> distancias = lector_datos.get_distancias();

    std::vector<int> random_cities(num_greedy);
    for (int i = 0; i < num_greedy; ++i) {
        random_cities[i] = get_rand_int(0, num_ciudades - 1);
    }

#pragma omp parallel for default(none) if (true) shared(distancias, lector_datos, random_cities, num_random, individuos, num_greedy)
    for (int i = 0; i < num_greedy; ++i) {
        std::vector<int> ruta_inicial;
        int ciudad_random = random_cities[i];
        ruta_inicial = camino_greedy(distancias, ciudad_random);
        Individuo individuo(ruta_inicial, lector_datos);
        individuo.evaluar();
        individuos[num_random + i] = individuo;
    }

    reloj_creacion.finalizar();

    double tiempo = reloj_creacion.obtener_tiempo_transcurrido(MILISEGUNDOS);

    if (ECHO) {
        std::cout << "Tiempo para generar la poblacion inicial: " << tiempo << " milisegundos. " <<
                  std::endl;
    }

    std::cout << RESET;
}

bool Poblacion::condicion_parada() {

    reloj.finalizar();
    double tiempo = reloj.obtener_tiempo_transcurrido(MILISEGUNDOS);

    return (num_generaciones >= MAX_NUMERO_GENERACIONES ||
            tiempo >= MAX_TIEMPO_EJECUCION * 1000 ||
            num_evaluaciones >= MAX_NUM_EVALUACIONES);
}

void Poblacion::introducir_elites(const std::vector<Individuo> &elites, const std::vector<Individuo> &nueva_poblacion) {
    for (const Individuo &elite: elites) {
        Individuo *peor = torneo_kworst();
        *peor = elite;
    }
}

Individuo Poblacion::individuo_aleatorio() {
    return torneo_kbest(1);
}

void Poblacion::intercambiar(int posicion1, int posicion2, std::vector<int> camino) {
    if (!(posicion1 >= 0 && posicion1 < camino.size() && posicion2 >= 0 && posicion2 < camino.size())) {
        throw std::out_of_range("Individuo::intercambiar::fuera de rango");
    }

    std::swap(camino[posicion1], camino[posicion2]);
}

std::vector<int> Poblacion::cruce_ternario_diferencial(const Individuo &padre, Individuo &aleatorio_1,
                                                       Individuo &aleatorio_2, Individuo &objetivo) {

    std::vector<int> camino_hijo = padre.get_camino();
    std::vector<int> camino_a1 = aleatorio_1.get_camino();
    // std::vector<int> camino_a2 = aleatorio_2.get_camino();
    // std::vector<int> camino_o = objetivo.get_camino();

    int tam = static_cast<int>(camino_hijo.size()) - 1;

    int indice_aleatorio = get_rand_int(0, tam - 1);

    // intercambiar(indice_aleatorio, indice_aleatorio + 1, camino_hijo);

    int indice_a = camino_hijo[indice_aleatorio];

    // int valor2 = camino_hijo[indice_aleatorio+1];

    /// Intercambio cruzado del primer valor (indice_a) con camino_a1

    for (int i = 0; i < tam; ++i) {
        if (camino_a1[i] == indice_a) {}
    }

    return camino_hijo;
}

void Poblacion::avanzar_poblacion_diferencial(std::vector<Individuo> &nueva_poblacion) {

    for (Individuo &padre: individuos) {

        /// Genero los tres Individuos necesarios para el cruce
        Individuo objetivo = torneo_kbest(kbest);
        Individuo aleatorio_1 = individuo_aleatorio();
        Individuo aleatorio_2 = individuo_aleatorio();

        while (aleatorio_1 == objetivo) {
            aleatorio_1 = individuo_aleatorio();
        }

        while (aleatorio_2 == aleatorio_1) {
            aleatorio_2 = individuo_aleatorio();
        }

        /// Creo el camino para el hijo
        std::vector<int> camino_hijo;
        camino_hijo = cruce_ternario_diferencial(padre, aleatorio_1, aleatorio_2, objetivo);

        /// Creo y evaluo el hijo
        Individuo hijo(camino_hijo, lector_datos);
        hijo.evaluar();
        num_evaluaciones++;

        /// Si el hijo mejora al padre, lo meto en la nueva poblacion
        if (hijo < padre) {
            nueva_poblacion.push_back(hijo);
        } else {
            nueva_poblacion.push_back(padre);
        }
    }

    num_generaciones++;
}

void Poblacion::evolucion_diferencial() {
    while (!condicion_parada()) {

        /// Generar la nueva población
        std::vector<Individuo> nueva_poblacion;
        nueva_poblacion.reserve(num_invididuos);
        avanzar_poblacion_diferencial(nueva_poblacion);

        /// Reemplazar la población antigua con la nueva
        individuos = nueva_poblacion;
        num_generaciones++;
    }
}

std::vector<int> Poblacion::cruzar(const Individuo &padre1, const Individuo &padre2) {
    double probabilidad_cruzar = get_rand_double(0.0, 1.0);

    if (probabilidad_cruzar < PROBABILIDAD_CRUCE) {
        if (get_rand_double(0.0, 1.0) < 0.5) { // Elige uno de los dos operadores de cruce
            return cruceOX2(padre1, padre2);
        } else {
            return cruceMOC(padre1, padre2);
        }
    } else {
        /// Si no hay cruce, devuelve el mejor de los padres
        if (padre1 < padre2) {
            return padre1.get_camino();
        } else {
            return padre2.get_camino();
        }
    }
}

std::vector<int> Poblacion::cruceOX2(const Individuo &padre_a, const Individuo &padre_b) const {
    std::vector<int> camino_hijo = padre_a.get_camino(); // Copia del camino del padre_a
    std::unordered_set<int> ciudadesSeleccionadas;
    std::vector<int> posicionesSeleccionadas;

    // Seleccionar aleatoriamente las posiciones a reordenar
    while (posicionesSeleccionadas.size() < num_ciudades / 2) {
        int pos = get_rand_int(0, num_ciudades - 1);
        if (ciudadesSeleccionadas.find(padre_a.get_ciudad(pos)) == ciudadesSeleccionadas.end()) {
            ciudadesSeleccionadas.insert(padre_a.get_ciudad(pos));
            posicionesSeleccionadas.push_back(pos);
        }
    }

    // Eliminar las ciudades seleccionadas del camino_hijo basado en las posiciones seleccionadas
    for (int pos: posicionesSeleccionadas) {
        camino_hijo[pos] = -1; // Marcamos la posición como vacía
    }

    // Rellenar las posiciones vacías en el camino_hijo con las ciudades en el mismo orden que aparecen en padre_b
    int indexParaInsertar = 0;
    for (int ciudad: padre_b.get_camino()) {
        if (ciudadesSeleccionadas.find(ciudad) != ciudadesSeleccionadas.end()) {
            // Encontrar la próxima posición vacía en el camino_hijo
            while (indexParaInsertar < camino_hijo.size() && camino_hijo[indexParaInsertar] != -1) {
                indexParaInsertar++;
            }

            // Comprobar si hemos salido del rango del vector
            if (indexParaInsertar >= camino_hijo.size()) {
                // Manejar el error o romper el bucle
                break;
            }

            camino_hijo[indexParaInsertar] = ciudad;
        }
    }

    return camino_hijo;
}

std::vector<int> Poblacion::cruceMOC(const Individuo &padre_a, const Individuo &padre_b) const {
    int punto_cruce = get_rand_int(0, num_ciudades - 1);
    std::vector<int> camino_hijo(num_ciudades, -1);
    std::unordered_set<int> ciudades_incluidas;

    for (int i = 0; i < punto_cruce; ++i) {
        int ciudad = padre_a.get_ciudad(i);
        camino_hijo[i] = ciudad;
        ciudades_incluidas.insert(ciudad);
    }

    for (int i = punto_cruce; i < num_ciudades; ++i) {
        int ciudad = padre_b.get_ciudad(i);
        if (ciudades_incluidas.insert(ciudad).second) {
            camino_hijo[i] = ciudad;
        }
    }

    std::vector<int> ciudades_restantes;
    for (int ciudad: padre_b.get_camino()) {
        if (ciudades_incluidas.insert(ciudad).second) {
            ciudades_restantes.push_back(ciudad);
        }
    }

    for (int i = punto_cruce, j = 0; i < num_ciudades && j < ciudades_restantes.size(); ++i) {
        if (camino_hijo[i] == -1) {
            camino_hijo[i] = ciudades_restantes[j++];
        }
    }

    return camino_hijo;
}

// Torneo kbest
Individuo Poblacion::torneo_kbest(int kbest) {
    if (kbest > num_invididuos) {
        throw std::runtime_error("KBEST no puede ser mayor que num_invididuos");
    }

    std::unordered_set<int> indices_unicos;
    while (indices_unicos.size() < kbest) {
        int indice;
        do {
            indice = get_rand_int(0, num_invididuos - 1);
        } while (indices_unicos.find(indice) != indices_unicos.end());

        indices_unicos.insert(indice);
    }

    auto mejor_indice = *indices_unicos.begin();
    for (auto indice: indices_unicos) {
        if (individuos[mejor_indice] > individuos[indice]) {
            mejor_indice = indice;
        }
    }

    return individuos[mejor_indice];
}

Individuo *Poblacion::torneo_kworst() {
    if (KWORST > num_invididuos) {
        // Manejar este error adecuadamente
        throw std::runtime_error("KWORST no puede ser mayor que num_invididuos");
    }

    std::unordered_set<int> indices_unicos;
    while (indices_unicos.size() < KWORST) {
        int indice = get_rand_int(0, num_invididuos - 1);
        indices_unicos.insert(indice);
    }

    auto peor_indice = *indices_unicos.begin();
    for (auto indice: indices_unicos) {
        if (individuos[peor_indice] < individuos[indice]) {
            peor_indice = indice;
        }
    }

    return &individuos[peor_indice];
}

Poblacion::~Poblacion() = default;

Poblacion::Poblacion(LectorCiudades &lector_datos, int num_ind, int kb, int elites) : lector_datos(lector_datos) {
    num_invididuos = num_ind;
    kbest = kb;
    num_ciudades = lector_datos.get_num_ciudades();
    num_elites = elites;
    crear_poblacion();
}

const std::vector<Individuo> &Poblacion::get_individuos() const {
    return individuos;
}
