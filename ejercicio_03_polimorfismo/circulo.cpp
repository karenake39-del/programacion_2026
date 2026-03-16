#include "circulo.h"
#include <iostream>

Circulo::Circulo(std::string nombre, double radio) : Figura(nombre) {
    this->radio = radio;
}

double Circulo::calcularArea() {
    return 3.14159 * radio * radio;
}

void Circulo::describir() {
    std::cout << "Figura: " << nombrefigura << ", radio: " << radio << std::endl;
}

double Circulo::obtenerRadio() {
    return radio;
}