#include "figura.h"
#include <iostream>

Figura::Figura(std::string nombreFigura) {
    this -> nombreFigura = nombreFigura;
    
}

Figura::~ Figura () {
    //destructor vircial (de la clase derivada correcta)
}

std::string Figura::obtenerNombreFigura(){
    return nombreFigura;
    
};