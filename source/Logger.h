#ifndef META_PRACTICA_02_LOGGER_H
#define META_PRACTICA_02_LOGGER_H

#include <vector>
#include "Poblacion.h"

class Logger {

    std::ofstream archivo_csv;
    std::ofstream archivo_params;
    std::string csv;
    std::string params;
    std::string p_cruce;
    std::string p_mutacion;
    std::string p_greedy;
    std::string semilla;
    std::string numero_individuos;
    std::string max_evaluaciones;
    std::string max_generaciones;
    std::string max_segs_ejecucion;
    std::string num_elites;
    std::string num_kbest;
    std::string num_kworst;
    std::string algoritmo;
    std::string operador;

public:
    explicit Logger(int num, int kbest, int elite, int semilla);

    virtual ~Logger();

    void escribir_log_csv(int evaluaciones_actual, int generaciones_actual, double mejor_coste_actual, double peor_coste_actual,
                          const std::vector<int> &mejor_solucion_actual);

    void inicializar_log();

    void ecribir_log_params();

    void inicializar_log_csv();

    void crear_archivos();
};


#endif //META_PRACTICA_02_LOGGER_H
