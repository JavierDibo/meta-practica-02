#ifndef META_PRACTICA_02_GLOBALS_H
#define META_PRACTICA_02_GLOBALS_H
#pragma once

#include <vector>
#include <string>
#include <limits>

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

#endif //META_PRACTICA_02_GLOBALS_H
