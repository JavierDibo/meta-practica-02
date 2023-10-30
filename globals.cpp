#include "globals.h"

bool ECHO;
bool LOG;
bool PARALELIZACION;
bool STATS;

int MAX_ITERACIONES;
int ALGORITMO;

std::vector<int> SEMILLAS;
std::vector<std::string> ARCHIVOS_DATOS;

const int INFINITO_POSITIVO = std::numeric_limits<int>::max();
const int INFINITO_NEGATIVO = std::numeric_limits<int>::min();

std::string ARCHIVO_DATOS;

GeneradorAleatorio random;

void inicializar_generador_aleatorio(unsigned int semilla) {
    random = GeneradorAleatorio(semilla);
}