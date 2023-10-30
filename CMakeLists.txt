cmake_minimum_required(VERSION 3.26)
project(meta_practica_02)

set(CMAKE_CXX_STANDARD 17)

add_executable(meta_practica_02 main.cpp
        LectorCiudades.cpp
        LectorCiudades.h
        LectorCiudades.h
        globals.h
        globals.cpp
        Reloj.cpp
        Reloj.h
        LectorParametros.cpp
        LectorParametros.h
        Logger.cpp
        Logger.h
        Individuo.cpp
        Individuo.h
        Poblacion.cpp
        Poblacion.h
        AlgoritmoGenetico.cpp
        AlgoritmoGenetico.h
        Poblacion.cpp
        GeneradorAleatorio.cpp
        GeneradorAleatorio.h
)

find_package(OpenMP)
if (OPENMP_FOUND)
    target_link_libraries(meta_practica_02 PUBLIC OpenMP::OpenMP_CXX)
endif ()
