#include <stdio.h>
#include <string.h>

typedef struct {
    char callePrincipal[50];
    int numeroCasa;
    char calleSecundaria[50];
} Direccion;

typedef struct {
    char nombre[50];
    Direccion direccion;
    int edad;
} Alumno;

/* Prototipos de las funciones */
void mostrarAlumno(Alumno *a, int tam);
//void guardarArchivo(Alumno *a, int tam);
//void leerArchivo(void);
void leerCSV();
int main() {
    Alumno clase[2];

    strcpy(clase[0].nombre, "Juan");
    strcpy(clase[0].direccion.callePrincipal, "Av. Prensa");
    clase[0].direccion.numeroCasa = 123;
    strcpy(clase[0].direccion.calleSecundaria, "Florida");
    clase[0].edad = 20;


    strcpy(clase[1].nombre, "Ana");
    strcpy(clase[1].direccion.callePrincipal, "Av. Galo Plaza");
    clase[1].direccion.numeroCasa = 543;
    strcpy(clase[1].direccion.calleSecundaria, "Inca");
    clase[1].edad = 21;

    printf("ALUMNOS REGISTRADOS\n\n");

    mostrarAlumno(clase, 2);

    guardarArchivo(clase, 2);

    printf("\nCONTENIDO DEL ARCHIVO\n\n");

    leerArchivo();

    return 0;
}

/* Muestra todos los alumnos del arreglo */
void mostrarAlumno(Alumno *a, int tam) {

    for (int i = 0; i < tam; i++) {

        printf("Nombre: %s\n", a[i].nombre);
        printf("Edad: %d\n", a[i].edad);

        printf(
            "Direccion: %s %d %s\n",
            a[i].direccion.callePrincipal,
            a[i].direccion.numeroCasa,
            a[i].direccion.calleSecundaria
        );

        printf("--------------------------\n");
    }
}

void guardarArchivo(Alumno *a, int tam) {

    FILE *archivo = fopen("alumnos.txt", "w");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    for (int i = 0; i < tam; i++) {

       // fprintf(archivo, "Nombre: %s\n", a[i].nombre);//
        //printf(archivo, "Edad: %d\n", a[i].edad);//

        fprintf(
            archivo,
            "Direccion: %s; %d; %s ;%d; %s\n",
            a[i].nombre,
            a[i].edad,
            a[i].direccion.callePrincipal,
            a[i].direccion.numeroCasa,
            a[i].direccion.calleSecundaria
        );

       // fprintf(archivo, "--------------------------\n");
    }

    fclose(archivo);

    printf("\nDatos guardados en alumnos.txt\n");
}

/* Lee y muestra el contenido del archivo */
void leerArchivo(void) {

    FILE *archivo = fopen("alumnos.txt", "r");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

    char linea[200];

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        printf("%s", linea);
    }

    fclose(archivo);
}

void leerCSV(){
    FILE *archivo = fopen("alumnos.txt", "w");

    if (archivo == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }

fclose
}