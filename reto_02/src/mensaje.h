#ifndef MENSAJE_H
#define MENSAJE_H
#include <string>
#include <ctime>

class Mensaje {

    public:
    std::string contenido;
    std::string remitente;
    std::time_t FechaHora;

    Mensaje(const std::string& texto, const std::string& autor);
    
};

#endif