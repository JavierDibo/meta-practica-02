#ifndef META_PRACTICA_02_GLOBALS_H
#define META_PRACTICA_02_GLOBALS_H
#pragma once

#include <vector>
#include <string>
#include <limits>
#include <random>
#include <algorithm>
#include "GeneradorAleatorio.h"

/// Variables globales --------------------------------------------------------------------------------------------------

extern bool ECHO;
extern bool LOG;
extern bool PARALELIZACION;
extern bool STATS;

extern int MAX_ITERACIONES;
extern int ALGORITMO;

extern const int INFINITO_POSITIVO;
extern const int INFINITO_NEGATIVO;

extern std::vector<int> SEMILLAS;
extern std::vector<std::string> ARCHIVOS_DATOS;
extern std::string ARCHIVO_DATOS;

extern GeneradorAleatorio random;

void inicializar_generador_aleatorio(unsigned int semilla);

inline std::vector<int> vector_aleatorio(int tamanno) {
    std::vector<int> vec(tamanno);
    for (int i = 0; i < tamanno; i++) {
        vec[i] = i;
    }
    std::shuffle(vec.begin(), vec.end(), random.get_motor());

    return vec;
}

#endif //META_PRACTICA_02_GLOBALS_H
