#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdexcept>

class LectorDatosCiudades {
private:
    std::string ruta;
    std::vector<std::vector<double>> ciudades;
    std::vector<std::vector<double>> distancias;

public:

    LectorDatosCiudades();

    explicit LectorDatosCiudades(const std::string &ruta);

    std::string getRuta() const {
        return ruta;
    }

    std::vector<std::vector<double>> getCiudades() const {
        return ciudades;
    }

    std::vector<std::vector<double>> getDistancias() const {
        return distancias;
    }
};