#include <stdio.h>

#define MAX 50
#define ASIGNATURAS 3

int main() {
    int estudiantes;
    float notas[MAX][ASIGNATURAS];

    printf("Ingrese el numero de estudiantes: ");
    scanf("%d", &estudiantes);

   
    for (int i = 0; i < estudiantes; i++) {
        printf("\nEstudiante %d\n", i + 1);

        for (int j = 0; j < ASIGNATURAS; j++) {
            do {
                printf("Nota de asignatura %d: ", j + 1);
                scanf("%f", &notas[i][j]);

                if (notas[i][j] < 0 || notas[i][j] > 10) {
                    printf("Nota invalida. Ingrese entre 0 y 10.\n");
                }

            } while (notas[i][j] < 0 || notas[i][j] > 10);
        }
    }

  
    printf("\n==== MATRIZ DE NOTAS ====\n");
    printf("Estudiante\tA1\tA2\tA3\n");

    for (int i = 0; i < estudiantes; i++) {
        printf("%d\t\t", i + 1);

        for (int j = 0; j < ASIGNATURAS; j++) {
            printf("%.2f\t", notas[i][j]);
        }

        printf("\n");
    }

    printf("\n==== RESULTADOS POR ESTUDIANTE ====\n");

    for (int i = 0; i < estudiantes; i++) {
        float suma = 0;
        float mayor = notas[i][0];
        float menor = notas[i][0];

        for (int j = 0; j < ASIGNATURAS; j++) {
            suma = suma + notas[i][j];

            if (notas[i][j] > mayor) {
                mayor = notas[i][j];
            }

            if (notas[i][j] < menor) {
                menor = notas[i][j];
            }
        }

        printf("\nEstudiante %d\n", i + 1);
        printf("Promedio: %.2f\n", suma / ASIGNATURAS);
        printf("Nota mas alta: %.2f\n", mayor);
        printf("Nota mas baja: %.2f\n", menor);
    }

   
    printf("\n===== RESULTADOS POR ASIGNATURA =====\n");

    for (int j = 0; j < ASIGNATURAS; j++) {
        float suma = 0;
        float mayor = notas[0][j];
        float menor = notas[0][j];
        int aprobados = 0;
        int reprobados = 0;

        for (int i = 0; i < estudiantes; i++) {
            suma = suma + notas[i][j];

            if (notas[i][j] > mayor) {
                mayor = notas[i][j];
            }

            if (notas[i][j] < menor) {
                menor = notas[i][j];
            }

            if (notas[i][j] >= 6) {
                aprobados++;
            } else {
                reprobados++;
            }
        }

        printf("\nAsignatura %d\n", j + 1);
        printf("Promedio: %.2f\n", suma / estudiantes);
        printf("Nota mas alta: %.2f\n", mayor);
        printf("Nota mas baja: %.2f\n", menor);
        printf("Aprobados: %d\n", aprobados);
        printf("Reprobados %d\n", reprobados);
    }

    return 0;
}