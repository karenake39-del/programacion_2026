#include <iostream>
#include "Estudiante.h"

using namespace std;

int main() {
    std::cout << "= Sistema de Estudiante =" << std::endl;
    std::cout << std::endl;

    Estudiante estudiante1;
    std::cout << "Estudiante 1 (constructor por defecto):" << std::endl;
    estudiante1.mostrarInformacionCompleta();
    std::cout << std::endl;

    estudiante1.actualizarNombre("Jessica Ivonne Cupul Canul");
    estudiante1.actualizarCodigo("A56478652");
    estudiante1.actualizarEdad(20);
    estudiante1.actualizarPromedio(9.2);
    estudiante1.actualizarActivo(true);

    std::cout << "Estudiante 1 después de actualizar:" << std::endl;
    estudiante1.mostrarInformacionCompleta();
    estudiante1.calcularEstadoAcademico();
    std::cout << std::endl;

    Estudiante estudiante2("Estefania Del Rosario Chuil Castillo", "A67534902", 22, 7.5, true);
    std::cout << "Estudiante 2 (constructor con parámetros):" << std::endl;
    estudiante2.mostrarInformacionCompleta();
    estudiante2.calcularEstadoAcademico();
    std::cout << std::endl;

    std::cout << "Incrementando edad del estudiante 2:" << std::endl;
    estudiante2.incrementarEdad();
    estudiante2.mostrarEdad();
    std::cout << std::endl;

    std::cout << "Intentando actualizar promedio con valor inválido:" << std::endl;
    estudiante2.actualizarPromedio(15.0);
    estudiante2.mostrarPromedio();
    std::cout << std::endl;

    return 0;
}
