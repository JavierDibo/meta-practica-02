#ifndef META_PRACTICA_02_RELOJ_H
#define META_PRACTICA_02_RELOJ_H

#include <string>
#include <chrono>

#include "globals.h"

class Reloj {

private:
    std::chrono::system_clock::time_point tiempo_inicio;
    std::chrono::system_clock::time_point tiempo_final;

public:
    void iniciar();

    void finalizar();

    double obtener_tiempo_transcurrido(int time_frame);

    Reloj();

    virtual ~Reloj();

};


#endif //META_PRACTICA_02_RELOJ_H
