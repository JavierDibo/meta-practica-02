#ifndef META_PRACTICA_02_LECTORPARAMETROS_H
#define META_PRACTICA_02_LECTORPARAMETROS_H
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "globals.h"

using std::string;

class LectorParametros {

private:

    std::map<string,string> mapa_parametros;

    string nombre_archivo_parametros;

public:

    explicit LectorParametros(string nombre_archivo);

    virtual ~LectorParametros();

    void leer_parametros();

    const std::map<string, string> &getMapaParametros() const;

    const string &getNombreArchivoParametros() const;

};


#endif //META_PRACTICA_02_LECTORPARAMETROS_H
