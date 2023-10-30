#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include "LectorParametros.h"
#include "globals.h"

void LectorParametros::leer_parametros() {

    std::ifstream archivo_parametros(nombre_archivo_parametros);
    if (!archivo_parametros.is_open()) {
        throw std::runtime_error("leer_parametros::No se pudo abrir el archivo " + nombre_archivo_parametros);
    }

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
            mapa_parametros[key] = value;
        }
    }

    archivo_parametros.close();

    if (mapa_parametros.find("semilla") != mapa_parametros.end()) {
        string seeds_string = mapa_parametros["semilla"];
        std::stringstream ss(seeds_string);
        string seed;
        while (std::getline(ss, seed, ',')) {
            SEMILLAS.push_back(std::stoi(seed));
        }
    }

    if (SEMILLAS.empty()) {
        SEMILLAS.push_back(12345678);
        SEMILLAS.push_back(23456781);
        SEMILLAS.push_back(34567812);
    }

    if (mapa_parametros.find("echo") != mapa_parametros.end()) {
        ECHO = (mapa_parametros["echo"] == "true");
    }

    if (mapa_parametros.find("log") != mapa_parametros.end()) {
        LOG = (mapa_parametros["log"] == "true");
    }

    if (mapa_parametros.find("max_iteraciones") != mapa_parametros.end()) {
        MAX_ITERACIONES = std::stoi(mapa_parametros["max_iteraciones"]);
    }

    if (mapa_parametros.find("datos") != mapa_parametros.end()) {
        string archivos_string = mapa_parametros["datos"];
        std::stringstream ss(archivos_string);
        string archivo;
        while (getline(ss, archivo, ',')) {
            ARCHIVOS_DATOS.push_back("datos/" + archivo);
        }
    }

    if (mapa_parametros.find("paralelizacion") != mapa_parametros.end()) {
        PARALELIZACION = (mapa_parametros["paralelizacion"] == "true");
    }

    if (mapa_parametros.find("statistics") != mapa_parametros.end()) {
        STATS = (mapa_parametros["statistics"] == "true");
    }
}

[[maybe_unused]] LectorParametros::LectorParametros(string nombre_archivo) {
    this->nombre_archivo_parametros = std::move(nombre_archivo);

    leer_parametros();
}

LectorParametros::~LectorParametros() = default;

[[maybe_unused]] const std::map<string, string> &LectorParametros::getMapaParametros() const {
    return mapa_parametros;
}

[[maybe_unused]] const string &LectorParametros::getNombreArchivoParametros() const {
    return nombre_archivo_parametros;
}