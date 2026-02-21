#ifndef EMPLEADO_TIEMPO_COMPLETO_H
#define EMPLEADO_TIEMPO_COMPLETO_H

#include <string>
#include "Empleado.h"

class EmpleadoTiempoCompleto : public Empleado {
    private:
         int horasSemanales;
         float bono;

    public: 
         EmpleadoTiempoCompleto();
         EmpleadoTiempoCompleto(string nombre, string id, float salarioBase, int horasSemanales, float bono);


         void actualizarHorasSemanales(int horasSemanales);
         void actualizarBono(float bono);

         int obtenerHorasSemanales();
         float obtenerBono();

         void mostrarInformacion();

         float calcularSalarioTotal();
};

#endif
