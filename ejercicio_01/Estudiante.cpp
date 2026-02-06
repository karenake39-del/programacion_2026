#ifndef ESTUDIANTES_H
#define ESTUDIANTES_H

#include <string>

class Estudiante {
private:
    std::string nombre;
    std::string codigo;
    int edad;
    float promedio;
    bool activo;

public:

    Estudiante();
    Estudiante(std::string nombre, std::string codigo, int edad, float promedio, bool activo);

    void actualizarNombre(std::string nuevoNombre);
    void actualizarCodigo(std::string nuevoCodigo);
    void actualizarEdad(int nuevaEdad);
    void actualizarPromedio(float nuevoPromedio);
    void actualizarActivo(bool nuevoEstado);

    std::string obtenerNombre();
    std::string obtenerCodigo();
    int obtenerEdad();
    float obtenerPromedio();
    bool obtenerActivo();

    void mostrarNombre();
    void mostrarCodigo();
    void mostrarEdad();
    void mostrarPromedio();
    void mostrarActivo();
    void mostrarInformacionCompleta();

    void incrementarEdad();
    void calcularEstadoAcademico();
};

#endif
