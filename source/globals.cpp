#include "globals.h"

bool ECHO;
bool LOG;
bool PARALELIZACION;
bool STATS;

int MAX_NUM_EVALUACIONES;
int MAX_NUMERO_GENERACIONES;
int ALGORITMO;
int NUMERO_ELITES;
int NUMERO_INDIVIDUOS;
int KBEST;
int KWORST;
double PROBABILIDAD_CRUCE;
double PROBABILIDAD_MUTACION;
double PROBABILIDAD_GREEDY;

std::vector<int> SEMILLAS;
std::vector<std::string> ARCHIVOS_DATOS;

std::string ARCHIVO_DATOS;

GeneradorAleatorio random;

void inicializar_generador_aleatorio(unsigned int semilla) {
    random = GeneradorAleatorio(semilla);
}