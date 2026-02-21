#include <iostream>
#include "celular.h"

using namespace std;

Celular::Celular(std::string modelo, std::string pin, std::string marca, bool encendido, double precio) {
    this->modelo = modelo;
    this->pin = pin;
    this->marca = marca;
    this->encendido = encendido;
    this->precio = precio;
}

void Celular::encenderApagar() {
    encendido = !encendido;
}


bool Celular::verificarPin(std::string pinIngresado) {
    if (pinIngresado == pin) {
        return true;
    } else {
        return false;
    }
}


void Celular::mostrarInformacion() {
    std::cout << "Marca: " << marca << std::endl;
    std::cout << "Modelo: " << modelo << std::endl;
    std::cout << "Precio: $" << precio << std::endl;

    if (encendido) {
        std::cout << "Estado: Encendido" << std::endl;
    } else {
        std::cout << "Estado: Apagado" << std::endl;
    }
}