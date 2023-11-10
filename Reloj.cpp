#include "Reloj.h"

Reloj::Reloj() = default;

Reloj::~Reloj() = default;

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
            return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                    tiempo_final - tiempo_inicio).count());
        case MICROSEGUNDOS:
            return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
                    tiempo_final - tiempo_inicio).count());
        default:
            return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                    tiempo_final - tiempo_inicio).count());
    }
}


