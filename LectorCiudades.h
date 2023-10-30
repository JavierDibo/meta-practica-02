#ifndef META_PRACTICA_02_LECTOR_DATOS_CUIDADES_H
#define META_PRACTICA_02_LECTOR_DATOS_CUIDADES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdexcept>

class LectorCiudades {
private:
    std::string ruta_archivo_datos;
    std::vector<std::vector<double>> ciudades;
    std::vector<std::vector<double>> distancias;
    int num_ciudades{};
public:


public:

    LectorCiudades();

    explicit LectorCiudades(const std::string &ruta);

    static bool son_iguales(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B,
                            double tolerancia = 1e-9);

    std::string get_ruta_archivo_datos() const {
        return ruta_archivo_datos;
    }

    std::vector<std::vector<double>> get_matriz_ciudades() const {
        return ciudades;
    }

    std::vector<std::vector<double>> get_matriz_distancias() const {
        return distancias;
    }

    double calcular_distancia(int ciudad_a, int ciudad_b) const;

    int get_num_ciudades() const;
};

#endif //META_PRACTICA_02_LECTOR_DATOS_CUIDADES_H