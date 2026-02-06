#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include <string>

class Estudiante {
private:
    std::string nombre;
    std::string codigo;
    int edad;
    float promedio;
    bool activo;

public:
    // Constructores
    Estudiante();
    Estudiante(std::string nombre, std::string codigo, int edad, float promedio, bool activo);

    // Setters
    void actualizarNombre(std::string nuevoNombre);
    void actualizarCodigo(std::string nuevoCodigo);
    void actualizarEdad(int nuevaEdad);
    void actualizarPromedio(float nuevoPromedio);
    void actualizarActivo(bool nuevoEstado);

    // Getters
    std::string obtenerNombre();
    std::string obtenerCodigo();
    int obtenerEdad();
    float obtenerPromedio();
    bool obtenerActivo();

    // Métodos de visualización
    void mostrarNombre();
    void mostrarCodigo();
    void mostrarEdad();
    void mostrarPromedio();
    void mostrarActivo();
    void mostrarInformacionCompleta();

    // Métodos de utilidad
    void incrementarEdad();
    void calcularEstadoAcademico();
};

#endif
