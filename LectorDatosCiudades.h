#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <stdexcept>

class LectorDatosCiudades {
private:
    std::string ruta_archivo_datos;
    std::vector<std::vector<double>> ciudades;
    std::vector<std::vector<double>> distancias;

public:

    LectorDatosCiudades();

    explicit LectorDatosCiudades(const std::string &ruta);

    static bool sonIguales(const std::vector<std::vector<double>>& A, const std::vector<std::vector<double>>& B, double tolerancia = 1e-9);

    [[maybe_unused]] [[nodiscard]] std::string getRutaArchivoDatos() const {
        return ruta_archivo_datos;
    }

    [[maybe_unused]] [[nodiscard]] std::vector<std::vector<double>> getCiudades() const {
        return ciudades;
    }

    [[maybe_unused]] [[nodiscard]] std::vector<std::vector<double>> getDistancias() const {
        return distancias;
    }
};