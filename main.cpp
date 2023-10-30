#include <iostream>
#include <string>
#include <map>
#include "LectorDatosCiudades.h"
#include "LectorParametros.h"
#include "globals.h"

/// Typedefs -----------------------------------------------------------------------------------------------------------

using std::string;
using std::map;
using std::vector;

/// Funciones ----------------------------------------------------------------------------------------------------------

int main() {

    LectorParametros lector_parametros(R"(.\parametros.txt)");

    LectorDatosCiudades lector_ciudades(ARCHIVO_DATOS);

    return 0;
}
