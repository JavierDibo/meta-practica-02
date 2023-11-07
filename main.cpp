#include <iostream>
#include <string>
#include "LectorCiudades.h"
#include "LectorParametros.h"
#include "globals.h"
#include "Poblacion.h"

/// Defs -----------------------------------------------------------------------------------------------------------

using std::string;
using std::map;
using std::vector;

/// Funciones ----------------------------------------------------------------------------------------------------------

int main() {

    LectorParametros lector_parametros(R"(.\parametros.txt)");

    inicializar_generador_aleatorio(SEMILLAS[0]);

    LectorCiudades lector_ciudades(ARCHIVO_DATOS);

    Poblacion poblacion(lector_ciudades);

    poblacion.evolucionar();

    std::vector<Individuo> fin = poblacion.get_individuos();

    std::sort(fin.begin(), fin.end());

    return 0;
}
