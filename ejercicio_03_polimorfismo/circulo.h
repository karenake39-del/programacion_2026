#ifndef CIRCULO_H
#define CIRCULO_H

#include "figura.h"
#include <string>

class Circulo : public Figura {

private: 
    double radio;
    
public:
    // constructor de parametros
    Circulo(std::string nombre, double radio);

    // métodos
    double calcularArea() override;
    void describir() override;

    // getter
    double obtenerRadio();
};

#endif