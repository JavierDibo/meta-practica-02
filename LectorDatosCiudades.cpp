#include <iomanip>
#include "LectorDatosCiudades.h"
#include "omp.h"
#include "globals.h"
#include "Reloj.h"

void mostrar_tiempo_transcurrido(const std::string &nombre_archivo, const Reloj &reloj_lector_datos) {

    if (ECHO) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Tiempo en procesar el archivo " << nombre_archivo << ": "
                  << reloj_lector_datos.obtener_tiempo_transcurrido() << " milisegundos." << std::endl;
    }
}

LectorDatosCiudades::LectorDatosCiudades(const std::string &ruta) {

    Reloj reloj_lector_datos;

    reloj_lector_datos.iniciar();

    // Extraer nombre del archivo
    std::string nombre_archivo;
    size_t pos = ruta.find_last_of('\\');
    if (pos != std::string::npos) {
        nombre_archivo = ruta.substr(pos + 1);
    } else {
        nombre_archivo = ruta;
    }

    // Abrir archivo
    this->ruta_archivo_datos = ruta.substr(0, ruta.find('.'));
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
    for (int i = 0; i < tam; ++i) {
#pragma omp parallel for default(none) shared(tam, i, INFINITO_POSITIVO) if (PARALELIZACION)
        for (int j = i; j < tam; ++j) {
            if (i == j) {
                distancias[i][j] = INFINITO_POSITIVO;
            } else {
                double dx = ciudades[i][0] - ciudades[j][0];
                double dy = ciudades[i][1] - ciudades[j][1];
                double distancia = std::hypot(dx, dy);
                distancias[i][j] = distancias[j][i] = distancia;
            }
        }
    }

    reloj_lector_datos.finalizar();
    mostrar_tiempo_transcurrido(nombre_archivo, reloj_lector_datos);
}

bool
LectorDatosCiudades::sonIguales(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B,
                                double tolerancia) {
    if (A.size() != B.size()) return false;

    for (size_t i = 0; i < A.size(); ++i) {
        if (A[i].size() != B[i].size()) return false;

        for (size_t j = 0; j < A[i].size(); ++j) {
            if (std::fabs(A[i][j] - B[i][j]) > tolerancia) {
                return false;
            }
        }
    }

    return true;
}

LectorDatosCiudades::LectorDatosCiudades() = default;
