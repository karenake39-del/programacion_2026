#include "figura.h"
#include <iostream>

Figura::Figura(std::string Nombrefigura) {
    this->nombrefigura = Nombrefigura;
}

Figura::~Figura() {
    // destructor virtual
}

std::string Figura::obtenerNombre() {
    return nombrefigura;
}