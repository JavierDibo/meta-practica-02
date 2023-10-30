#include "Reloj.h"
#include "globals.h"

Reloj::Reloj() {}

Reloj::~Reloj() {

}

void Reloj::iniciar() {
    tiempo_inicio = std::chrono::high_resolution_clock::now();
}

void Reloj::finalizar() {

    tiempo_final = std::chrono::high_resolution_clock::now();

    tiempo_transcurrido = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(
            tiempo_final - tiempo_inicio).count();
}

double Reloj::obtener_tiempo_transcurrido() const {
    return tiempo_transcurrido;
}


