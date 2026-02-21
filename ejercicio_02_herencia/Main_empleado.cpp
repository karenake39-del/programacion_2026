#include <iostream>
#include "Empleado_tiempo_completo.h"

using namespace std;

int main (){
    cout << "=== Sistema de Gestión de Empleados ===" << endl;
    cout << endl;

    Empleado emp1("Ana López", "EMP-001", 15000.0);
    cout << "Empleado (clase base):" << endl;
    emp1.mostrarInformacion();
    cout << endl;

    EmpleadoTiempoCompleto etc1;
    cout << "Empleado tiempo completo: " << endl;
    etc1.mostrarInformacion();
    cout << endl;

  EmpleadoTiempoCompleto etc2("Carlos Ruiz", "EMP-002", 20000.0, 40, 2000.0);
    cout << "Empleado tiempo completo: " << endl;
    etc2.mostrarInformacion();
    cout << "Salario total: " << etc2.calcularSalarioTotal() << endl;
    cout << endl;

    etc2.actualizarBono(3000.0);
    cout << "Despues de actualizar bono: "<< endl;
    etc2.mostrarInformacion();
    cout << "Salario total: " << etc2.calcularSalarioTotal() << endl;

    return 0;
    
}
