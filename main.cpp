#include <iostream>
#include <string>
#include <map>
#include "LectorCiudades.h"
#include "LectorParametros.h"
#include "globals.h"
#include "Poblacion.h"

/// Typedefs -----------------------------------------------------------------------------------------------------------

using std::string;
using std::map;
using std::vector;

/// Funciones ----------------------------------------------------------------------------------------------------------

int main() {

    LectorParametros lector_parametros(R"(.\parametros.txt)");

    inicializar_generador_aleatorio(SEMILLAS[0]);

    LectorCiudades lector_ciudades(ARCHIVO_DATOS);

    int num_individuos = 20;
    Poblacion poblacion(num_individuos, lector_ciudades);

    poblacion.evolucion_generacional();

    return 0;
}
