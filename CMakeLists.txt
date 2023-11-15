cmake_minimum_required(VERSION 3.26)
project(meta_practica_02)

set(CMAKE_CXX_STANDARD 17)

add_executable(meta_practica_02 source/main.cpp
        source/LectorCiudades.cpp
        source/LectorCiudades.h
        source/LectorCiudades.h
        source/globals.h
        source/globals.cpp
        source/Reloj.cpp
        source/Reloj.h
        source/LectorParametros.cpp
        source/LectorParametros.h
        source/Logger.cpp
        source/Logger.h
        source/Individuo.cpp
        source/Individuo.h
        source/Poblacion.cpp
        source/Poblacion.h
        source/Poblacion.cpp
)

find_package(OpenMP)
if (OPENMP_FOUND)
    target_link_libraries(meta_practica_02 PUBLIC OpenMP::OpenMP_CXX)
endif ()
