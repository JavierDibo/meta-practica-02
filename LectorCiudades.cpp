#include "LectorCiudades.h"

void mostrar_tiempo_transcurrido(const std::string &nombre_archivo, Reloj &reloj_lector_datos) {

    if (ECHO) {
        std::cout << "\n----------------------------------------------------------------------" << std::endl;
        std::cout << "| Tiempo en procesar el archivo " << nombre_archivo << ": "
                  << reloj_lector_datos.obtener_tiempo_transcurrido(MILISEGUNDOS) << " milisegundos. |" << std::endl;
        std::cout << "----------------------------------------------------------------------\n" << std::endl;
    }
}

LectorCiudades::LectorCiudades(const std::string &ruta) {

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
    num_ciudades = std::stoi(linea.substr(linea.find(':') + 1));
    ciudades.resize(num_ciudades, std::vector<double>(2));
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

    distancias.resize(num_ciudades, std::vector<double>(num_ciudades));

    // Calcular las distancias entre las ciudades
    for (int i = 0; i < num_ciudades; ++i) {
#pragma omp parallel for default(none) shared(num_ciudades, i, INFINITO_POSITIVO) if (PARALELIZACION)
        for (int j = i; j < num_ciudades; ++j) {
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
LectorCiudades::son_iguales(const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B,
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

double LectorCiudades::calcular_distancia(int ciudad_a, int ciudad_b) const {
    if (ciudad_a < 0 || ciudad_a >= ciudades.size() || ciudad_b < 0 || ciudad_b >= ciudades.size()) {
        throw std::out_of_range("LectorCiudades::calcular_distancia::Ciudad fuera de rango");
    }

    return distancias[ciudad_a][ciudad_b];
}

int LectorCiudades::get_num_ciudades() const {
    return num_ciudades;
}

const std::vector<std::vector<double>> &LectorCiudades::get_ciudades() const {
    return ciudades;
}

const std::vector<std::vector<double>> &LectorCiudades::get_distancias() const {
    return distancias;
}

LectorCiudades::LectorCiudades() = default;
