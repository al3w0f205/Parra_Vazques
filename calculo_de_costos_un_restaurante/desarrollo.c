#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INGREDIENTES 200
#define MAX_PLATOS 100
#define MAX_RELACIONES 1000
#define MAX_CODIGO 20
#define MAX_NOMBRE 60
#define MAX_UNIDAD 25
#define MAX_CATEGORIA 40
#define MAX_LINEA 256

#define ARCH_INGREDIENTES "ingredientes.csv"
#define ARCH_PLATOS "platos.csv"
#define ARCH_RELACIONES "plato_ingredientes.csv"

typedef struct {
    char codigo[MAX_CODIGO];
    char nombre[MAX_NOMBRE];
    float costoUnitario;
    char unidadMedida[MAX_UNIDAD];
} Ingrediente;

typedef struct {
    char codigo[MAX_CODIGO];
    char nombre[MAX_NOMBRE];
    char categoria[MAX_CATEGORIA];
    float impuesto;
    float servicio;
    float ganancia;
} Plato;

typedef struct {
    char codigoPlato[MAX_CODIGO];
    char codigoIngrediente[MAX_CODIGO];
    float cantidadUsada;
} PlatoIngrediente;

typedef struct {
    Ingrediente ingredientes[MAX_INGREDIENTES];
    Plato platos[MAX_PLATOS];
    PlatoIngrediente relaciones[MAX_RELACIONES];

    int totalIngredientes;
    int totalPlatos;
    int totalRelaciones;
} Sistema;


/* PROTOTIPOS */

void limpiarSalto(char *texto);
void leerCadena(const char *mensaje, char *destino, int tamano);
int leerEntero(const char *mensaje, int minimo, int maximo);
float leerFloat(const char *mensaje, float minimo, float maximo,
                int incluirMinimo);
int confirmar(const char *mensaje);
int contieneTexto(const char *texto, const char *busqueda);
void pausa(void);

int buscarIngrediente(const Sistema *s, const char *codigo);
int buscarPlato(const Sistema *s, const char *codigo);
int buscarRelacion(const Sistema *s, const char *codPlato,
                   const char *codIng);
int ingredienteEnUso(const Sistema *s, const char *codigo);

void cargarDatos(Sistema *s);
void guardarDatos(const Sistema *s);

void cargarIngredientes(Sistema *s);
void cargarPlatos(Sistema *s);
void cargarRelaciones(Sistema *s);

void guardarIngredientes(const Sistema *s);
void guardarPlatos(const Sistema *s);
void guardarRelaciones(const Sistema *s);

void menuIngredientes(Sistema *s);
void registrarIngrediente(Sistema *s);
void listarIngredientes(const Sistema *s);
void buscarIngredienteMenu(const Sistema *s);
void actualizarIngrediente(Sistema *s);
void eliminarIngrediente(Sistema *s);

void menuPlatos(Sistema *s);
void registrarPlato(Sistema *s);
void listarPlatos(const Sistema *s);
void buscarPlatoMenu(const Sistema *s);
void actualizarPlato(Sistema *s);
void eliminarPlato(Sistema *s);

void menuRelaciones(Sistema *s);
void asociarIngrediente(Sistema *s);
void listarIngredientesPlato(const Sistema *s);
void actualizarCantidad(Sistema *s);
void eliminarRelacion(Sistema *s);

float calcularCostoIngredientes(const Sistema *s,
                                const char *codigoPlato);

float calcularCostoFinal(const Sistema *s, const Plato *plato);

void menuReportes(const Sistema *s);
void mostrarDetalleCosto(const Sistema *s);
void reporteGeneral(const Sistema *s);


/* PROGRAMA PRINCIPAL */

int main(void) {

    Sistema sistema = {0};
    int opcion;

    cargarDatos(&sistema);

    do {
        printf("\n==========================================\n");
        printf(" SISTEMA DE COSTOS DE RESTAURANTE\n");
        printf("==========================================\n");
        printf("1. Gestion de ingredientes\n");
        printf("2. Gestion de platos\n");
        printf("3. Ingredientes por plato\n");
        printf("4. Calculo y reportes\n");
        printf("5. Guardar cambios\n");
        printf("6. Salir\n");

        opcion = leerEntero("Seleccione una opcion: ", 1, 6);

        switch (opcion) {

            case 1:
                menuIngredientes(&sistema);
                break;

            case 2:
                menuPlatos(&sistema);
                break;

            case 3:
                menuRelaciones(&sistema);
                break;

            case 4:
                menuReportes(&sistema);
                break;

            case 5:
                guardarDatos(&sistema);
                printf("Datos guardados correctamente.\n");
                pausa();
                break;

            case 6:
                if (confirmar("Desea guardar antes de salir")) {
                    guardarDatos(&sistema);
                }

                printf("Programa finalizado.\n");
                break;
        }

    } while (opcion != 6);

    return 0;
}


/* UTILIDADES */

void limpiarSalto(char *texto) {
    texto[strcspn(texto, "\r\n")] = '\0';
}


void leerCadena(const char *mensaje, char *destino, int tamano) {

    int caracter;

    while (1) {

        printf("%s", mensaje);

        if (fgets(destino, tamano, stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (strchr(destino, '\n') == NULL) {

            while ((caracter = getchar()) != '\n' &&
                   caracter != EOF) {
            }
        }

        limpiarSalto(destino);

        if (strlen(destino) == 0) {
            printf("La entrada no puede estar vacia.\n");
        }

        else if (strchr(destino, ',') != NULL) {
            printf("No escriba comas porque se guarda en CSV.\n");
        }

        else {
            return;
        }
    }
}


int leerEntero(const char *mensaje, int minimo, int maximo) {

    char linea[100];
    char *fin;
    long valor;

    while (1) {

        printf("%s", mensaje);

        if (fgets(linea, sizeof(linea), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        valor = strtol(linea, &fin, 10);

        while (isspace((unsigned char)*fin)) {
            fin++;
        }

        if (fin != linea &&
            *fin == '\0' &&
            valor >= minimo &&
            valor <= maximo) {

            return (int)valor;
        }

        printf("Ingrese un entero entre %d y %d.\n",
               minimo, maximo);
    }
}


float leerFloat(const char *mensaje, float minimo,
                float maximo, int incluirMinimo) {

    char linea[100];
    char *fin;
    float valor;
    int valido;

    while (1) {

        printf("%s", mensaje);

        if (fgets(linea, sizeof(linea), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        valor = strtof(linea, &fin);

        while (isspace((unsigned char)*fin)) {
            fin++;
        }

        if (incluirMinimo) {
            valido = valor >= minimo;
        } else {
            valido = valor > minimo;
        }

        if (fin != linea &&
            *fin == '\0' &&
            valido &&
            valor <= maximo) {

            return valor;
        }

        printf("Valor invalido. Intente nuevamente.\n");
    }
}


int confirmar(const char *mensaje) {

    char respuesta[10];

    while (1) {

        printf("%s (s/n): ", mensaje);

        if (fgets(respuesta, sizeof(respuesta), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }

        if (respuesta[0] == 's' ||
            respuesta[0] == 'S') {

            return 1;
        }

        if (respuesta[0] == 'n' ||
            respuesta[0] == 'N') {

            return 0;
        }

        printf("Escriba s o n.\n");
    }
}


int contieneTexto(const char *texto, const char *busqueda) {

    char textoMinuscula[MAX_NOMBRE];
    char busquedaMinuscula[MAX_NOMBRE];

    int i;

    snprintf(textoMinuscula,
             sizeof(textoMinuscula),
             "%s",
             texto);

    snprintf(busquedaMinuscula,
             sizeof(busquedaMinuscula),
             "%s",
             busqueda);

    for (i = 0; textoMinuscula[i] != '\0'; i++) {

        textoMinuscula[i] =
            (char)tolower(
                (unsigned char)textoMinuscula[i]
            );
    }

    for (i = 0; busquedaMinuscula[i] != '\0'; i++) {

        busquedaMinuscula[i] =
            (char)tolower(
                (unsigned char)busquedaMinuscula[i]
            );
    }

    return strstr(textoMinuscula,
                  busquedaMinuscula) != NULL;
}


void pausa(void) {

    char linea[10];

    printf("\nPresione ENTER para continuar...");
    fgets(linea, sizeof(linea), stdin);
}


/* FUNCIONES DE BUSQUEDA */

int buscarIngrediente(const Sistema *s,
                      const char *codigo) {

    int i;

    for (i = 0; i < s->totalIngredientes; i++) {

        if (strcmp(s->ingredientes[i].codigo,
                   codigo) == 0) {

            return i;
        }
    }

    return -1;
}


int buscarPlato(const Sistema *s,
                const char *codigo) {

    int i;

    for (i = 0; i < s->totalPlatos; i++) {

        if (strcmp(s->platos[i].codigo,
                   codigo) == 0) {

            return i;
        }
    }

    return -1;
}


int buscarRelacion(const Sistema *s,
                   const char *codPlato,
                   const char *codIng) {

    int i;

    for (i = 0; i < s->totalRelaciones; i++) {

        if (strcmp(s->relaciones[i].codigoPlato,
                   codPlato) == 0 &&

            strcmp(s->relaciones[i].codigoIngrediente,
                   codIng) == 0) {

            return i;
        }
    }

    return -1;
}


int ingredienteEnUso(const Sistema *s,
                     const char *codigo) {

    int i;

    for (i = 0; i < s->totalRelaciones; i++) {

        if (strcmp(
                s->relaciones[i].codigoIngrediente,
                codigo
            ) == 0) {

            return 1;
        }
    }

    return 0;
}


/* ARCHIVOS CSV */

void cargarDatos(Sistema *s) {

    s->totalIngredientes = 0;
    s->totalPlatos = 0;
    s->totalRelaciones = 0;

    cargarIngredientes(s);
    cargarPlatos(s);
    cargarRelaciones(s);
}


void guardarDatos(const Sistema *s) {

    guardarIngredientes(s);
    guardarPlatos(s);
    guardarRelaciones(s);
}


void cargarIngredientes(Sistema *s) {

    FILE *archivo;
    char linea[MAX_LINEA];

    archivo = fopen(ARCH_INGREDIENTES, "r");

    if (archivo == NULL) {
        return;
    }

    fgets(linea, sizeof(linea), archivo);

    while (s->totalIngredientes < MAX_INGREDIENTES &&
           fgets(linea, sizeof(linea), archivo) != NULL) {

        Ingrediente *ing;
        char *token;

        ing = &s->ingredientes[s->totalIngredientes];

        limpiarSalto(linea);

        token = strtok(linea, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(ing->codigo,
                 sizeof(ing->codigo),
                 "%s",
                 token);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(ing->nombre,
                 sizeof(ing->nombre),
                 "%s",
                 token);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        ing->costoUnitario = strtof(token, NULL);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(ing->unidadMedida,
                 sizeof(ing->unidadMedida),
                 "%s",
                 token);

        if (ing->costoUnitario > 0) {
            s->totalIngredientes++;
        }
    }

    fclose(archivo);
}


void cargarPlatos(Sistema *s) {

    FILE *archivo;
    char linea[MAX_LINEA];

    archivo = fopen(ARCH_PLATOS, "r");

    if (archivo == NULL) {
        return;
    }

    fgets(linea, sizeof(linea), archivo);

    while (s->totalPlatos < MAX_PLATOS &&
           fgets(linea, sizeof(linea), archivo) != NULL) {

        Plato *plato;
        char *token;

        plato = &s->platos[s->totalPlatos];

        limpiarSalto(linea);

        token = strtok(linea, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(plato->codigo,
                 sizeof(plato->codigo),
                 "%s",
                 token);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(plato->nombre,
                 sizeof(plato->nombre),
                 "%s",
                 token);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(plato->categoria,
                 sizeof(plato->categoria),
                 "%s",
                 token);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        plato->impuesto = strtof(token, NULL);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        plato->servicio = strtof(token, NULL);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        plato->ganancia = strtof(token, NULL);

        s->totalPlatos++;
    }

    fclose(archivo);
}


void cargarRelaciones(Sistema *s) {

    FILE *archivo;
    char linea[MAX_LINEA];

    archivo = fopen(ARCH_RELACIONES, "r");

    if (archivo == NULL) {
        return;
    }

    fgets(linea, sizeof(linea), archivo);

    while (s->totalRelaciones < MAX_RELACIONES &&
           fgets(linea, sizeof(linea), archivo) != NULL) {

        PlatoIngrediente *relacion;
        char *token;

        relacion =
            &s->relaciones[s->totalRelaciones];

        limpiarSalto(linea);

        token = strtok(linea, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(relacion->codigoPlato,
                 sizeof(relacion->codigoPlato),
                 "%s",
                 token);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        snprintf(relacion->codigoIngrediente,
                 sizeof(relacion->codigoIngrediente),
                 "%s",
                 token);

        token = strtok(NULL, ",");

        if (token == NULL) {
            continue;
        }

        relacion->cantidadUsada =
            strtof(token, NULL);

        if (relacion->cantidadUsada > 0) {
            s->totalRelaciones++;
        }
    }

    fclose(archivo);
}


void guardarIngredientes(const Sistema *s) {

    FILE *archivo;
    int i;

    archivo = fopen(ARCH_INGREDIENTES, "w");

    if (archivo == NULL) {
        printf("Error al guardar ingredientes.csv\n");
        return;
    }

    fprintf(
        archivo,
        "codigo_ing,nombre_ing,costo_unitario,"
        "unidad_medida\n"
    );

    for (i = 0; i < s->totalIngredientes; i++) {

        fprintf(
            archivo,
            "%s,%s,%.4f,%s\n",
            s->ingredientes[i].codigo,
            s->ingredientes[i].nombre,
            s->ingredientes[i].costoUnitario,
            s->ingredientes[i].unidadMedida
        );
    }

    fclose(archivo);
}


void guardarPlatos(const Sistema *s) {

    FILE *archivo;
    int i;

    archivo = fopen(ARCH_PLATOS, "w");

    if (archivo == NULL) {
        printf("Error al guardar platos.csv\n");
        return;
    }

    fprintf(
        archivo,
        "codigo_plato,nombre_plato,categoria,"
        "impuesto_porcentaje,servicio_porcentaje,"
        "ganancia_porcentaje\n"
    );

    for (i = 0; i < s->totalPlatos; i++) {

        fprintf(
            archivo,
            "%s,%s,%s,%.2f,%.2f,%.2f\n",
            s->platos[i].codigo,
            s->platos[i].nombre,
            s->platos[i].categoria,
            s->platos[i].impuesto,
            s->platos[i].servicio,
            s->platos[i].ganancia
        );
    }

    fclose(archivo);
}


void guardarRelaciones(const Sistema *s) {

    FILE *archivo;
    int i;

    archivo = fopen(ARCH_RELACIONES, "w");

    if (archivo == NULL) {
        printf("Error al guardar plato_ingredientes.csv\n");
        return;
    }

    fprintf(
        archivo,
        "codigo_plato,codigo_ing,cantidad_usada\n"
    );

    for (i = 0; i < s->totalRelaciones; i++) {

        fprintf(
            archivo,
            "%s,%s,%.4f\n",
            s->relaciones[i].codigoPlato,
            s->relaciones[i].codigoIngrediente,
            s->relaciones[i].cantidadUsada
        );
    }

    fclose(archivo);
}


/* GESTION DE INGREDIENTES */

void menuIngredientes(Sistema *s) {

    int opcion;

    do {
        printf("\n=== GESTION DE INGREDIENTES ===\n");
        printf("1. Registrar ingrediente\n");
        printf("2. Listar ingredientes\n");
        printf("3. Buscar ingrediente\n");
        printf("4. Actualizar ingrediente\n");
        printf("5. Eliminar ingrediente\n");
        printf("0. Volver\n");

        opcion = leerEntero("Opcion: ", 0, 5);

        switch (opcion) {

            case 1:
                registrarIngrediente(s);
                pausa();
                break;

            case 2:
                listarIngredientes(s);
                pausa();
                break;

            case 3:
                buscarIngredienteMenu(s);
                pausa();
                break;

            case 4:
                actualizarIngrediente(s);
                pausa();
                break;

            case 5:
                eliminarIngrediente(s);
                pausa();
                break;
        }

    } while (opcion != 0);
}


void registrarIngrediente(Sistema *s) {

    Ingrediente nuevo;

    if (s->totalIngredientes >= MAX_INGREDIENTES) {
        printf("No hay espacio para mas ingredientes.\n");
        return;
    }

    leerCadena(
        "Codigo: ",
        nuevo.codigo,
        sizeof(nuevo.codigo)
    );

    if (buscarIngrediente(s, nuevo.codigo) >= 0) {
        printf("Ese codigo ya existe.\n");
        return;
    }

    leerCadena(
        "Nombre: ",
        nuevo.nombre,
        sizeof(nuevo.nombre)
    );

    nuevo.costoUnitario =
        leerFloat(
            "Costo unitario (>0): ",
            0,
            1000000,
            0
        );

    leerCadena(
        "Unidad de medida: ",
        nuevo.unidadMedida,
        sizeof(nuevo.unidadMedida)
    );

    s->ingredientes[s->totalIngredientes] = nuevo;
    s->totalIngredientes++;

    printf("Ingrediente registrado.\n");
}


void listarIngredientes(const Sistema *s) {

    int i;

    if (s->totalIngredientes == 0) {
        printf("No hay ingredientes registrados.\n");
        return;
    }

    printf(
        "\n%-12s %-25s %-12s %-15s\n",
        "CODIGO",
        "NOMBRE",
        "COSTO",
        "UNIDAD"
    );

    printf(
        "------------------------------------------------"
        "------------------\n"
    );

    for (i = 0; i < s->totalIngredientes; i++) {

        printf(
            "%-12s %-25s $%-11.4f %-15s\n",
            s->ingredientes[i].codigo,
            s->ingredientes[i].nombre,
            s->ingredientes[i].costoUnitario,
            s->ingredientes[i].unidadMedida
        );
    }
}


void buscarIngredienteMenu(const Sistema *s) {

    char busqueda[MAX_NOMBRE];
    int i;
    int encontrados = 0;

    leerCadena(
        "Codigo o parte del nombre: ",
        busqueda,
        sizeof(busqueda)
    );

    for (i = 0; i < s->totalIngredientes; i++) {

        if (
            strcmp(
                s->ingredientes[i].codigo,
                busqueda
            ) == 0 ||

            contieneTexto(
                s->ingredientes[i].nombre,
                busqueda
            )
        ) {

            printf(
                "%s | %s | $%.4f | %s\n",
                s->ingredientes[i].codigo,
                s->ingredientes[i].nombre,
                s->ingredientes[i].costoUnitario,
                s->ingredientes[i].unidadMedida
            );

            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron ingredientes.\n");
    }
}


void actualizarIngrediente(Sistema *s) {

    char codigo[MAX_CODIGO];
    int indice;
    Ingrediente *ing;

    leerCadena(
        "Codigo del ingrediente: ",
        codigo,
        sizeof(codigo)
    );

    indice = buscarIngrediente(s, codigo);

    if (indice < 0) {
        printf("Ingrediente no encontrado.\n");
        return;
    }

    ing = &s->ingredientes[indice];

    leerCadena(
        "Nuevo nombre: ",
        ing->nombre,
        sizeof(ing->nombre)
    );

    ing->costoUnitario =
        leerFloat(
            "Nuevo costo (>0): ",
            0,
            1000000,
            0
        );

    leerCadena(
        "Nueva unidad: ",
        ing->unidadMedida,
        sizeof(ing->unidadMedida)
    );

    printf("Ingrediente actualizado.\n");
}


void eliminarIngrediente(Sistema *s) {

    char codigo[MAX_CODIGO];
    int indice;
    int i;

    leerCadena(
        "Codigo del ingrediente: ",
        codigo,
        sizeof(codigo)
    );

    indice = buscarIngrediente(s, codigo);

    if (indice < 0) {
        printf("Ingrediente no encontrado.\n");
        return;
    }

    if (ingredienteEnUso(s, codigo)) {

        printf(
            "No se puede eliminar porque esta "
            "asociado a un plato.\n"
        );

        return;
    }

    if (!confirmar("Confirma la eliminacion")) {
        printf("Operacion cancelada.\n");
        return;
    }

    for (i = indice;
         i < s->totalIngredientes - 1;
         i++) {

        s->ingredientes[i] =
            s->ingredientes[i + 1];
    }

    s->totalIngredientes--;

    printf("Ingrediente eliminado.\n");
}


/* GESTION DE PLATOS */

void menuPlatos(Sistema *s) {

    int opcion;

    do {
        printf("\n=== GESTION DE PLATOS ===\n");
        printf("1. Registrar plato\n");
        printf("2. Listar platos\n");
        printf("3. Buscar plato\n");
        printf("4. Actualizar plato\n");
        printf("5. Eliminar plato\n");
        printf("0. Volver\n");

        opcion = leerEntero("Opcion: ", 0, 5);

        switch (opcion) {

            case 1:
                registrarPlato(s);
                pausa();
                break;

            case 2:
                listarPlatos(s);
                pausa();
                break;

            case 3:
                buscarPlatoMenu(s);
                pausa();
                break;

            case 4:
                actualizarPlato(s);
                pausa();
                break;

            case 5:
                eliminarPlato(s);
                pausa();
                break;
        }

    } while (opcion != 0);
}


void registrarPlato(Sistema *s) {

    Plato nuevo;

    if (s->totalPlatos >= MAX_PLATOS) {
        printf("No hay espacio para mas platos.\n");
        return;
    }

    leerCadena(
        "Codigo: ",
        nuevo.codigo,
        sizeof(nuevo.codigo)
    );

    if (buscarPlato(s, nuevo.codigo) >= 0) {
        printf("Ese codigo ya existe.\n");
        return;
    }

    leerCadena(
        "Nombre: ",
        nuevo.nombre,
        sizeof(nuevo.nombre)
    );

    leerCadena(
        "Categoria: ",
        nuevo.categoria,
        sizeof(nuevo.categoria)
    );

    nuevo.impuesto =
        leerFloat(
            "Impuesto (0 a 99.99): ",
            0,
            99.99f,
            1
        );

    nuevo.servicio =
        leerFloat(
            "Servicio (0 a 99.99): ",
            0,
            99.99f,
            1
        );

    nuevo.ganancia =
        leerFloat(
            "Ganancia (0 a 99.99): ",
            0,
            99.99f,
            1
        );

    s->platos[s->totalPlatos] = nuevo;
    s->totalPlatos++;

    printf("Plato registrado.\n");
}


void listarPlatos(const Sistema *s) {

    int i;

    if (s->totalPlatos == 0) {
        printf("No hay platos registrados.\n");
        return;
    }

    printf(
        "\n%-12s %-22s %-15s %-12s %-12s\n",
        "CODIGO",
        "NOMBRE",
        "CATEGORIA",
        "BASE",
        "FINAL"
    );

    printf(
        "------------------------------------------------"
        "--------------------------\n"
    );

    for (i = 0; i < s->totalPlatos; i++) {

        float base;
        float final;

        base =
            calcularCostoIngredientes(
                s,
                s->platos[i].codigo
            );

        final =
            calcularCostoFinal(
                s,
                &s->platos[i]
            );

        printf(
            "%-12s %-22s %-15s $%-11.4f $%-11.4f\n",
            s->platos[i].codigo,
            s->platos[i].nombre,
            s->platos[i].categoria,
            base,
            final
        );
    }
}


void buscarPlatoMenu(const Sistema *s) {

    char busqueda[MAX_NOMBRE];
    int i;
    int encontrados = 0;

    leerCadena(
        "Codigo o parte del nombre: ",
        busqueda,
        sizeof(busqueda)
    );

    for (i = 0; i < s->totalPlatos; i++) {

        if (
            strcmp(
                s->platos[i].codigo,
                busqueda
            ) == 0 ||

            contieneTexto(
                s->platos[i].nombre,
                busqueda
            )
        ) {

            printf(
                "%s | %s | %s | Base: $%.4f | "
                "Final: $%.4f\n",
                s->platos[i].codigo,
                s->platos[i].nombre,
                s->platos[i].categoria,

                calcularCostoIngredientes(
                    s,
                    s->platos[i].codigo
                ),

                calcularCostoFinal(
                    s,
                    &s->platos[i]
                )
            );

            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("No se encontraron platos.\n");
    }
}


void actualizarPlato(Sistema *s) {

    char codigo[MAX_CODIGO];
    int indice;
    Plato *plato;

    leerCadena(
        "Codigo del plato: ",
        codigo,
        sizeof(codigo)
    );

    indice = buscarPlato(s, codigo);

    if (indice < 0) {
        printf("Plato no encontrado.\n");
        return;
    }

    plato = &s->platos[indice];

    leerCadena(
        "Nuevo nombre: ",
        plato->nombre,
        sizeof(plato->nombre)
    );

    leerCadena(
        "Nueva categoria: ",
        plato->categoria,
        sizeof(plato->categoria)
    );

    plato->impuesto =
        leerFloat(
            "Nuevo impuesto: ",
            0,
            99.99f,
            1
        );

    plato->servicio =
        leerFloat(
            "Nuevo servicio: ",
            0,
            99.99f,
            1
        );

    plato->ganancia =
        leerFloat(
            "Nueva ganancia: ",
            0,
            99.99f,
            1
        );

    printf("Plato actualizado.\n");
}


void eliminarPlato(Sistema *s) {

    char codigo[MAX_CODIGO];
    int indice;
    int i;
    int j;

    leerCadena(
        "Codigo del plato: ",
        codigo,
        sizeof(codigo)
    );

    indice = buscarPlato(s, codigo);

    if (indice < 0) {
        printf("Plato no encontrado.\n");
        return;
    }

    if (!confirmar(
            "Se eliminaran tambien sus relaciones. Confirma"
        )) {

        printf("Operacion cancelada.\n");
        return;
    }

    i = 0;

    while (i < s->totalRelaciones) {

        if (
            strcmp(
                s->relaciones[i].codigoPlato,
                codigo
            ) == 0
        ) {

            for (j = i;
                 j < s->totalRelaciones - 1;
                 j++) {

                s->relaciones[j] =
                    s->relaciones[j + 1];
            }

            s->totalRelaciones--;
        }

        else {
            i++;
        }
    }

    for (i = indice;
         i < s->totalPlatos - 1;
         i++) {

        s->platos[i] =
            s->platos[i + 1];
    }

    s->totalPlatos--;

    printf("Plato eliminado.\n");
}


/* INGREDIENTES POR PLATO */

void menuRelaciones(Sistema *s) {

    int opcion;

    do {
        printf("\n=== INGREDIENTES POR PLATO ===\n");
        printf("1. Asociar ingrediente a plato\n");
        printf("2. Listar ingredientes de un plato\n");
        printf("3. Actualizar cantidad usada\n");
        printf("4. Quitar ingrediente de un plato\n");
        printf("0. Volver\n");

        opcion = leerEntero("Opcion: ", 0, 4);

        switch (opcion) {

            case 1:
                asociarIngrediente(s);
                pausa();
                break;

            case 2:
                listarIngredientesPlato(s);
                pausa();
                break;

            case 3:
                actualizarCantidad(s);
                pausa();
                break;

            case 4:
                eliminarRelacion(s);
                pausa();
                break;
        }

    } while (opcion != 0);
}


void asociarIngrediente(Sistema *s) {

    PlatoIngrediente nueva;

    if (s->totalRelaciones >= MAX_RELACIONES) {
        printf("No hay espacio para mas relaciones.\n");
        return;
    }

    leerCadena(
        "Codigo del plato: ",
        nueva.codigoPlato,
        sizeof(nueva.codigoPlato)
    );

    if (buscarPlato(s, nueva.codigoPlato) < 0) {
        printf("El plato no existe.\n");
        return;
    }

    leerCadena(
        "Codigo del ingrediente: ",
        nueva.codigoIngrediente,
        sizeof(nueva.codigoIngrediente)
    );

    if (
        buscarIngrediente(
            s,
            nueva.codigoIngrediente
        ) < 0
    ) {

        printf("El ingrediente no existe.\n");
        return;
    }

    if (
        buscarRelacion(
            s,
            nueva.codigoPlato,
            nueva.codigoIngrediente
        ) >= 0
    ) {

        printf("La relacion ya existe.\n");
        return;
    }

    nueva.cantidadUsada =
        leerFloat(
            "Cantidad usada (>0): ",
            0,
            1000000,
            0
        );

    s->relaciones[s->totalRelaciones] = nueva;
    s->totalRelaciones++;

    printf("Ingrediente asociado al plato.\n");
}


void listarIngredientesPlato(const Sistema *s) {

    char codigoPlato[MAX_CODIGO];
    int i;
    int encontrados = 0;
    float total = 0;

    leerCadena(
        "Codigo del plato: ",
        codigoPlato,
        sizeof(codigoPlato)
    );

    if (buscarPlato(s, codigoPlato) < 0) {
        printf("El plato no existe.\n");
        return;
    }

    printf(
        "\n%-12s %-25s %-12s %-12s\n",
        "COD. ING.",
        "INGREDIENTE",
        "CANTIDAD",
        "PARCIAL"
    );

    printf(
        "------------------------------------------------"
        "----------------\n"
    );

    for (i = 0; i < s->totalRelaciones; i++) {

        if (
            strcmp(
                s->relaciones[i].codigoPlato,
                codigoPlato
            ) == 0
        ) {

            int indiceIngrediente;
            float parcial;

            indiceIngrediente =
                buscarIngrediente(
                    s,
                    s->relaciones[i].codigoIngrediente
                );

            if (indiceIngrediente >= 0) {

                parcial =
                    s->ingredientes[indiceIngrediente]
                        .costoUnitario *

                    s->relaciones[i]
                        .cantidadUsada;

                printf(
                    "%-12s %-25s %-12.4f $%-11.4f\n",
                    s->ingredientes[indiceIngrediente]
                        .codigo,

                    s->ingredientes[indiceIngrediente]
                        .nombre,

                    s->relaciones[i]
                        .cantidadUsada,

                    parcial
                );

                total += parcial;
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {

        printf(
            "El plato no tiene ingredientes asociados.\n"
        );
    }

    else {
        printf(
            "Costo total de ingredientes: $%.4f\n",
            total
        );
    }
}


void actualizarCantidad(Sistema *s) {

    char codPlato[MAX_CODIGO];
    char codIngrediente[MAX_CODIGO];
    int indice;

    leerCadena(
        "Codigo del plato: ",
        codPlato,
        sizeof(codPlato)
    );

    leerCadena(
        "Codigo del ingrediente: ",
        codIngrediente,
        sizeof(codIngrediente)
    );

    indice =
        buscarRelacion(
            s,
            codPlato,
            codIngrediente
        );

    if (indice < 0) {
        printf("La relacion no existe.\n");
        return;
    }

    s->relaciones[indice].cantidadUsada =
        leerFloat(
            "Nueva cantidad (>0): ",
            0,
            1000000,
            0
        );

    printf("Cantidad actualizada.\n");
}


void eliminarRelacion(Sistema *s) {

    char codPlato[MAX_CODIGO];
    char codIngrediente[MAX_CODIGO];
    int indice;
    int i;

    leerCadena(
        "Codigo del plato: ",
        codPlato,
        sizeof(codPlato)
    );

    leerCadena(
        "Codigo del ingrediente: ",
        codIngrediente,
        sizeof(codIngrediente)
    );

    indice =
        buscarRelacion(
            s,
            codPlato,
            codIngrediente
        );

    if (indice < 0) {
        printf("La relacion no existe.\n");
        return;
    }

    if (!confirmar(
            "Confirma quitar el ingrediente del plato"
        )) {

        printf("Operacion cancelada.\n");
        return;
    }

    for (i = indice;
         i < s->totalRelaciones - 1;
         i++) {

        s->relaciones[i] =
            s->relaciones[i + 1];
    }

    s->totalRelaciones--;

    printf("Relacion eliminada.\n");
}


/* CALCULOS */

float calcularCostoIngredientes(
    const Sistema *s,
    const char *codigoPlato
) {

    float total = 0;
    int i;

    for (i = 0; i < s->totalRelaciones; i++) {

        if (
            strcmp(
                s->relaciones[i].codigoPlato,
                codigoPlato
            ) == 0
        ) {

            int indiceIngrediente;

            indiceIngrediente =
                buscarIngrediente(
                    s,
                    s->relaciones[i]
                        .codigoIngrediente
                );

            if (indiceIngrediente >= 0) {

                total +=
                    s->ingredientes[indiceIngrediente]
                        .costoUnitario *

                    s->relaciones[i]
                        .cantidadUsada;
            }
        }
    }

    return total;
}


float calcularCostoFinal(
    const Sistema *s,
    const Plato *plato
) {

    float base;
    float impuesto;
    float servicio;
    float ganancia;

    base =
        calcularCostoIngredientes(
            s,
            plato->codigo
        );

    impuesto =
        base *
        plato->impuesto /
        100.0f;

    servicio =
        base *
        plato->servicio /
        100.0f;

    ganancia =
        base *
        plato->ganancia /
        100.0f;

    return base +
           impuesto +
           servicio +
           ganancia;
}


/* REPORTES */

void menuReportes(const Sistema *s) {

    int opcion;

    do {
        printf("\n=== CALCULO Y REPORTES ===\n");
        printf("1. Detalle de costo de un plato\n");
        printf("2. Reporte general\n");
        printf("0. Volver\n");

        opcion = leerEntero("Opcion: ", 0, 2);

        switch (opcion) {

            case 1:
                mostrarDetalleCosto(s);
                pausa();
                break;

            case 2:
                reporteGeneral(s);
                pausa();
                break;
        }

    } while (opcion != 0);
}


void mostrarDetalleCosto(const Sistema *s) {

    char codigo[MAX_CODIGO];
    int indice;

    float base;
    float impuesto;
    float servicio;
    float ganancia;
    float final;

    const Plato *plato;

    leerCadena(
        "Codigo del plato: ",
        codigo,
        sizeof(codigo)
    );

    indice = buscarPlato(s, codigo);

    if (indice < 0) {
        printf("Plato no encontrado.\n");
        return;
    }

    plato = &s->platos[indice];

    base =
        calcularCostoIngredientes(
            s,
            plato->codigo
        );

    impuesto =
        base *
        plato->impuesto /
        100.0f;

    servicio =
        base *
        plato->servicio /
        100.0f;

    ganancia =
        base *
        plato->ganancia /
        100.0f;

    final =
        base +
        impuesto +
        servicio +
        ganancia;

    printf(
        "\nPlato: %s - %s\n",
        plato->codigo,
        plato->nombre
    );

    printf(
        "Costo ingredientes: $%.4f\n",
        base
    );

    printf(
        "Impuesto (%.2f%%): $%.4f\n",
        plato->impuesto,
        impuesto
    );

    printf(
        "Servicio (%.2f%%): $%.4f\n",
        plato->servicio,
        servicio
    );

    printf(
        "Ganancia (%.2f%%): $%.4f\n",
        plato->ganancia,
        ganancia
    );

    printf(
        "COSTO FINAL: $%.4f\n",
        final
    );
}


void reporteGeneral(const Sistema *s) {

    int i;

    listarPlatos(s);

    printf(
        "\nTotal ingredientes: %d\n",
        s->totalIngredientes
    );

    printf(
        "Total platos: %d\n",
        s->totalPlatos
    );

    printf(
        "Total relaciones: %d\n",
        s->totalRelaciones
    );

    for (i = 0; i < s->totalPlatos; i++) {

        if (
            calcularCostoIngredientes(
                s,
                s->platos[i].codigo
            ) == 0
        ) {

            printf(
                "ALERTA: El plato %s no tiene "
                "ingredientes o su costo es 0.\n",
                s->platos[i].codigo
            );
        }
    }
}