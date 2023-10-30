#include <unordered_map>
#include "Poblacion.h"

void Poblacion::crear_poblacion() {

    for (int i = 0; i < num_invididuos; ++i) {
        std::vector<int> ruta_inicial = vector_aleatorio(num_ciudades);
        Individuo ind(ruta_inicial, lector_datos);
        individuos.push_back(ind);
    }

    // Aquí podrías implementar la lógica para crear_poblacion los individuos,
    // generando un 80% de forma aleatoria y el 20% restante mediante un
    // método greedy aleatorizado.
}

bool Poblacion::condicion_parada() const {
    return (num_evuaciones > 1000);
}

void Poblacion::evolucion_generacional() {
    while (!condicion_parada()) {
        std::vector<Individuo> nueva_poblacion;
        nueva_poblacion.reserve(num_invididuos);

        // Aplicar elitismo: conservar los mejores individuos
        if (num_elites > 0) {
            std::sort(individuos.begin(), individuos.end());
            nueva_poblacion.insert(nueva_poblacion.end(), individuos.begin(), individuos.begin() + num_elites);
        }

        // Generar el resto de la nueva población
        while (nueva_poblacion.size() < num_invididuos) {
            Individuo padre1 = seleccionar();
            Individuo padre2 = seleccionar();
            Individuo hijo = cruzar(padre1, padre2);
            mutar(hijo);
            nueva_poblacion.push_back(hijo);
        }

        // Reemplazar la población antigua con la nueva
        individuos = nueva_poblacion;

        // Evaluar la nueva población
        for (Individuo &individuo: individuos) {
            individuo.calcular_coste_camino();
        }

        num_evuaciones++;
    }
}

Individuo Poblacion::cruzar(const Individuo &padre1, const Individuo &padre2) {
    double probabilidad_cruzar = random.get_double(0.0, 1.0);

    if (probabilidad_cruzar < 0.7) { // 70% de probabilidad_cruzar de cruce
        if (random.get_double(0.0, 1.0) < 0.5) { // Elige uno de los dos operadores de cruce
            return cruceOX2(padre1, padre2);
        } else {
            return crucePMX(padre1, padre2);
        }
    } else {
        // Si no hay cruce, devuelve uno de los padres
        return (random.get_double(0.0, 1.0) < 0.5) ? padre1 : padre2;
    }
}

Individuo Poblacion::cruceOX2(const Individuo &padre1, const Individuo &padre2) {
    int puntoCruce1 = random.get_int(0, num_invididuos - 1);
    int puntoCruce2 = random.get_int(0, num_invididuos - 1);

    // Asegurarse de que puntoCruce1 < puntoCruce2
    if (puntoCruce1 > puntoCruce2) {
        std::swap(puntoCruce1, puntoCruce2);
    }

    std::vector<int> hijo(num_invididuos);
    std::unordered_set<int> ciudadesEnHijo;

    // Copiar la sección del primer padre a la descendencia
    for (int i = puntoCruce1; i <= puntoCruce2; ++i) {
        hijo[i] = padre1.get_ciudad(i);
        ciudadesEnHijo.insert(hijo[i]);
    }

    // Completar la descendencia con las ciudades del segundo padre
    for (int i = 0, j = 0; i < num_invididuos; ++i) {
        if (i >= puntoCruce1 && i <= puntoCruce2) continue;
        while (ciudadesEnHijo.count(padre2.get_ciudad(j))) {
            ++j;
        }
        hijo[i] = padre2.get_ciudad(j++);
    }

    return Individuo(hijo, lector_datos);
}

Individuo Poblacion::crucePMX(const Individuo &padre1, const Individuo &padre2) {

    int puntoCruce1 = random.get_int(0, num_invididuos - 1);
    int puntoCruce2 = random.get_int(0, num_invididuos - 1);

    // Asegurarse de que puntoCruce1 < puntoCruce2
    if (puntoCruce1 > puntoCruce2) {
        std::swap(puntoCruce1, puntoCruce2);
    }

    std::vector<int> hijo(num_invididuos);
    std::unordered_map<int, int> mapeo;

    // Intercambiar los segmentos entre los puntos de cruce
    for (int i = puntoCruce1; i <= puntoCruce2; ++i) {
        hijo[i] = padre1.get_ciudad(i);
        mapeo[padre1.get_ciudad(i)] = padre2.get_ciudad(i);
    }

    // Completar la descendencia resolviendo los conflictos de duplicados
    for (int i = 0; i < num_invididuos; ++i) {
        if (i >= puntoCruce1 && i <= puntoCruce2) continue;
        int ciudad = padre2.get_ciudad(i);
        while (mapeo.count(ciudad)) {
            ciudad = mapeo[ciudad];
        }
        hijo[i] = ciudad;
    }

    return Individuo(hijo, lector_datos);
}

Individuo Poblacion::seleccionar() {

    int indice1 = random.get_int(0, num_invididuos - 1);
    int indice2 = random.get_int(0, num_invididuos - 1);

    while (indice1 == indice2) {
        indice2 = random.get_int(0, num_invididuos - 1); // Asegurarse de que son diferentes
    }

    if (individuos[indice1] < individuos[indice2]) {
        return individuos[indice1];
    } else {
        return individuos[indice2];
    }
}

void Poblacion::mutar(Individuo &individuo) const {

    // Aplicar mutación con una probabilidad del 10%

    double dado_mutar = random.get_double(0.0, 1.0);

    if (dado_mutar < 0.1) {
        int posicion1 = random.get_int(0, num_invididuos - 1);
        int posicion2 = random.get_int(0, num_invididuos - 1);

        // Asegurarse de que las dos posiciones son diferentes
        while (posicion1 == posicion2) {
            posicion2 = random.get_int(0, num_invididuos - 1);
        }

        // Realizar el intercambio 2-opt
        individuo.intercambiar(posicion1, posicion2);
    }
}

Poblacion::~Poblacion() = default;

Poblacion::Poblacion(int num_individuos, LectorCiudades &lector_datos) : num_invididuos(num_individuos),
                                                                         lector_datos(lector_datos) {
    // Individuos
    individuos.reserve(num_individuos);
    num_ciudades = lector_datos.get_num_ciudades();
    crear_poblacion();

    num_elites = 1;
    num_evuaciones = 0;
}
