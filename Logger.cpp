#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "Logger.h"
#include "globals.h"

std::ofstream
inicializar_log_primero_mejor(int semilla, const std::string &nombre_archivo, const std::string &algoritmo) {

    std::filesystem::path path(nombre_archivo);
    std::string nombre = path.stem().string();

    std::string nombre_log =
            "logs/" + algoritmo + "/" + nombre + "_" + algoritmo + "_" + std::to_string(semilla) + ".csv";

    std::filesystem::path directorio = std::filesystem::path(nombre_log).parent_path();
    if (!std::filesystem::exists(directorio)) {
        std::filesystem::create_directories(directorio);
    }

    std::ofstream archivo_log(nombre_log);

    if (!archivo_log.is_open()) {
        std::cerr << "inicializar_log::no se pudo abrir el archivo " + nombre_log << std::endl;
        throw;
    }

    if (archivo_log.is_open()) {
        archivo_log << "Iteracion,Movimiento_i,Movimiento_j,Coste solucion,Delta,Max_Iteraciones,Solucion\n";
    }

    return archivo_log;
}

void escribir_log_primero_mejor(std::ofstream &archivo_log, int iteraciones, int coste, int delta,
                                const std::vector<int> &solucion) {

    archivo_log << iteraciones << "," << coste << "," << delta
                << "," << MAX_ITERACIONES << ",";

    for (const auto dato: solucion) {
        archivo_log << dato << " ";
    }

    archivo_log << "\n";
}

Logger::Logger(int idAlgoritmo) : id_algoritmo(idAlgoritmo) {

}

Logger::~Logger() {

}
