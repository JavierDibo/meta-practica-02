#include <fstream>
#include <string>
#include <filesystem>
#include <iostream>
#include "Logger.h"
#include "globals.h"

std::string procesar_string_archivo_datos(std::string &archivo_datos) {
    size_t posInicio = archivo_datos.find_last_of('/') + 1;

    // Encontrar la posición del primer '.' después del último '/'
    size_t posFin = archivo_datos.find('.', posInicio);

    // Extraer el substring
    std::string resultado = archivo_datos.substr(posInicio, posFin - posInicio);

    // Convertir a mayúsculas
    std::transform(resultado.begin(), resultado.end(), resultado.begin(),
                   [](unsigned char c) { return std::toupper(c); });

    return resultado;
}

void Logger::ecribir_log_params() {

    archivo_params.open(params);

    if (!archivo_params) {
        std::cerr << "Error al abrir " << params << std::endl;
    }

    archivo_params << "Algoritmo " << algoritmo << " con operador " << operador << "\n";
    archivo_params << "------------------------------------\n\n";
    archivo_params << "Parametros usados\n";
    archivo_params << "Semilla: " << semilla << "\n";
    archivo_params << "Número de individuos: " << numero_individuos << "\n";
    archivo_params << "Máximas evaluaciones: " << max_evaluaciones << "\n";
    archivo_params << "Máximas generaciones: " << max_generaciones << "\n";
    archivo_params << "Máximos segundos de ejecución: " << max_segs_ejecucion << "\n";
    archivo_params << "Número de elites: " << num_elites << "\n";
    archivo_params << "Número de k-best: " << num_kbest << "\n";
    archivo_params << "Número de k-worst: " << num_kworst << "\n";
    archivo_params << "Probabilidad de cruce: " << p_cruce << "\n";
    archivo_params << "Probabilidad de mutación: " << p_mutacion << "\n";
    archivo_params << "Probabilidad greedy: " << p_greedy << "\n";

    archivo_params.close();
}

void Logger::inicializar_log_csv() {

    if (!archivo_csv) {
        std::cerr << "Error al abrir " << csv << std::endl;
    }

    archivo_csv << "Evaluacion,Generacion,Mejor coste,Peor coste,Solucion\n";

}

void Logger::escribir_log_csv(int evaluaciones_actual, int generaciones_actual, double mejor_coste_actual,
                              double peor_coste_actual, const std::vector<int> &mejor_solucion_actual) {

    archivo_csv << evaluaciones_actual << "," << generaciones_actual << "," << mejor_coste_actual << ","
                << peor_coste_actual << ",";

    archivo_csv << "[";
    for (const auto &valor: mejor_solucion_actual)
        archivo_csv << valor << " ";
    archivo_csv << "]\n";

    // archivo_csv.flush();

}

Logger::~Logger() {
    archivo_csv.close();
}

void Logger::inicializar_log() {

    if (ALGORITMO == 0) {
        algoritmo = "generacional";
        if (OPERADOR_CRUCE_GEN == 0) {
            operador = "OX2";
        } else {
            operador = "MOC";
        }

    } else {
        algoritmo = "diferencial";
        if (OPERADOR_DIFERENCIAL == 0) {
            operador = "EDA";
        } else {
            operador = "EDB";
        }
    }

    crear_archivos();

    archivo_csv.open(csv);
    if (!archivo_csv) {
        std::cerr << "Error al abrir " << csv << std::endl;
    }

    ecribir_log_params();

    inicializar_log_csv();
}

void Logger::crear_archivos() {

    /// logs/diferencial/eda/datos/
    /// semilla_numInd_maxEv_maxGen_maxSegs_numElit_numKbest_numKworst_cruce_mutacion_greedy.csv

    std::string archivo_datos = procesar_string_archivo_datos(VEC_ARCHIVOS_DATOS[0]);

    std::string identificador = semilla + "_ind" + numero_individuos + "_elit" + num_elites + "_kbest" + num_kbest;

    std::string directorio_base = "logs/" + archivo_datos + "/" + algoritmo + "_" + operador + "/";
    params = directorio_base + identificador + ".params";
    csv = directorio_base + identificador + ".csv";

    std::filesystem::create_directories(directorio_base);
}

Logger::Logger(int num, int kbest, int elite, int semilla) {

    if (MAX_NUM_EVALUACIONES != INFINITO_POSITIVO)
        max_evaluaciones = std::to_string(MAX_NUM_EVALUACIONES);
    else
        max_evaluaciones = "INF";
    if (MAX_NUMERO_GENERACIONES != INFINITO_POSITIVO)
        max_generaciones = std::to_string(MAX_NUMERO_GENERACIONES);
    else
        max_generaciones = "INF";
    max_segs_ejecucion = std::to_string(MAX_TIEMPO_EJECUCION);
    num_kworst = std::to_string(KWORST);
    p_cruce = std::to_string(PROBABILIDAD_CRUCE);
    p_mutacion = std::to_string(PROBABILIDAD_MUTACION);
    p_greedy = std::to_string(PROBABILIDAD_GREEDY);
    this->semilla = std::to_string(semilla);
    numero_individuos = std::to_string(num);
    num_elites = std::to_string(elite);
    num_kbest = std::to_string(kbest);

    inicializar_log();
}
