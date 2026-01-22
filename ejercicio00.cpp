#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 50

typedef struct {
    char nombre[100];
    char codigo[20];
    int edad;
    float promedio;
    bool activo;
} Estudiante;

void actualizarNombre(Estudiante *e, char nuevoNombre[]) {
    strcpy(e->nombre, nuevoNombre);
}

void actualizarCodigo(Estudiante *e, char nuevoCodigo[]) {
    strcpy(e->codigo, nuevoCodigo);
}

void actualizarizarEdad(Estudiante *e, int nuevaEdad) {
    e->edad = nuevaEdad;
}

void actualizarPromedio(Estudiante *e, float nuevoPromedio) {
    if (nuevoPromedio >= 0.0 && nuevoPromedio <= 10.0) {
        e->promedio = nuevoPromedio;
    } else {
        printf("Promedio invalido\n");
    }
}

void actualizarActivo(Estudiante *e, bool nuevoEstado) {
    e->activo = nuevoEstado;
}

void mostrarInformacionCompleta(Estudiante e) {
    printf("Nombre: %s\n", e.nombre);
    printf("Codigo: %s\n", e.codigo);
    printf("Edad: %d años\n", e.edad);
    printf("Promedio: %.2f\n", e.promedio);
    printf("Estado: %s\n", e.activo ? "Activo" : "Inactivo");
}

int main() {

    Estudiante estudiantes[MAX];
    int total = 0;
    int opcion;

    do {
        printf("\n--- MENU ---\n");
        printf("1. Agregar estudiante\n");
        printf("2. Mostrar un estudiante\n");
        printf("3. Mostrar todos los estudiantes\n");
        printf("4. Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &opcion);
        getchar(); // limpiar ENTER

        // ===== OPCIÓN 1 =====
        if (opcion == 1 && total < MAX) {

            char nombre[100];
            char codigo[20];
            int edad;
            float promedio;
            int activo;

            printf("Nombre: ");
            fgets(nombre, 100, stdin);
            nombre[strcspn(nombre, "\n")] = 0;

            printf("Codigo: ");
            fgets(codigo, 20, stdin);
            codigo[strcspn(codigo, "\n")] = 0;

            printf("Edad: ");
            scanf("%d", &edad);

            printf("Promedio: ");
            scanf("%f", &promedio);

            printf("Activo (1 = si, 0 = no): ");
            scanf("%d", &activo);
            getchar();

            actualizarNombre(&estudiantes[total], nombre);
            actualizarCodigo(&estudiantes[total], codigo);
            actualizarizarEdad(&estudiantes[total], edad);
            actualizarPromedio(&estudiantes[total], promedio);
            actualizarActivo(&estudiantes[total], activo);

            total++;
            printf("Estudiante agregado correctamente.\n");
        }

        else if (opcion == 2) {

            char buscarCodigo[20];
            bool encontrado = false;

            printf("Codigo del estudiante: ");
            fgets(buscarCodigo, 20, stdin);
            buscarCodigo[strcspn(buscarCodigo, "\n")] = 0;

            for (int i = 0; i < total; i++) {
                if (strcmp(estudiantes[i].codigo, buscarCodigo) == 0) {
                    mostrarInformacionCompleta(estudiantes[i]);
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                printf("Estudiante no encontrado.\n");
            }
        }

        else if (opcion == 3) {

            if (total == 0) {
                printf("No hay estudiantes registrados.\n");
            } else {
                for (int i = 0; i < total; i++) {
                    printf("\nEstudiante %d\n", i + 1);
                    mostrarInformacionCompleta(estudiantes[i]);
                }
            }
        }

    } while (opcion != 4);

    printf("Programa finalizado.\n");
    return 0;
}
