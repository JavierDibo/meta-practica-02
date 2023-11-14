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

extern int MAX_NUM_EVALUACIONES;
extern int MAX_NUMERO_GENERACIONES;
extern int MAX_TIEMPO_EJECUCION;
extern int ALGORITMO;
extern int NUMERO_ELITES;
extern int NUMERO_INDIVIDUOS;
extern int KBEST;
extern int KWORST;

extern double PROBABILIDAD_CRUCE;
extern double PROBABILIDAD_MUTACION;
extern double PROBABILIDAD_GREEDY;

constexpr int INFINITO_POSITIVO = std::numeric_limits<int>::max();
constexpr int INFINITO_NEGATIVO = std::numeric_limits<int>::min();
constexpr int SEGUNDOS = 1;
constexpr int MILISEGUNDOS = 2;
constexpr int MICROSEGUNDOS = 3;

extern std::vector<int> SEMILLAS;
extern std::vector<std::string> VEC_ARCHIVOS_DATOS;
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
