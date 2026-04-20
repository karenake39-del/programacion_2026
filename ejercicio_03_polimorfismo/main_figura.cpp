#include "figura.h"
#include "circulo.h"
#include "rectangulo.h"
#include <iostream>

int main() {

    std::cout << "=== Sistema de Figuras Geométricas (Polimorfismo) ===" << std::endl;
    std::cout << std::endl;

    // Punteros a la clase base
    Figura* f1 = new Circulo("Circulo", 5.0);
    Figura* f2 = new Rectangulo("Rectangulo", 4.0, 6.0);

    std::cout << "--- Figura 1 (Circulo) ---" << std::endl;
    f1->describir();
    std::cout << "Area: " << f1->calcularArea() << std::endl;
    std::cout << std::endl;

    std::cout << "--- Figura 2 (Rectangulo) ---" << std::endl;
    f2->describir();
    std::cout << "Area: " << f2->calcularArea() << std::endl;
    std::cout << std::endl;

    // Polimorfismo: tratamiento uniforme
    std::cout << "--- Tratamiento uniforme con Figura* ---" << std::endl;

    Figura* figuras[] = {f1, f2};

    for(int i = 0; i < 2; i++) {
        figuras[i]->describir();
        std::cout << "Area: " << figuras[i]->calcularArea() << std::endl;
    }

    // Liberar memoria
    delete f1;
    delete f2;

    return 0;
}