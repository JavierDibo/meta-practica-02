#include "Individuo.h"

Individuo::Individuo(std::vector<int> camino, LectorCiudades &lector_datos) : camino(std::move(camino)),
                                                                              lector_ciudades(lector_datos) {
}

Individuo::Individuo(const Individuo &otro) = default;

void Individuo::mutar() {
    // Aplicar mutación con una probabilidad del 10%
    double dado_mutar = get_rand_double(0.0, 1.0);
    int tam = static_cast<int>(camino.size());

    if (dado_mutar < PROBABILIDAD_MUTACION) {
        int posicion1 = get_rand_int(0, tam - 1);
        int posicion2 = get_rand_int(0, tam - 1);

        // Asegurarse de que las dos posiciones son diferentes
        while (posicion1 == posicion2) {
            posicion2 = get_rand_int(0, tam - 1);
        }

        // Realizar el intercambio 2-opt
        intercambiar(posicion1, posicion2);
    }
}

void Individuo::intercambiar(int posicion1, int posicion2) {
    if (posicion1 >= 0 && posicion1 < camino.size() && posicion2 >= 0 && posicion2 < camino.size()) {
        std::swap(camino[posicion1], camino[posicion2]);
    } else {
        throw std::out_of_range("Individuo::intercambiar::fuera de rango");
    }
}

Individuo &Individuo::operator=(const Individuo &otro) {
    if (this != &otro) {
        camino = otro.camino;
        coste = otro.coste;
    }
    return *this;
}

bool hasDuplicates(const std::vector<int> &vec) {
    std::unordered_set<int> elements;

    for (int value: vec) {
        if (elements.find(value) != elements.end()) {
            // Si el valor ya se encuentra en el conjunto, hay un duplicado
            return true;
        }
        // Si el valor no se encuentra, se agrega al conjunto
        elements.insert(value);
    }

    // Si terminamos de revisar todos los elementos y no encontramos duplicados
    return false;
}

void Individuo::evaluar() {

    hasDuplicates(camino);

    coste = 0.0;
    for (size_t i = 0; i < camino.size() - 1; ++i) {
        coste += lector_ciudades.calcular_distancia(camino[i], camino[i + 1]);
    }
    coste += lector_ciudades.calcular_distancia(camino.back(), camino.front()); // Cerrar el ciclo

    if(coste < 0)
        throw std::bad_alloc();
}

double Individuo::get_coste() const {
    return coste;
}

int Individuo::get_ciudad(int index) const {
    return camino[index];
}

[[maybe_unused]] void Individuo::set_ciudad(int index, int ciudad) {
    camino[index] = ciudad;
}

bool Individuo::operator<(const Individuo &otro) const {
    return coste < otro.get_coste();
}

bool Individuo::operator>(const Individuo &otro) const {
    return coste > otro.get_coste();
}

bool Individuo::operator<=(const Individuo &otro) const {
    return coste <= otro.get_coste();
}

bool Individuo::operator>=(const Individuo &otro) const {
    return coste >= otro.get_coste();
}

bool Individuo::operator==(const Individuo &otro) const {
    return coste == otro.get_coste();
}

bool Individuo::operator!=(const Individuo &otro) const {
    return coste != otro.get_coste();
}

const std::vector<int> &Individuo::get_camino() const {
    return camino;
}

Individuo::~Individuo() = default;
