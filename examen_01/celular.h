#ifndef CELULAR_H
#define CELULAR_H

#include <string>

class Celular {
private:
    std::string modelo;
    std::string pin;
    std::string marca;
    bool encendido;
    double precio;

public:

    Celular(std::string modelo, std::string pin, std::string marca, bool encendido, double precio);

    void encenderApagar();
    bool verificarPin(std::string pinIngresado);
    void mostrarInformacion();
};

#endif