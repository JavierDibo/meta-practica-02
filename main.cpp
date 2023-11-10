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
using std::cout;
using std::endl;

/// Funciones ----------------------------------------------------------------------------------------------------------

void imprimir_informacion_semilla(int semilla, Reloj &reloj, std::vector<Individuo> &poblacion) {

    if (ECHO) {
        cout << "\nSemilla: " << semilla << endl;
        cout << "Tiempo de ejecucion: " << reloj.obtener_tiempo_transcurrido(MILISEGUNDOS)
             << " milisegundos." << endl;
        cout << "Coste del mejor individuo: " << poblacion.begin()->get_coste() << endl;
        cout << "Coste del peor individuo: " << poblacion.at(poblacion.size() - 1).get_coste()
             << endl;
        cout << "---------------------------------------------------------------------" << endl;
    }
}

void imprimir_informacion_global(Reloj reloj) {

    if (ECHO) {
        cout << "\nTiempo de ejecucion total: " << reloj.obtener_tiempo_transcurrido(MILISEGUNDOS)
             << " milisegundos" << endl;
        cout << "Numero de generaciones: " << MAX_NUMERO_GENERACIONES << endl;
        cout << "Numero de elites: " << NUMERO_ELITES << endl;
        cout << "Numero de individuos: " << NUMERO_INDIVIDUOS << endl;
        cout << "KBest: " << KBEST << endl;
        cout << "KWorst: " << KWORST << endl;
        cout << "Probabilidad de cruce: " << PROBABILIDAD_CRUCE * 100 << "%" << endl;
        cout << "Probabilidad de mutacion: " << PROBABILIDAD_MUTACION * 100 << "%" << endl;
        cout << "Porcentaje generado por greedy: " << PROBABILIDAD_GREEDY * 100 << "%" << endl;
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <ruta archivo parametros>\n";
        return 1;
    }

    Reloj reloj_principal;

    reloj_principal.iniciar();

    std::string ruta_parametros = argv[1];

    LectorParametros lector_parametros(ruta_parametros);

    LectorCiudades lector_ciudades(ARCHIVO_DATOS);

    for (int semilla: SEMILLAS) {

        Reloj reloj_iteracion;
        reloj_iteracion.iniciar();

        inicializar_generador_aleatorio(semilla);

        Poblacion poblacion(lector_ciudades);
        poblacion.evolucionar();

        vector<Individuo> poblacion_final = poblacion.get_individuos();
        std::sort(poblacion_final.begin(), poblacion_final.end());

        reloj_iteracion.finalizar();

        imprimir_informacion_semilla(semilla, reloj_iteracion, poblacion_final);
    }

    reloj_principal.finalizar();

    imprimir_informacion_global(reloj_principal);

    return 0;
}
