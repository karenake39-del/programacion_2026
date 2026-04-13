#ifndef FIGURA_H
#define FIGURA_H

#include <string>

class Figura {
    protected:
    std::string nombrefigura;
    
    public: 
    Figura(std::string Nombrefigura);
    virtual ~Figura();
    
    //funciones virtuales puras: la clase abstracta
    virtual void describir() = 0;
    virtual void calcularArea() = 0;
    
    //getters
    std::string obtenerNombre();
};

#endif