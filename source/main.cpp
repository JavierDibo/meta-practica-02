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

const std::string YELLOW = "\033[33m";  // Yellow
const std::string BLUE = "\033[34m";    // Blue
const std::string RESET = "\033[0m";    // Reset to default color
const std::string CYAN = "\033[36m";  // Cyan


struct ResultadoT {
    double coste = INFINITO_POSITIVO;
    int semilla = 0;
    int num_ind = 0;
    int kbest = 0;
    int num_elites = 0;
};

ResultadoT mejor_resultado{};

/// Funciones ----------------------------------------------------------------------------------------------------------

void imprimir_informacion_actual(Reloj &reloj_actual, ResultadoT resultado_actual, std::vector<Individuo> &poblacion,
                                 const string &archivo) {

    double coste = poblacion.begin()->get_coste();
    if (coste < mejor_resultado.coste) {
        mejor_resultado.coste = coste;
        mejor_resultado.kbest = resultado_actual.kbest;
        mejor_resultado.num_ind = resultado_actual.num_ind;
        mejor_resultado.semilla = resultado_actual.semilla;
        mejor_resultado.num_elites = resultado_actual.num_elites;
    }

    if (!ECHO)
        return;

    cout << BLUE;
    cout << "\nSemilla: " << resultado_actual.semilla << endl;
    cout << "Archivo datos: " << archivo << endl;
    cout << "Numero de individuos: " << resultado_actual.num_ind << endl;
    cout << "Kbest: " << resultado_actual.kbest << endl;
    cout << "Num elites: " << resultado_actual.num_elites << std::endl;
    cout << RESET;
    cout << "Tiempo de ejecucion: " << reloj_actual.obtener_tiempo_transcurrido(MILISEGUNDOS) << " milisegundos."
         << endl;
    cout << YELLOW;
    cout << "Coste del mejor individuo: " << coste << endl;
    cout << RESET;
    cout << "Coste del peor individuo: " << poblacion.at(poblacion.size() - 1).get_coste() << endl;
    cout << "Numero de generaciones: " << NUM_GENERACIONES_SEMILLA << endl;
    cout << "Numero de evaluaciones: " << NUM_EVALUACIONES_SEMILLA << endl;
    cout << "---------------------------------------------------------------------" << endl;
}

void imprimir_informacion_global(Reloj reloj) {

    cout << "\nRESUMEN GLOBAL\n";

    /// Semillas
    cout << "Semilla: ";
    for (const auto &semilla: VEC_SEMILLAS) {
        cout << semilla << " ";
    }
    cout << "\n";

    /// Archivos
    cout << "Archivo de datos: ";
    for (const auto &archivo: VEC_ARCHIVOS_DATOS) {
        cout << archivo << " ";
    }
    cout << "\n";

    /// Kbest
    cout << "KBest: ";
    for (const auto &kbest: VEC_KBEST) {
        cout << kbest << " ";
    }
    cout << "\n";

    /// Numeros de individuos
    cout << "Num individuos: ";
    for (const auto &num_individuos: VEC_NUM_INDIVIDUOS) {
        cout << num_individuos << " ";
    }
    cout << "\n";

    /// Numeros de elites
    cout << "Num elites: ";
    for (const auto &elites: VEC_NUMERO_ELITES) {
        cout << elites << " ";
    }
    cout << "\n";

    if (VEC_ARCHIVOS_DATOS.size() < 2) {
        cout << CYAN;
        cout << "Mejor coste: " << mejor_resultado.coste << std::endl;
        cout << "Semilla: " << mejor_resultado.semilla << std::endl;
        cout << "Numero de individuos: " << mejor_resultado.num_ind << std::endl;
        cout << "Num kbest: " << mejor_resultado.kbest << std::endl;
        cout << "Num elites: " << mejor_resultado.num_elites << std::endl;
        cout << RESET;
    }
    cout << "Tiempo de ejecucion total: " << reloj.obtener_tiempo_transcurrido(MILISEGUNDOS) << " milisegundos\n";
    cout << (MAX_NUMERO_GENERACIONES < MAX_NUM_EVALUACIONES ? "Numero de generaciones maximo: "
                                                            : "Numero de evaluaciones maximo: ")
         << (MAX_NUMERO_GENERACIONES < MAX_NUM_EVALUACIONES ? MAX_NUMERO_GENERACIONES : MAX_NUM_EVALUACIONES) << "\n";

    cout << "Probabilidad de cruce: " << PROBABILIDAD_CRUCE * 100 << "%\n";
    cout << "Probabilidad de mutacion: " << PROBABILIDAD_MUTACION * 100 << "%\n";
    cout << "Porcentaje generado por greedy: " << PROBABILIDAD_GREEDY * 100 << "%\n";

}

void lanzar_evolucion(Poblacion &poblacion) {

    if (ECHO) {
        std::cout << std::endl;
        std::cout << "Lanzando evolucion de la poblacion..." << std::endl;
    }

    if (ALGORITMO == 0)
        poblacion.evolucion_generacional();
    if (ALGORITMO == 1)
        poblacion.evolucion_diferencial();
}

void ejecutar( LectorCiudades lector_ciudades, const string& archivo_datos) {
    for (const int &semilla: VEC_SEMILLAS) {

        inicializar_generador_aleatorio(semilla);

        for (const int &num: VEC_NUM_INDIVIDUOS) {

            for (const int &elite: VEC_NUMERO_ELITES) {

                for (const int &kbest: VEC_KBEST) {

                    Reloj reloj_iteracion;
                    reloj_iteracion.iniciar();

                    Poblacion poblacion(lector_ciudades, num, kbest, elite);

                    lanzar_evolucion(poblacion);

                    vector<Individuo> poblacion_final = poblacion.get_individuos();
                    std::sort(poblacion_final.begin(), poblacion_final.end());

                    reloj_iteracion.finalizar();

                    imprimir_informacion_actual(reloj_iteracion, {INFINITO_POSITIVO, semilla, num, kbest, elite},
                                                poblacion_final, archivo_datos);
                }
            }
        }
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

    string archivo_datos = VEC_ARCHIVOS_DATOS[0];

    LectorCiudades lector_ciudades(archivo_datos);

    ejecutar(lector_ciudades, archivo_datos);

    reloj_principal.finalizar();

    imprimir_informacion_global(reloj_principal);

    return 0;
}
