#ifndef META_PRACTICA_02_GLOBALS_H
#define META_PRACTICA_02_GLOBALS_H
#pragma once

#include <vector>
#include <string>
#include <limits>
#include <random>
#include <algorithm>
#include <thread>

/// Variables globales --------------------------------------------------------------------------------------------------

extern bool ECHO;
extern bool LOG;
extern bool PARALELIZACION;
extern bool STATS;

extern int MAX_NUM_EVALUACIONES;
extern int MAX_NUMERO_GENERACIONES;
extern int ALGORITMO;
extern int NUMERO_ELITES;
extern int KWORST;
extern int SEMILLA;
extern double MEJOR_COSTE;

extern int NUM_EVALUACIONES_SEMILLA;
extern int NUM_GENERACIONES_SEMILLA;

extern double MAX_TIEMPO_EJECUCION;
extern double PROBABILIDAD_CRUCE;
extern double PROBABILIDAD_MUTACION;
extern double PROBABILIDAD_GREEDY;

constexpr int INFINITO_POSITIVO = std::numeric_limits<int>::max();
constexpr int INFINITO_NEGATIVO = std::numeric_limits<int>::min();
constexpr int SEGUNDOS = 1;
constexpr int MILISEGUNDOS = 2;
constexpr int MICROSEGUNDOS = 3;

extern std::vector<int> VEC_SEMILLAS;
extern std::vector<int> VEC_KBEST;
extern std::vector<int> VEC_NUM_INDIVIDUOS;
extern std::vector<std::string> VEC_ARCHIVOS_DATOS;
extern std::string ARCHIVO_DATOS;

void inicializar_generador_aleatorio(int semilla);

std::vector<int> vector_aleatorio(int tamanno);

int get_rand_int(const int &min, const int &max);

double get_rand_double(const double &min, const double &max);

#endif //META_PRACTICA_02_GLOBALS_H
