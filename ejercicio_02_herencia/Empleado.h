#ifndef EMPLEADO_H
#define EMPLEADO_H

#include <string>
using namespace std;

class Empleado {
    protected:
         string nombre;
         string id;
         float salarioBase;

    public:

         Empleado();
         Empleado(string nombre, string id, float salarioBase);

         void actualizarNombre(string nombre);
         void actualizarId(string id);
         void actualizarsalarioBase(float salarioBase);

         string obtenerNombre();
         string obtenerId();
         float obtenersalarioBase();

         void mostrarInformacion();

};

#endif