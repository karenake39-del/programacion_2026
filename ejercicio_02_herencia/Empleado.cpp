#include "Empleado.h"
#include <iostream>

using namespace std;

Empleado::Empleado() {
    nombre = "";
    id = "";
    salarioBase = 0.0;
}

Empleado::Empleado(string nombre, string id, float salarioBase){
    this->nombre = nombre;
    this->id = id;
    this->salarioBase = salarioBase;
}

void Empleado::actualizarNombre(string nuevoNombre){
    nombre = nuevoNombre;
}

void Empleado::actualizarId(string nuevoId){
    id = nuevoId;
}

void Empleado::actualizarsalarioBase(float nuevosalarioBase){
    salarioBase = nuevosalarioBase;
}

string Empleado::obtenerNombre(){
    return nombre;
}

string Empleado::obtenerId(){
    return id;
}

float Empleado::obtenersalarioBase(){
    return salarioBase;
}

void Empleado::mostrarInformacion(){
    cout << "Nombre: " << nombre << endl;
    cout << "ID: " << id << endl;
    cout << "Salario base: $" << salarioBase<< endl;
}
