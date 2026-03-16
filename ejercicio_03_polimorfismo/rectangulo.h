#ifndef RECTANGULO_H
#define RECTANGULO_H

#include "figura.h"

class Rectangulo : public Figura {

private:
    double base;
    double altura;

public:
    Rectangulo(std::string nombre, double base, double altura);

    double calcularArea() override;
    void describir() override;

    double obtenerBase();
    double obtenerAltura();
};

#endif