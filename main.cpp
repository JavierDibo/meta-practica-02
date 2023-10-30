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

    LectorParametros lp(R"(.\parametros.txt)");

    string larga = "d15112.tsp";
    string corta = "ch130.tsp";

    LectorDatosCiudades lc(R"(C:\Users\jfdg0\Desktop\Metaheuristicas\Practica 2\Datos\)" + corta);

    return 0;
}
