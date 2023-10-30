#ifndef META_PRACTICA_02_RELOJ_H
#define META_PRACTICA_02_RELOJ_H

#include <string>
#include <chrono>

class Reloj {

private:
    std::chrono::system_clock::time_point tiempo_inicio;
    std::chrono::system_clock::time_point tiempo_final;
    double tiempo_transcurrido = 0.0;

public:
    void iniciar();

    void finalizar();

    double obtener_tiempo_transcurrido() const;

    Reloj();

    virtual ~Reloj();

};


#endif //META_PRACTICA_02_RELOJ_H