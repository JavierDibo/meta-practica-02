#include "Individuo.h"

Individuo::Individuo(std::vector<int> camino, LectorCiudades &lector_datos)  : camino(std::move(camino)),
                                                                               lector_datos(lector_datos) {
    calcular_coste_camino();
}

void Individuo::mutar()  {
    // Aplicar mutación con una probabilidad del 10%
    double dado_mutar = random.get_double(0.0, 1.0);

    if (dado_mutar < 0.1) {
        int posicion1 = random.get_int(0, static_cast<int>(camino.size()) - 1);
        int posicion2 = random.get_int(0, static_cast<int>(camino.size()) - 1);

        // Asegurarse de que las dos posiciones son diferentes
        while (posicion1 == posicion2) {
            posicion2 = random.get_int(0, static_cast<int>(camino.size()) - 1);
        }

        // Realizar el intercambio 2-opt
        intercambiar(posicion1, posicion2);
    }
}

void Individuo::intercambiar(int posicion1, int posicion2) {
    if (posicion1 >= 0 && posicion1 < camino.size() && posicion2 >= 0 && posicion2 < camino.size()) {
        std::swap(camino[posicion1], camino[posicion2]);
        calcular_coste_camino();
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

void Individuo::calcular_coste_camino() {
    coste = 0.0;
    for (size_t i = 0; i < camino.size() - 1; ++i) {
        coste += lector_datos.calcular_distancia(camino[i], camino[i + 1]);
    }
    coste += lector_datos.calcular_distancia(camino.back(), camino.front()); // Cerrar el ciclo
}

double Individuo::get_coste() const {
    return coste;
}

int Individuo::get_ciudad(int index) const {
    return camino[index];
}

void Individuo::set_ciudad(int index, int ciudad) {
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

Individuo::~Individuo() = default;
