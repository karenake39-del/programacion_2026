#ifndef CONVERSACION_H
#define CONVERSACION_H

#include <vector>
#include <string>
#include "mensaje.h"

class Conversacion {
private:
    std::vector<Mensaje> mensajes; 

public:

    void agregarMensaje(const Mensaje& msg);

    void guardarEnArchivo(const std::string& ruta) const;
};

#endif