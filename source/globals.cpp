#include "globals.h"

bool ECHO;
bool LOG;
bool PARALELIZACION;
bool STATS;

int MAX_NUM_EVALUACIONES;
int MAX_NUMERO_GENERACIONES;
int ALGORITMO;
int NUMERO_ELITES;
int KWORST;
int SEMILLA;
double MAX_TIEMPO_EJECUCION;
double PROBABILIDAD_CRUCE;
double PROBABILIDAD_MUTACION;
double PROBABILIDAD_GREEDY;

int NUM_EVALUACIONES_SEMILLA;
int NUM_GENERACIONES_SEMILLA;

std::vector<int> VEC_SEMILLAS;
std::vector<int> VEC_KBEST;
std::vector<int> VEC_NUM_INDIVIDUOS;
std::vector<std::string> VEC_ARCHIVOS_DATOS;

std::string ARCHIVO_DATOS;

static std::mt19937 generator;

void inicializar_generador_aleatorio(int semilla) {
    SEMILLA = semilla;
    generator = std::mt19937(SEMILLA);
}

int get_rand_int(const int &min, const int &max) {
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

double get_rand_double(const double &min, const double &max) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

std::vector<int> vector_aleatorio(int tamanno) {
    std::vector<int> vec(tamanno);
    std::iota(vec.begin(), vec.end(), 0); // Generar permutacion identidad
    std::shuffle(vec.begin(), vec.end(), generator);

    return vec;
}