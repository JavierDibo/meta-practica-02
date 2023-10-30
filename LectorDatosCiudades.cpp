#include <chrono>
#include <iomanip>
#include "LectorDatosCiudades.h"
#include "omp.h"
#include "globals.h"

void mostrar_tiempo_transcurrido(const std::string &nombre_archivo,
                                 const std::chrono::duration<double, std::milli> &tiempo_transcurrido) {
    auto tiempo_final = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
            tiempo_transcurrido).count();

    if (ECHO) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Tiempo en procesar el archivo " << nombre_archivo << ": " << tiempo_final << " milisegundos."
                  << std::endl;
    }
}

LectorDatosCiudades::LectorDatosCiudades(const std::string &ruta) {

    auto tiempo_inicio = std::chrono::high_resolution_clock::now();

    // Extraer nombre del archivo
    std::string nombre_archivo;
    size_t pos = ruta.find_last_of('\\');
    if (pos != std::string::npos) {
        nombre_archivo = ruta.substr(pos + 1);
    } else {
        nombre_archivo = ruta;
    }

    // Abrir archivo
    this->ruta = ruta.substr(0, ruta.find('.'));
    std::ifstream archivo_datos(ruta);
    if (!archivo_datos.is_open()) {
        throw std::runtime_error("File not found");
    }

    // Obtener dimension
    std::string linea;
    while (getline(archivo_datos, linea)) {
        if (linea.find("DIMENSION") != std::string::npos) {
            break;
        }
    }

    // Leer el tamaño y las ciudades
    int tam = std::stoi(linea.substr(linea.find(':') + 1));
    ciudades.resize(tam, std::vector<double>(2));
    while (getline(archivo_datos, linea)) {
        if (linea.find("EOF") != std::string::npos) {
            break;
        }
        if (linea.empty() || linea[0] < '0' || linea[0] > '9') {
            continue;
        }
        std::istringstream iss(linea);
        int index;
        iss >> index;
        --index;
        iss >> ciudades[index][0] >> ciudades[index][1];
    }

    distancias.resize(tam, std::vector<double>(tam));

    // Calcular las distancias entre las ciudades
#pragma omp parallel for default(none) shared(tam)
    for (int i = 0; i < tam; ++i) {
        for (int j = i; j < tam; ++j) {
            if (i == j) {
                distancias[i][j] = std::numeric_limits<double>::infinity();
            } else {
                distancias[i][j] = distancias[j][i] = std::sqrt(std::pow(ciudades[i][0] - ciudades[j][0], 2) +
                                                                std::pow(ciudades[i][1] - ciudades[j][1], 2));
            }
        }
    }

    auto tiempo_fin = std::chrono::high_resolution_clock::now();
    mostrar_tiempo_transcurrido(nombre_archivo, tiempo_fin - tiempo_inicio);
}

LectorDatosCiudades::LectorDatosCiudades() = default;
