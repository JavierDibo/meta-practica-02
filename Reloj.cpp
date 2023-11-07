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
}

double Reloj::obtener_tiempo_transcurrido(int time_frame) {
    switch (time_frame) {
        case SEGUNDOS:
            return std::chrono::duration_cast<std::chrono::duration<double>>(
                    tiempo_final - tiempo_inicio).count();
        case MILISEGUNDOS:
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                    tiempo_final - tiempo_inicio).count();
        case MICROSEGUNDOS:
            return std::chrono::duration_cast<std::chrono::microseconds>(
                    tiempo_final - tiempo_inicio).count();
        default:
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                    tiempo_final - tiempo_inicio).count();
    }
}


