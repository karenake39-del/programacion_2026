#include "mensaje.h"

Mensaje::Mensaje(const std::string& texto, const std::string& autor){
    contenido = texto;
    remitente = autor;
    FechaHora = std::time(nullptr);
}