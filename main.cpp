#include <iostream>
#include <string>
#include <map>
#include "LectorDatosCiudades.h"
#include "globals.h"

/// Typedefs -----------------------------------------------------------------------------------------------------------

using std::string;
using std::map;
using std::vector;

/// Funciones ----------------------------------------------------------------------------------------------------------

map<string, string> lectura_parametros();

int main() {

    string larga = "d15112.tsp";
    string corta = "ch130.tsp";

    LectorDatosCiudades lc(R"(C:\Users\jfdg0\Desktop\Metaheuristicas\Practica 2\Datos\)" + larga);

    return 0;
}

std::map<string, string> lectura_parametros() {

    string nombre_archivo = "./parametros.txt";

    std::ifstream archivo_parametros(nombre_archivo);
    if (!archivo_parametros.is_open()) {
        std::cerr << "lectura_parametros::No se pudo abrir el archivo " << nombre_archivo << std::endl;
        return {};
    }

    map<string, string> parametros;

    string line;
    while (std::getline(archivo_parametros, line)) {
        if (!line.empty() && (line[0] == '#' || (line.size() >= 2 && line[0] == '/' && line[1] == '/'))) {
            continue;
        }

        size_t pos = line.find('=');
        if (pos != string::npos) {
            string key = line.substr(0, pos);
            string value = line.substr(pos + 1);
            key.erase(0, key.find_first_not_of(' '));
            key.erase(key.find_last_not_of(' ') + 1);
            value.erase(0, value.find_first_not_of(' '));
            value.erase(value.find_last_not_of(' ') + 1);
            parametros[key] = value;
        }
    }

    archivo_parametros.close();

    if (parametros.find("semilla") != parametros.end()) {
        string seeds_string = parametros["semilla"];
        std::stringstream ss(seeds_string);
        string seed;
        while (std::getline(ss, seed, ',')) {
            SEMILLAS.push_back(std::stoi(seed));
        }
    }

    if (parametros.find("echo") != parametros.end()) {
        ECHO = (parametros["echo"] == "true");
    }

    if (parametros.find("log") != parametros.end()) {
        LOG = (parametros["log"] == "true");
    }

    if (parametros.find("max_iteraciones") != parametros.end()) {
        MAX_ITERACIONES = std::stoi(parametros["max_iteraciones"]);
    }

    if (parametros.find("datos") != parametros.end()) {
        string archivos_string = parametros["datos"];
        std::stringstream ss(archivos_string);
        string archivo;
        while (getline(ss, archivo, ',')) {
            ARCHIVOS_DATOS.push_back("datos/" + archivo);
        }
    }

    if (parametros.find("pragma") != parametros.end()) {
        PARALELIZACION = (parametros["pragma"] == "true");
    }

    if (parametros.find("statistics") != parametros.end()) {
        STATS = (parametros["statistics"] == "true");
    }

    return parametros;
}

