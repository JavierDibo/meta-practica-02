#include <iostream>
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
            return (static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                    tiempo_final - tiempo_inicio).count()))/1000;
        case MILISEGUNDOS:
            return (static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(
                    tiempo_final - tiempo_inicio).count()))/1000;
        case MICROSEGUNDOS:
            return (static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(
                    tiempo_final - tiempo_inicio).count()))/1000;
        default:
            std::cerr << "Reloj::obtener_tiempo_transcurrido::unidad no reconocida";
            return {};
    }
}


