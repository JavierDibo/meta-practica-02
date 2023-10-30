#ifndef META_PRACTICA_02_LECTORPARAMETROS_H
#define META_PRACTICA_02_LECTORPARAMETROS_H
#include <string>
#include <map>

using std::string;

class LectorParametros {

private:

    std::map<string,string> mapa_parametros;

    string nombre_archivo_parametros;

public:

    [[maybe_unused]] explicit LectorParametros(string nombre_archivo);

    virtual ~LectorParametros();

    void leer_parametros();

    [[maybe_unused]] [[nodiscard]] const std::map<string, string> &getMapaParametros() const;

    [[maybe_unused]] [[nodiscard]] const string &getNombreArchivoParametros() const;

};


#endif //META_PRACTICA_02_LECTORPARAMETROS_H
