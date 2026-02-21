#include <iostream>

#include "celular.h"

int main() {

    // Crear un objeto Celular usando el constructor
    Celular celular1("Galaxy S23", "1234", "Samsung", false, 18000);

    std::cout << "=== INFORMACION INICIAL ===" << std::endl;
    celular1.mostrarInformacion();

    // Encender el celular
    std::cout << "\nEncendiendo el celular..." << std::endl;
    celular1.encenderApagar();
    celular1.mostrarInformacion();

    // Verificar PIN correcto
    std::cout << "\nVerificando PIN 1234..." << std::endl;
    if (celular1.verificarPin("1234")) {
        std::cout << "PIN correcto. Acceso permitido." << std::endl;
    } else {
        std::cout << "PIN incorrecto. Acceso denegado." << std::endl;
    }

    // Verificar PIN incorrecto
    std::cout << "\nVerificando PIN 0000..." << std::endl;
    if (celular1.verificarPin("0000")) {
        std::cout << "PIN correcto. Acceso permitido." << std::endl;
    } else {
        std::cout << "PIN incorrecto. Acceso denegado." << std::endl;
    }

    return 0;
}