#ifndef META_PRACTICA_02_GENERADORALEATORIO_H
#define META_PRACTICA_02_GENERADORALEATORIO_H

#include <random>
#include <vector>

class GeneradorAleatorio {
private:
    std::mt19937 motor;

public:
    explicit GeneradorAleatorio(unsigned int semilla) : motor(semilla) {}

    int get_int(int min, int max) {
        std::uniform_int_distribution<> distribucion(min, max);
        return distribucion(motor);
    }

    double get_double(double min, double max) {
        std::uniform_real_distribution<> distribucion(min, max);
        return distribucion(motor);
    }

    bool get_bool() {
        std::uniform_int_distribution<> distribucion(0, 1);
        return distribucion(motor) == 1;
    }

    template<typename T>
    T select_random_from(const std::vector<T> &lista) {
        std::uniform_int_distribution<> distribucion(0, lista.size() - 1);
        return lista[distribucion(motor)];
    }

    std::mt19937 &get_motor() {
        return motor;
    }

    GeneradorAleatorio() {}
};

#endif //META_PRACTICA_02_GENERADORALEATORIO_H
