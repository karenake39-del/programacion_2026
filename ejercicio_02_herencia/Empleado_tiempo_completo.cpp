#include "Empleado_tiempo_completo.h"
#include <iostream>

using namespace std;

EmpleadoTiempoCompleto::EmpleadoTiempoCompleto() : Empleado() {
    horasSemanales = 0;
    bono = 0.0;
}

EmpleadoTiempoCompleto::EmpleadoTiempoCompleto(string nombre, string id, float salarioBase, int horasSemanales, float bono) : Empleado(nombre, id, salarioBase){
    this->horasSemanales = horasSemanales;
    this->bono = bono;
}

void EmpleadoTiempoCompleto::actualizarHorasSemanales(int nuevasHorasSemanales){
    horasSemanales = nuevasHorasSemanales;
}

void EmpleadoTiempoCompleto::actualizarBono(float nuevoBono){
    bono = nuevoBono;
}

int EmpleadoTiempoCompleto::obtenerHorasSemanales(){
    return horasSemanales;
}

float EmpleadoTiempoCompleto::obtenerBono(){
    return bono;
}

void EmpleadoTiempoCompleto::mostrarInformacion(){
    Empleado::mostrarInformacion();
    cout << "Horas semanales: " << horasSemanales << endl;
    cout << "Bono: " << bono << endl;
}

float EmpleadoTiempoCompleto::calcularSalarioTotal(){
    return salarioBase + bono;
}

