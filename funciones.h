/**************************************************************
 * TPI PE 2026 - Sistema de Gestión de Alquiler de Vehículos
 * Archivo: funciones.h
 **************************************************************/

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stddef.h>
#include <math.h>
#include <errno.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include "structs.h"

#define ARCH_CLIENTES       "clientes.dat"
#define ARCH_VEHICULOS      "vehiculos.dat"
#define ARCH_ALQUILERES     "alquileres.dat"
#define ARCH_MOVIMIENTOS    "movimientos.dat"

#define EPSILON             0.005f

/* ==============================================================
 * PROTOTIPOS DE FUNCIONES MODULARES
 * ============================================================== */

/* Interfaz y consola */
void limpiar(void);
void gotoxy(int x, int y);
void setColor(int fg, int bg);
void pausarConsola(void);
void dibujarMarco(int x1, int y1, int x2, int y2, int colorBorde, int colorFondo);
void uiTransicionEntrada(const char *titulo);
void pantallaCarga(void);
void animTransicionMenu(const char *mensaje);
void pantallaSalida(void);

/* Lecturas y validaciones (Retornan 1 si fue exitoso, 0 si canceló con '0') */
int leerLinea(char *buffer, size_t size);
void normalizarTexto(char *s);
int validarINT(const char *msg);
long validarLONG(const char *msg);
float validarFLOAT(const char *msg);
int validarTXT(const char *msg, char *buffer, size_t len);
int validarAlfanumericoTXT(const char *msg, char *buffer, size_t len);
int validarCorreo(const char *mensaje, char *destino, size_t capacidad);
int pedirTelefono(const char *mensaje, char *destino, size_t capacidad);
int validarPatente(const char *origen, char *destinoNormalizado);
int generarCOD(const char *archivo, size_t tamRegistro, size_t offsetCodigo);

/* Fechas y tiempo */
int esBisiesto(int anio);
int diasDelMes(int mes, int anio);
int validarFechaStruct(Fecha f);
Fecha obtenerFechaActual(void);
Fecha leerFechaInicio(void);
Fecha sumarDiasAFecha(Fecha f, int dias);
int compararFechas(Fecha f1, Fecha f2);
long fechaADiasRata(Fecha f);
int diferenciaDias(Fecha fMayor, Fecha fMenor);
long convertirFechaAClave(Fecha f);

/* Módulo Clientes */
int buscarClientePorDNI(long dni, Cliente *cpunt);
int buscarCualquierClientePorDNI(long dni, Cliente *cpunt);
int registrarClienteEnArchivo(Cliente *c);
int modificarEstadoCliente(long dni, short nuevoEstado);
int clienteTieneAlquileresPendientes(long dni);
void listarClientesActivosCorto(void);
void listarTodosClientesCorto(void);
void registrarCliente(void);
void modificarClientes(void);
void administrarClientesUI(void);
void consultarClienteUI(void);
void menuClientes(void);

/* Módulo Vehículos */
const char *obtenerNombreEstadoVehiculo(int estado);
void imprimirFilaVehiculo(const Vehiculo *v);
int buscarVehiculoPorPatente(const char *patente, Vehiculo *vOut, long *posByteOut);
int actualizarEstadoVehiculo(long posByte, int nuevoEstado);
void listarVehiculosDisponiblesCorto(void);
void listarFlotaCompleta(void);
void listarAlquileresActivosConDetalle(void);
void listarAlquileresConDeuda(void);
int vehiculoTieneAlquilerActivo(const char *patente);
void registrarVehiculo(void);
void modificarVehiculo(void);
void buscarVehiculo(void);
void listarVehiculosDisponibles(void);
void listarVehiculosAlquilados(void);
void menuVehiculos(void);

/* Módulo Alquileres */
int guardarAlquiler(const Alquiler *alq);
int buscarAlquilerPorDniYNumero(long dni, int numAlquiler, Alquiler *alqOut, long *posByteOut);
int buscarAlquilerPorNumero(int numAlquiler, Alquiler *alqOut, long *posByteOut);
int sobreescribirAlquiler(long posByte, const Alquiler *alq);
void mostrarFilaAlquiler(const Alquiler *a);
void imprimirEncabezadoConsultaAlquileres(void);
void registrarAlquiler(void);
void modificarAlquiler(void);
void registrarDevolucion(void);
void listarAlquileresActivos(void);
void listarTodosAlquileres(void);
void consultarAlquileresPorRangoFechas(void);
void consultarAlquileresPorCliente(void);
void consultarAlquileresPorVehiculo(void);
void consultarAlquileresPorEstado(void);
void consultarAlquileresPorImporte(void);
void menuAlquileres(void);
void menuDevoluciones(void);
void menuConsultasAlquileres(void);

/* Módulo Movimientos y Pagos */
const char *nombreTipo(int tipo);
void imprimirEncabezadoMov(void);
void imprimirMovimiento(const Movimiento *m);
void actualizarArchivoCliente(const Movimiento *m);
int registrarMovimiento(int numeroAlquiler, int tipo, float monto, Fecha fecha,
                        long dni, const char *patente);
void registrarPago(void);
void mostrarMovimientosCliente(long dni);
void consultarMovimientos(void);
void listarPagos(void);
void listarClientesConDeuda(void);
void estadisticasEmpresa(void);
void menuPagos(void);

/* Menús Generales */
void menuConsultasYReportes(void);
void menuPrincipal(void);

/* ==============================================================
 * IMPLEMENTACIÓN: INTERFAZ Y CONSOLA (WINDOWS)
 * ============================================================== */

void limpiar(void) {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
    fflush(stdout);
#endif
}

void gotoxy(int x, int y) {
#ifdef _WIN32
    COORD c;
    c.X = (SHORT)(x < 0 ? 0 : x);
    c.Y = (SHORT)(y < 0 ? 0 : y);
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
#else
    printf("\033[%d;%dH", y, x);
#endif
}

void setColor(int fg, int bg) {
#ifdef _WIN32
    WORD attr = (WORD)(((bg & 0x0F) << 4) | (fg & 0x0F));
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
#else
    int f = (fg >= 0 && fg <= 15) ? fg : 7;
    int b = (bg >= 0 && bg <= 15) ? bg : 0;
    printf("\033[%d;%dm", (f < 8 ? 30 : 90) + (f % 8), (b < 8 ? 40 : 100) + (b % 8));
#endif
}

void pausarConsola(void) {
    int ch; 
    puts("\nPresione ENTER para continuar...");
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

void dibujarMarco(int x1, int y1, int x2, int y2, int colorBorde, int colorFondo) {
    const unsigned char TL = 201; /* ╔ */
    const unsigned char TR = 187; /* ╗ */
    const unsigned char BL = 200; /* ╚ */
    const unsigned char BR = 188; /* ╝ */
    const unsigned char H  = 205; /* ═ */
    const unsigned char V  = 186; /* ║ */

    int x, y, t;
    int w, h;

    if (x2 < x1) { t = x1; x1 = x2; x2 = t; } 
    if (y2 < y1) { t = y1; y1 = y2; y2 = t; }

    w = x2 - x1;
    h = y2 - y1;

    if (w < 2 || h < 2) {
        setColor(colorBorde, colorFondo);
        for (y = y1; y <= y2; y++) {
            for (x = x1; x <= x2; x++) {
                gotoxy(x, y); 
                putchar(' ');
            }
        }
    } else {
        setColor(colorBorde, colorFondo);
        gotoxy(x1, y1); 
        putchar(TL);
        for (x = x1 + 1; x < x2; x++) { 
            putchar(H); 
        }
        putchar(TR);

        for (y = y1 + 1; y < y2; y++) { 
            gotoxy(x1, y); 
            putchar(V); 
            for (x = x1 + 1; x < x2; x++) { 
                putchar(' '); 
            }
            putchar(V); 
        }

        gotoxy(x1, y2); 
        putchar(BL); 
        for (x = x1 + 1; x < x2; x++) { 
            putchar(H); 
        }
        putchar(BR);
        setColor(15, 0);
    }
}

void animTransicionMenu(const char *mensaje) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int anchoCon, centroX, centroY;
    int cajaAncho, cajaAlto = 7;
    int x1, x2, y1, y2;
    int msgX, msgY;
    int i, pasos = 28;       
    const char spinner[] = "|/-\\";
    int len = (int)strlen(mensaje);

    cajaAncho = len + 12;
    if (cajaAncho < 42) cajaAncho = 42;

    limpiar();
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    anchoCon = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    centroX  = anchoCon / 2;
    centroY  = 10;

    x1 = centroX - (cajaAncho / 2);
    x2 = centroX + (cajaAncho / 2);
    y1 = centroY - (cajaAlto / 2);
    y2 = centroY + (cajaAlto / 2);

    dibujarMarco(x1, y1, x2, y2, 14, 0);
    setColor(14, 0);
    msgX = centroX - (len / 2);
    msgY = y1 + 2;
    gotoxy(msgX, msgY);
    printf("%s", mensaje);

    setColor(11, 0);
    for (i = 0; i < pasos; i++) {
        gotoxy(msgX - 2, msgY);
        putchar(spinner[i % 4]);

        gotoxy(msgX + len, msgY);
        switch (i % 4) {
            case 0: printf("    "); break;
            case 1: printf(".   "); break;
            case 2: printf("..  "); break;
            default:printf("... "); break;
        }

        {
            int w = cajaAncho - 6;
            int baseX = x1 + 3;
            int yBar  = y1 + 4;
            int pos   = (i % (w - 3));
            int j;

            gotoxy(baseX, yBar);
            for (j = 0; j < w; j++) putchar(' ');
            gotoxy(baseX + pos, yBar);   putchar(219);
            gotoxy(baseX + pos + 1, yBar); if (pos + 1 < w) putchar(219);
            gotoxy(baseX + pos + 2, yBar); if (pos + 2 < w) putchar(219);
        }

        Sleep(50);
    }

    setColor(10, 0);
    gotoxy(centroX - 9, y2); 
    printf("Listo. Continuando...");
    Sleep(200);
    setColor(15, 0);
    limpiar();
#else
    limpiar();
    printf("\n%s...\n", mensaje);
#endif
}

void uiTransicionEntrada(const char *titulo) {
    char msg[128];
    snprintf(msg, sizeof(msg), "Ingresando a %s", titulo);
    animTransicionMenu(msg);
}

void pantallaCarga(void) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int ancho, centroX, centroY;
    const int MARCO_ANCHO = 60;
    const int MARCO_ALTO  = 15;
    const int BARRA_LEN   = 20;

    int x1, x2, y1, y2;
    int barraX, tituloX, msgX;
    int i;

    limpiar();

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    ancho   = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    centroX = ancho / 2;
    centroY = 10;

    x1 = centroX - (MARCO_ANCHO / 2);
    x2 = centroX + (MARCO_ANCHO / 2);
    y1 = centroY - (MARCO_ALTO / 2);
    y2 = centroY + (MARCO_ALTO / 2);

    barraX  = centroX - (BARRA_LEN / 2) - 1;  
    tituloX = centroX - 10;                  
    msgX    = centroX - 16;                  

    system("color 5E");
    dibujarMarco(x1, y1, x2, y2, 14, 5);

    setColor(14, 5);
    gotoxy(tituloX, y1 + 3);
    printf("CARGANDO SISTEMA...");

    gotoxy(barraX, y1 + 6); 
    putchar('[');
    for (i = 0; i < BARRA_LEN; i++) putchar(' ');
    putchar(']');

    for (i = 0; i <= BARRA_LEN; i++) {
        gotoxy(barraX + 1 + i, y1 + 6);
        if (i < BARRA_LEN) putchar(219); 
        fflush(stdout);

        gotoxy(barraX + BARRA_LEN + 3, y1 + 6);
        printf("%3d%%", (i * 100) / BARRA_LEN);

        gotoxy(msgX, y1 + 8);
        if      (i == 0)                printf("Inicializando modulos...      ");
        else if (i == BARRA_LEN * 1 / 4) printf("Cargando datos de usuario...   ");
        else if (i == BARRA_LEN * 2 / 4) printf("Verificando archivos...        ");
        else if (i == BARRA_LEN * 3 / 4) printf("Preparando interfaz...         ");
        else if (i == BARRA_LEN)         printf("Listo para comenzar!           ");

        Sleep(80);
    }

    limpiar();
    dibujarMarco(centroX - 30, centroY - 3, centroX + 30, centroY + 3, 10, 0);
    setColor(10, 0);
    gotoxy(centroX - 17, centroY);
    printf("BIENVENIDO AL SISTEMA DE GESTION");
    Sleep(1200);

    setColor(15, 0);
    system("color 07");
    limpiar();
#else
    limpiar();
    puts("=================================================");
    puts("        BIENVENIDO AL SISTEMA DE GESTION         ");
    puts("=================================================");
#endif
}

void pantallaSalida(void) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int anchoCon, centroX, centroY;
    int cajaAncho, cajaAlto = 9;
    int x1, x2, y1, y2;
    int i, pasos = 60;              
    const char spinner[] = "|/-\\";
    const char *titulo   = "Saliendo del sistema";
    const char *sub      = "Gracias por usar el gestor";

    limpiar();
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    anchoCon = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    centroX  = anchoCon / 2;
    centroY  = 10;

    {
        int need = (int)strlen(sub) + 12;
        cajaAncho = (need < 50) ? 50 : need;
    }

    x1 = centroX - (cajaAncho / 2);
    x2 = centroX + (cajaAncho / 2);
    y1 = centroY - (cajaAlto / 2);
    y2 = centroY + (cajaAlto / 2);

    dibujarMarco(x1, y1, x2, y2, 14, 0);

    setColor(14, 0);
    gotoxy(centroX - (int)strlen(titulo) / 2, y1 + 2);
    printf("%s", titulo);

    setColor(11, 0);
    gotoxy(centroX - (int)strlen(sub) / 2, y1 + 3);
    printf("%s", sub);

    {
        int baseX = x1 + 3;
        int w     = cajaAncho - 6;
        int yBar  = y1 + 5;

        setColor(8, 0);
        gotoxy(baseX, yBar);
        for (i = 0; i < w; i++) putchar(176); 

        setColor(10, 0);
        for (i = 0; i < pasos; i++) {
            int fill = (i * w) / (pasos - 1);
            int k;

            gotoxy(baseX, yBar);
            for (k = 0; k < fill; k++) putchar(219);  

            setColor(14, 0);
            gotoxy(baseX + w + 2, yBar);
            putchar(spinner[i % 4]);

            gotoxy(centroX + (int)strlen(titulo) / 2 + 1, y1 + 2);
            switch (i % 4) {
                case 0: printf("    "); break;
                case 1: printf(".   "); break;
                case 2: printf("..  "); break;
                default:printf("... "); break;
            }

            setColor(10, 0);
            Sleep(25);
        }
    }

    setColor(10, 0);
    gotoxy(centroX - 8, y2);
    printf("Cerrando... OK");

    Sleep(300);
    setColor(15, 0);
    limpiar();
#else
    limpiar();
    puts("Cerrando sistema... Hasta pronto.");
#endif
}

/* ==============================================================
 * IMPLEMENTACIÓN: LECTURA Y VALIDACIONES 
 * ============================================================== */

int leerLinea(char *buffer, size_t size) {
    int exito = 0;
    int c; 
    size_t n;

    if (buffer != NULL && size >= 2 && size <= (size_t)INT_MAX) {
        if (fgets(buffer, (int)size, stdin) != NULL) { 
            n = strlen(buffer);
            if (n > 0 && buffer[n - 1] == '\n') { 
                buffer[--n] = '\0'; 
                exito = 1;
            } else if (!feof(stdin)) { 
                while ((c = getchar()) != '\n' && c != EOF) { } 
                buffer[0] = '\0'; 
                puts("Error: Entrada demasiado larga; vuelva a intentarlo."); 
            } else {
                exito = 1;
            }
            if (n > 0 && buffer[n - 1] == '\r') {
                buffer[n - 1] = '\0';
            }
        } else {
            buffer[0] = '\0';
        }
    }
    return exito;
}

void normalizarTexto(char *s) {
    int i;
    if (s != NULL) {
        for (i = 0; s[i] != '\0'; i++) {
            s[i] = (char)toupper((unsigned char)s[i]);
        }
    }
}

int validarINT(const char *msg) {
    char buffer[100], *fin;
    long valor = 0;
    int listo = 0;

    while (!listo) {
        printf("%s", msg);
        if (!leerLinea(buffer, sizeof(buffer))) { 
            puts("Fin de entrada detectado."); 
            valor = 0;
            listo = 1;
        } else if (buffer[0] == ' ') {
            puts("Error: No se permiten espacios al inicio.");
        } else if (buffer[0] == '\0') { 
            puts("Error: El campo no puede quedar vacio."); 
        } else {
            errno = 0; 
            valor = strtol(buffer, &fin, 10);
            
            if (errno == ERANGE || fin == buffer || *fin != '\0') {
                puts("Error: Ingrese solo caracteres numericos enteros (sin espacios ni simbolos)."); 
            } else if (valor < 0 || valor > INT_MAX) {
                puts("Error: Ingrese un entero positivo valido."); 
            } else {
                listo = 1;
            }
        }
    }
    return (int)valor;
}

long validarLONG(const char *msg) {
    char buffer[100], *fin;
    long valor = 0;
    int listo = 0;

    while (!listo) {
        printf("%s", msg);
        if (!leerLinea(buffer, sizeof(buffer))) { 
            puts("Fin de entrada detectado."); 
            valor = 0L;
            listo = 1;
        } else if (buffer[0] == ' ') {
            puts("Error: No se permiten espacios al inicio.");
        } else if (buffer[0] == '\0') { 
            puts("Error: El campo no puede quedar vacio."); 
        } else {
            errno = 0; 
            valor = strtol(buffer, &fin, 10);
            
            if (errno == ERANGE || fin == buffer || *fin != '\0') {
                puts("Error: No se permiten letras, espacios o caracteres especiales."); 
            } else if (valor < 0) {
                puts("Error: No se permiten numeros negativos."); 
            } else {
                listo = 1;
            }
        }
    }
    return valor;
}

float validarFLOAT(const char *msg) {
    char buffer[100], *fin;
    float valor = 0.0f;
    int listo = 0;

    while (!listo) {
        printf("%s", msg);
        if (!leerLinea(buffer, sizeof(buffer))) { 
            puts("Fin de entrada detectado."); 
            valor = 0.0f;
            listo = 1;
        } else if (buffer[0] == ' ') {
            puts("Error: No se permiten espacios al inicio.");
        } else if (buffer[0] == '\0') { 
            puts("Error: El campo no puede quedar vacio."); 
        } else {
            char *q; 
            for (q = buffer; *q; ++q) {
                if (*q == ',') *q = '.';
            }

            errno = 0; 
            valor = strtof(buffer, &fin);
            
            if (errno == ERANGE || fin == buffer || *fin != '\0' || !isfinite(valor)) {
                puts("Error: Ingrese un valor numerico valido (sin espacios intermedios ni letras)."); 
            } else if (valor < 0.0f) {
                puts("Error: No se permiten importes negativos."); 
            } else {
                listo = 1;
            }
        }
    }
    return valor;
}


int validarTXT(const char *msg, char *buffer, size_t len) {
    int listo = 0;
    int exito = 0;

    if (buffer != NULL && len >= 2) {
        while (!listo) {
            size_t largo, i;
            int valido = 1;
            int tieneLetra = 0;

            printf("%s (0 para cancelar): ", msg);
            if (!leerLinea(buffer, len)) { 
                buffer[0] = '\0'; 
                exito = 0;
                listo = 1;
            } else if (strcmp(buffer, "0") == 0) { 
                buffer[0] = '\0'; 
                puts("Operacion cancelada."); 
                exito = 0;
                listo = 1;
            } else {
                largo = strlen(buffer);
                if (largo == 0) {
                    puts("ERROR! No puede estar vacio.");
                } else if (buffer[0] == ' ') {
                    puts("ERROR! No puede comenzar con espacios en blanco.");
                } else if (buffer[largo - 1] == ' ') {
                    puts("ERROR! No puede terminar con espacios en blanco.");
                } else {
                    for (i = 0; i < largo; i++) {
                        unsigned char c = (unsigned char)buffer[i];
                        if (isalpha(c)) {
                            tieneLetra = 1;
                        } else if (c != ' ') {
                            printf("ERROR! Solo se permiten letras (A-Z) y espacios. Caracter no valido detectado: '%c'\n", c);
                            valido = 0;
                            break;
                        }
                    }

                    if (valido && !tieneLetra) {
                        puts("ERROR! Debe contener al menos una letra real.");
                        valido = 0;
                    }

                    if (valido) {
                        normalizarTexto(buffer);
                        exito = 1;
                        listo = 1;
                    }
                }
            }
        }
    }
    return exito;
}

int validarAlfanumericoTXT(const char *msg, char *buffer, size_t len) {
    int listo = 0;
    int exito = 0;

    if (buffer != NULL && len >= 2) {
        while (!listo) {
            size_t largo, i;
            int valido = 1;
            int tieneCaracterUtil = 0;

            printf("%s (0 para cancelar): ", msg);
            if (!leerLinea(buffer, len)) { 
                buffer[0] = '\0'; 
                exito = 0;
                listo = 1;
            } else if (strcmp(buffer, "0") == 0) { 
                buffer[0] = '\0'; 
                puts("Operacion cancelada."); 
                exito = 0;
                listo = 1;
            } else {
                largo = strlen(buffer);
                if (largo == 0) {
                    puts("ERROR! No puede quedar vacio.");
                } else if (buffer[0] == ' ') {
                    puts("ERROR! No puede comenzar con espacios en blanco.");
                } else if (buffer[largo - 1] == ' ') {
                    puts("ERROR! No puede terminar con espacios en blanco.");
                } else {
                    for (i = 0; i < largo; i++) {
                        unsigned char c = (unsigned char)buffer[i];
                        if (isalnum(c)) {
                            tieneCaracterUtil = 1;
                        } else if (c != ' ' && c != '.' && c != '-') {
                            printf("ERROR! Caracter especial no permitido: '%c'\n", c);
                            valido = 0;
                            break;
                        }
                    }

                    if (valido && !tieneCaracterUtil) {
                        puts("ERROR! Debe ingresar letras o numeros validos.");
                        valido = 0;
                    }

                    if (valido) {
                        normalizarTexto(buffer);
                        exito = 1;
                        listo = 1;
                    }
                }
            }
        }
    }
    return exito;
}

int validarCorreo(const char *mensaje, char *destino, size_t capacidad) {
    char buffer[256], dominio[64]; 
    const char *dominios[] = {"@gmail.com", "@outlook.com", "@hotmail.com", "@yahoo.com", "@icloud.com", "@live.com"};
    size_t i; 
    int ok = 0;
    int listo = 0;

    if (mensaje != NULL && destino != NULL && capacidad >= 2) {
        while (!listo) {
            printf("%s (0 para cancelar): ", mensaje);
            if (!leerLinea(buffer, sizeof(buffer))) { 
                ok = 0;
                listo = 1;
            } else if (strcmp(buffer, "0") == 0) { 
                puts("Operacion cancelada."); 
                ok = 0;
                listo = 1;
            } else if (buffer[0] == ' ') {
                puts("Error: El correo no puede iniciar con espacios.");
            } else {
                size_t k; 
                int blanco = 0; 
                for (k = 0; buffer[k]; ++k) {
                    if (isspace((unsigned char)buffer[k])) blanco = 1;
                }

                if (!buffer[0] || strlen(buffer) >= capacidad || blanco) { 
                    puts("Error: El correo no puede estar vacio, ser excesivo ni contener espacios en ninguna parte."); 
                } else {
                    char *at = strchr(buffer, '@'); 
                    if (!at || at == buffer || strchr(at + 1, '@')) { 
                        puts("Error: Formato de correo invalido. Requiere nombre de usuario antes del '@'."); 
                    } else if (strlen(at) >= sizeof(dominio)) { 
                        puts("Error: Dominio demasiado extenso."); 
                    } else {
                        for (i = 0; at[i]; ++i) { 
                            dominio[i] = (char)tolower((unsigned char)at[i]); 
                        }
                        dominio[i] = '\0';
                        
                        ok = 0; 
                        for (i = 0; i < sizeof(dominios) / sizeof(dominios[0]); ++i) {
                            if (strcmp(dominio, dominios[i]) == 0) ok = 1;
                        }

                        if (!ok) { 
                            puts("Error: Dominio no reconocido. Use dominios estandar (@gmail.com, @hotmail.com, @outlook.com, etc.)."); 
                        } else {
                            memcpy(destino, buffer, strlen(buffer) + 1); 
                            listo = 1;
                        }
                    }
                }
            }
        }
    }
    return ok;
}

int pedirTelefono(const char *mensaje, char *destino, size_t capacidad) {
    char buffer[64]; 
    size_t i, n; 
    int soloNumeros;
    int listo = 0;
    int exito = 0;

    (void)mensaje;

    if (destino != NULL && capacidad >= 11) {
        while (!listo) {
            printf("telefono[Ingresa los 10 digitos de tu numero completo (codigo de area + numero local). Sin el 0 inicial y sin el 15. Ejemplo: 3764123456.]: ");
            if (!leerLinea(buffer, sizeof(buffer))) { 
                exito = 0;
                listo = 1;
            } else if (strcmp(buffer, "0") == 0) { 
                puts("Operacion cancelada."); 
                exito = 0;
                listo = 1;
            } else {
                n = strlen(buffer);
                if (n != 10) {
                    printf("Error: El telefono debe contener exactamente 10 digitos (ingreso %u digitos).\n", (unsigned int)n);
                } else {
                    soloNumeros = 1;
                    for (i = 0; i < 10; ++i) {
                        if (!isdigit((unsigned char)buffer[i])) {
                            printf("Error: Solo se permiten numeros, sin espacios ni signos. Caracter invalido: '%c'\n", buffer[i]);
                            soloNumeros = 0;
                            break;
                        }
                    }

                    if (soloNumeros) {
                        memcpy(destino, buffer, 11);
                        exito = 1;
                        listo = 1;
                    }
                }
            }
        }
    }
    return exito;
}

int validarPatente(const char *origen, char *destinoNormalizado) {
    char limpia[8]; 
    size_t i;
    int esValida = 0;

    if (origen != NULL && destinoNormalizado != NULL && strlen(origen) == 7) {
        if (strchr(origen, ' ') == NULL && strchr(origen, '\t') == NULL) {
            int tieneEspacio = 0;
            for (i = 0; origen[i] != '\0'; ++i) {
                if (isspace((unsigned char)origen[i])) {
                    tieneEspacio = 1;
                    break;
                }
            }

            if (!tieneEspacio) {
                for (i = 0; i < 7; ++i) {
                    limpia[i] = (char)toupper((unsigned char)origen[i]);
                }
                limpia[7] = '\0';
                
                if (isalpha((unsigned char)limpia[0]) && isalpha((unsigned char)limpia[1]) &&
                    isdigit((unsigned char)limpia[2]) && isdigit((unsigned char)limpia[3]) &&
                    isdigit((unsigned char)limpia[4]) && isalpha((unsigned char)limpia[5]) &&
                    isalpha((unsigned char)limpia[6])) {
                    memcpy(destinoNormalizado, limpia, sizeof(limpia));
                    esValida = 1;
                }
            }
        }
    }
    return esValida;
}

int generarCOD(const char *archivo, size_t tamRegistro, size_t offsetCodigo) {
    FILE *f; 
    unsigned char *registro; 
    int codigo = 1, ultimo = 0; 
    size_t leidos;

    if (archivo != NULL && tamRegistro > 0 && offsetCodigo <= tamRegistro && sizeof(int) <= tamRegistro - offsetCodigo) {
        registro = (unsigned char*)malloc(tamRegistro); 
        if (registro != NULL) {
            f = fopen(archivo, "rb");
            if (f != NULL) {
                while ((leidos = fread(registro, 1, tamRegistro, f)) == tamRegistro) {
                    memcpy(&ultimo, registro + offsetCodigo, sizeof(ultimo));
                    if (ultimo >= codigo) { 
                        if (ultimo == INT_MAX) { 
                            codigo = 0; 
                            break; 
                        } 
                        codigo = ultimo + 1; 
                    }
                }
                if (leidos != 0 && leidos != tamRegistro) { 
                    puts("Advertencia: Registro final incompleto al generar codigo."); 
                    codigo = 0; 
                }
                if (ferror(f)) codigo = 0;
                fclose(f);
            }
            free(registro);
        } else {
            codigo = 0;
        }
    } else {
        codigo = 0;
    }
    return codigo;
}

/* ==============================================================
 * IMPLEMENTACIÓN: FECHAS Y TIEMPO
 * ============================================================== */

int esBisiesto(int anio) {
    return ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0));
}

int diasDelMes(int mes, int anio) {
    int d = 30;
    int dias[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && esBisiesto(anio)) {
        d = 29;
    } else if (mes >= 1 && mes <= 12) {
        d = dias[mes];
    }
    return d;
}

int validarFechaStruct(Fecha f) {
    int resultado = 0;
    if (f.anio >= 2020 && f.anio <= 2026) {
        if (f.mes >= 1 && f.mes <= 12) {
            if (f.dia >= 1 && f.dia <= diasDelMes(f.mes, f.anio)) {
                resultado = 1;
            }
        }
    }
    return resultado;
}

Fecha obtenerFechaActual(void) {
    Fecha f;
    time_t t = time(NULL);
    struct tm *ahora = localtime(&t);

    if (ahora != NULL) {
        f.dia  = ahora->tm_mday;
        f.mes  = ahora->tm_mon + 1;
        f.anio = ahora->tm_year + 1900;
    } else {
        f.dia = 0; f.mes = 0; f.anio = 0;
    }
    return f;
}

Fecha leerFechaInicio(void) {
    Fecha f; 
    int listo = 0;
    memset(&f, 0, sizeof(f));

    while (!listo) {
        puts("Ingrese fecha (periodo valido 2020 a 2026; 0 para cancelar):");
        f.dia = validarINT("  Dia: "); 
        if (!f.dia) {
            f = (Fecha){0, 0, 0};
            listo = 1;
        } else {
            f.mes = validarINT("  Mes: "); 
            if (!f.mes) {
                f = (Fecha){0, 0, 0};
                listo = 1;
            } else {
                f.anio = validarINT("  Anio: "); 
                if (!f.anio) {
                    f = (Fecha){0, 0, 0};
                    listo = 1;
                } else if (validarFechaStruct(f)) {
                    listo = 1;
                } else {
                    puts("Error: Fecha inexistente o fuera del periodo institucional (2020-2026). Intente nuevamente.");
                }
            }
        }
    }
    return f;
}

Fecha sumarDiasAFecha(Fecha f, int dias) {
    Fecha dev = f;
    dev.dia += dias;

    while (dev.dia > diasDelMes(dev.mes, dev.anio)) {
        dev.dia -= diasDelMes(dev.mes, dev.anio);
        dev.mes++;
        if (dev.mes > 12) {
            dev.mes = 1;
            dev.anio++;
        }
    }
    return dev;
}

int compararFechas(Fecha f1, Fecha f2) {
    int res = 0;
    if (f1.anio != f2.anio) {
        res = (f1.anio < f2.anio) ? -1 : 1;
    } else if (f1.mes != f2.mes) {
        res = (f1.mes < f2.mes) ? -1 : 1;
    } else if (f1.dia != f2.dia) {
        res = (f1.dia < f2.dia) ? -1 : 1;
    }
    return res;
}

long fechaADiasRata(Fecha f) {
    long y = f.anio, m = f.mes, d = f.dia;
    if (m < 3) {
        y--;
        m += 12;
    }
    return 365L * y + y / 4L - y / 100L + y / 400L + (153L * m - 457L) / 5L + d - 306L;
}

int diferenciaDias(Fecha fMayor, Fecha fMenor) {
    long d1 = fechaADiasRata(fMayor);
    long d2 = fechaADiasRata(fMenor);
    long dif = d1 - d2;
    return (dif < 0) ? 0 : (int)dif;
}

long convertirFechaAClave(Fecha f) {
    return (long)f.anio * 10000L + (long)f.mes * 100L + (long)f.dia;
}

/* ==============================================================
 * IMPLEMENTACIÓN: MÓDULO CLIENTES
 * ============================================================== */

int buscarClientePorDNI(long dni, Cliente *cpunt) {
    int encontrado = 0;
    FILE *f;
    Cliente c;

    if (dni > 0) {
        f = fopen(ARCH_CLIENTES, "rb");
        if (f != NULL) {
            while (!encontrado && fread(&c, sizeof(Cliente), 1, f) == 1) {
                if (c.dni == dni && c.estado == 1) {
                    encontrado = 1;
                    if (cpunt != NULL) *cpunt = c;
                }
            }
            fclose(f);
        }
    }
    return encontrado;
}

int buscarCualquierClientePorDNI(long dni, Cliente *cpunt) {
    int encontrado = 0;
    FILE *f;
    Cliente c;

    if (dni > 0) {
        f = fopen(ARCH_CLIENTES, "rb");
        if (f != NULL) {
            while (!encontrado && fread(&c, sizeof(Cliente), 1, f) == 1) {
                if (c.dni == dni) {
                    encontrado = 1;
                    if (cpunt != NULL) *cpunt = c;
                }
            }
            fclose(f);
        }
    }
    return encontrado;
}

int clienteTieneAlquileresPendientes(long dni) {
    FILE *f = fopen(ARCH_ALQUILERES, "rb");
    Alquiler a;
    int tienePendiente = 0;

    if (f != NULL) {
        while (!tienePendiente && fread(&a, sizeof(Alquiler), 1, f) == 1) {
            if (a.dnidelcliente == dni) {
                if (a.estado == 1) {
                    tienePendiente = 1;
                } else if ((a.importetotal - a.importepagado) > EPSILON) {
                    tienePendiente = 1;
                }
            }
        }
        fclose(f);
    }
    return tienePendiente;
}

int registrarClienteEnArchivo(Cliente *c) {
    FILE *f = NULL;
    Cliente temp;
    int existe = 0;
    int exito = 0;

    if (c != NULL && c->dni > 0) {
        f = fopen(ARCH_CLIENTES, "rb");
        if (f != NULL) {
            while (fread(&temp, sizeof(Cliente), 1, f) == 1) {
                if (temp.dni == c->dni) {
                    existe = 1;
                    break;
                }
            }
            fclose(f);
        }

        if (!existe) {
            f = fopen(ARCH_CLIENTES, "ab");
            if (f != NULL) {
                if (fwrite(c, sizeof(Cliente), 1, f) == 1u) {
                    exito = 1;
                }
                fclose(f);
            }
        }
    }
    return exito;
}

int modificarEstadoCliente(long dni, short nuevoEstado) {
    FILE *f; 
    Cliente c; 
    long pos; 
    int ok = 0;

    if ((nuevoEstado == 0 || nuevoEstado == 1) && dni > 0) {
        f = fopen(ARCH_CLIENTES, "r+b"); 
        if (f != NULL) {
            while ((pos = ftell(f)) >= 0 && fread(&c, sizeof(c), 1, f) == 1) {
                if (c.dni == dni) {
                    c.estado = nuevoEstado;
                    if (fseek(f, pos, SEEK_SET) == 0 && fwrite(&c, sizeof(c), 1, f) == 1 && fflush(f) == 0) {
                        ok = 1;
                    }
                    break;
                }
            }
            fclose(f);
        }
    }
    return ok;
}

void listarClientesActivosCorto(void) {
    FILE *f = fopen(ARCH_CLIENTES, "rb");
    Cliente c;
    int count = 0;

    printf("\n--- CLIENTES ACTIVOS REGISTRADOS ---\n");
    if (f != NULL) {
        while (fread(&c, sizeof(Cliente), 1, f) == 1) {
            if (c.estado == 1) {
                printf("  -> DNI: %ld | Nombre: %s, %s\n", c.dni, c.apellido, c.nombre);
                count++;
            }
        }
        fclose(f);
    }
    if (count == 0) {
        printf("  (No hay clientes activos registrados)\n");
    }
    printf("------------------------------------\n");
}

void listarTodosClientesCorto(void) {
    FILE *f = fopen(ARCH_CLIENTES, "rb"); 
    Cliente c; 
    int n = 0;

    puts("\n--- PADRON GENERAL DE CLIENTES ---");
    if (f != NULL) {
        while (fread(&c, sizeof(c), 1, f) == 1) { 
            printf("DNI: %-10ld | %-15s, %-15s | Estado: %s\n", c.dni, c.apellido, c.nombre, c.estado == 1 ? "ACTIVO" : "INACTIVO"); 
            ++n; 
        }
        fclose(f);
    } else {
        puts("No se pudo abrir clientes.dat.");
    }
    if (!n) puts("No hay clientes registrados.");
}

void registrarCliente(void) {
    Cliente c, match;
    int terminar = 0, dni_ok = 0, opcion = -1;
    int continuar = 1;

    memset(&c, 0, sizeof(Cliente));
    printf("\n===============================================\n");
    printf("              REGISTRAR NUEVO CLIENTE          \n");
    printf("===============================================\n");


    while (!terminar && !dni_ok) {
        c.dni = validarLONG("Ingrese DNI del cliente (0 para cancelar): ");
        if (c.dni == 0) {
            printf("Operacion cancelada por el usuario.\n");
            continuar = 0;
            terminar = 1;
        } else if (c.dni <= 999999L || c.dni > 99999999L) {
            printf("Error: DNI invalido. Ingrese un documento de 7 u 8 digitos.\n");
        } else {
            if (buscarCualquierClientePorDNI(c.dni, &match)) {
                if (match.estado == 1) {
                    printf("Error: Ya existe un cliente activo con DNI %ld.\n", c.dni);
                } else {
                    printf("Aviso: DNI existente. El cliente con DNI %ld figura INACTIVO en el sistema. Si desea reactivarlo, utilice la opcion 'Modificar estado de cliente'.\n", c.dni);
                    continuar = 0;
                    terminar = 1;
                }
            } else {
                dni_ok = 1;
            }
        }
    }


    if (continuar) {
        if (!validarTXT("Apellido", c.apellido, sizeof(c.apellido))) {
            continuar = 0;
        }
    }

    if (continuar) {
        if (!validarTXT("Nombre", c.nombre, sizeof(c.nombre))) {
            continuar = 0;
        }
    }

    if (continuar) {
        if (!validarCorreo("Email", c.email, sizeof(c.email))) {
            continuar = 0;
        }
    }

    if (continuar) {
        if (!pedirTelefono("", c.telefono, sizeof(c.telefono))) {
            continuar = 0;
        }
    }


    if (continuar) {
        if (!validarTXT("Direccion (Calle - Solo letras,luego se le pedira la numeracion)", c.direccion.calle, sizeof(c.direccion.calle))) {
            continuar = 0;
        }
    }

    if (continuar) {
        int numValido = 0;
        while (!numValido && continuar) {
            c.direccion.numeracion = validarINT("Numeracion (solo numeros) (0 cancela): ");
            if (c.direccion.numeracion == 0) {
                continuar = 0;
            } else if (c.direccion.numeracion <= 0) {
                printf("Error: La numeracion debe ser un numero positivo.\n");
            } else {
                numValido = 1;
            }
        }
    }


    if (continuar) {
        c.fechadealta = obtenerFechaActual();
        c.estado = 1;

        printf("\n===============================================\n");
        printf("        RESUMEN DE DATOS A REGISTRAR           \n");
        printf("===============================================\n");
        printf("  DNI          : %ld\n", c.dni);
        printf("  Apellido     : %s\n", c.apellido);
        printf("  Nombre       : %s\n", c.nombre);
        printf("  Email        : %s\n", c.email);
        printf("  Telefono     : %s\n", c.telefono);
        printf("  Domicilio    : %s %d\n", c.direccion.calle, c.direccion.numeracion);
        printf("  Fecha de Alta: %02d/%02d/%04d\n", c.fechadealta.dia, c.fechadealta.mes, c.fechadealta.anio);
        printf("  Estado       : %s\n", c.estado == 1 ? "ACTIVO" : "INACTIVO");
        printf("===============================================\n");

        printf("1) Confirmar y guardar cliente\n0) Cancelar operacion\n");
        opcion = validarINT("Seleccione una opcion: ");
        if (opcion == 1) {
            if (registrarClienteEnArchivo(&c)) {
                printf("\nCliente registrado correctamente en %s.\n", ARCH_CLIENTES);
            } else {
                printf("\nError critico: No se pudo guardar el cliente en el archivo.\n");
            }
        } else {
            printf("\nOperacion cancelada. No se aplicaron cambios.\n");
        }
    }
}

void modificarClientes(void) {
    long dni = 0;
    Cliente c;
    int nuevoEstado = 0;
    short estadoDestino = 0;
    int continuar = 1;

    printf("\n===============================================\n");
    printf("         MODIFICAR ESTADO DE CLIENTE           \n");
    printf("===============================================\n");

    listarTodosClientesCorto();
    dni = validarLONG("Ingrese DNI del cliente a modificar (0 para cancelar): ");
    if (dni == 0) {
        printf("Operacion cancelada.\n");
        continuar = 0;
    } 
    
    if (continuar) {
        if (!buscarCualquierClientePorDNI(dni, &c)) {
            printf("Error: Cliente no encontrado en el sistema.\n");
            continuar = 0;
        }
    }

    if (continuar) {
        printf("\nCliente seleccionado: %s, %s | Estado actual: %s\n", 
               c.apellido, c.nombre, c.estado == 1 ? "ACTIVO" : "INACTIVO");
        printf("Seleccione nuevo estado:\n1 - Activo\n2 - Inactivo\n0 - Cancelar\n");
        nuevoEstado = validarINT("Opcion: ");

        if (nuevoEstado == 0) {
            printf("Modificacion cancelada por el usuario.\n");
        } else if (nuevoEstado != 1 && nuevoEstado != 2) {
            printf("Opcion invalida. Modificacion cancelada.\n");
        } else {
            estadoDestino = (nuevoEstado == 1) ? 1 : 0;

            if (c.estado == estadoDestino) {
                if (c.estado == 0) {
                    printf("\nAviso: El cliente ya se encuentra INACTIVO. No se puede dar de baja si ya lo esta.\n");
                } else {
                    printf("\nAviso: El cliente ya figura ACTIVO en el sistema. No se puede dar de alta nuevamente.\n");
                }
            } else if (estadoDestino == 0 && clienteTieneAlquileresPendientes(dni)) {
                printf("\nERROR: No se puede desactivar al cliente porque posee alquileres en curso o saldos deudores pendientes.\n");
            } else {
                if (modificarEstadoCliente(dni, estadoDestino)) {
                    printf("\nEstado del cliente modificado exitosamente a %s.\n", 
                           estadoDestino == 1 ? "ACTIVO" : "INACTIVO");
                } else {
                    printf("\nError critico: No se pudo actualizar el estado en el archivo.\n");
                }
            }
        }
    }
}

void administrarClientesUI(void) {
    FILE *f = fopen(ARCH_CLIENTES, "rb");
    Cliente c;
    int count = 0;

    printf("\n========================================================================================\n");
    printf("                            PADRON COMPLETO DE CLIENTES                                \n");
    printf("========================================================================================\n");
    printf("%-12s | %-20s | %-20s | %-25s | %-10s\n", "DNI", "Apellido", "Nombre", "Email", "Estado");
    printf("----------------------------------------------------------------------------------------\n");

    if (f != NULL) {
        while (fread(&c, sizeof(Cliente), 1, f) == 1) {
            printf("%-12ld | %-20s | %-20s | %-25s | %-10s\n",
                   c.dni, c.apellido, c.nombre, c.email,
                   c.estado == 1 ? "ACTIVO" : "INACTIVO");
            count++;
        }
        fclose(f);
    }
    printf("----------------------------------------------------------------------------------------\n");
    printf("Total de clientes listados: %d\n", count);
}

void consultarClienteUI(void) {
    long dni = 0;
    Cliente c;

    printf("\n===============================================\n");
    printf("           CONSULTAR DATOS DE CLIENTE          \n");
    printf("===============================================\n");

    listarClientesActivosCorto();
    dni = validarLONG("Ingrese DNI del cliente (0 para cancelar): ");
    if (dni == 0) {
        printf("Operacion cancelada.\n");
    } else if (!buscarCualquierClientePorDNI(dni, &c)) {
        printf("Aviso: No se encuentra ningun cliente registrado con el DNI %ld.\n", dni);
    } else {
        printf("\n--- FICHA GENERAL DEL CLIENTE ---\n");
        printf("DNI          : %ld\n", c.dni);
        printf("Apellido     : %s\n", c.apellido);
        printf("Nombre       : %s\n", c.nombre);
        printf("Email        : %s\n", c.email);
        printf("Telefono     : %s\n", c.telefono);
        printf("Domicilio    : %s %d\n", c.direccion.calle, c.direccion.numeracion);
        printf("Fecha de Alta: %02d/%02d/%04d\n", c.fechadealta.dia, c.fechadealta.mes, c.fechadealta.anio);
        printf("Estado       : %s\n", c.estado == 1 ? "ACTIVO" : "INACTIVO");

        mostrarMovimientosCliente(c.dni);
    }
}

void menuClientes(void) {
    int op = -1, salir = 0;

    while (!salir) {
    	limpiar();
        printf("\n-----------------------------------------------\n");
        printf("               GESTION DE CLIENTES             \n");
        printf("-----------------------------------------------\n");
        printf("1. Registrar nuevo cliente\n");
        printf("2. Modificar estado de cliente (reactivacion y baja)\n");
        printf("3. Consultar cliente (Ficha e historial)\n");
        printf("4. Listar padron de clientes\n");
        printf("0. Volver al Menu Principal\n");
        printf("-----------------------------------------------\n");

        op = validarINT("Seleccione una opcion: ");
        switch (op) {
            case 1:
                uiTransicionEntrada("Registrar Cliente");
                registrarCliente();
                pausarConsola();
                break;
            case 2:
                uiTransicionEntrada("Modificar Cliente");
                modificarClientes();
                pausarConsola();
                break;
            case 3:
                uiTransicionEntrada("Consultar Ficha");
                consultarClienteUI();
                pausarConsola();
                break;
            case 4:
                uiTransicionEntrada("Padron General");
                administrarClientesUI();
                pausarConsola();
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

/* ==============================================================
 * IMPLEMENTACIÓN: MÓDULO VEHÍCULOS
 * ============================================================== */

const char *obtenerNombreEstadoVehiculo(int estado) {
    const char *nombre = "Desconocido";
    if (estado == 1) nombre = "Disponible";
    else if (estado == 2) nombre = "Alquilado";
    else if (estado == 3) nombre = "Baja";
    else if (estado == 4) nombre = "Mantenimiento";
    return nombre;
}

void imprimirFilaVehiculo(const Vehiculo *v) {
    if (v != NULL) {
        printf("%-8s | %-15s | %-20s | %4d | %-15s | %2d | $%10.2f | %-14s\n",
               v->patente, v->marca, v->modelo, v->anio, v->tipodevehiculo,
               v->capacidad, v->preciopordia, obtenerNombreEstadoVehiculo(v->estado));
    }
}

int buscarVehiculoPorPatente(const char *patente, Vehiculo *vOut, long *posByteOut) {
    int encontrado = 0;
    FILE *arch;
    Vehiculo v;
    long posActual = 0;

    if (patente != NULL && patente[0] != '\0') {
        arch = fopen(ARCH_VEHICULOS, "rb");
        if (arch != NULL) {
            while (!encontrado) {
                posActual = ftell(arch);
                if (posActual < 0) break;
                if (fread(&v, sizeof(Vehiculo), 1, arch) != 1) break;
                if (strcmp(v.patente, patente) == 0) {
                    encontrado = 1;
                    if (vOut != NULL) *vOut = v;
                    if (posByteOut != NULL) *posByteOut = posActual;
                }
            }
            fclose(arch);
        }
    }
    return encontrado;
}

int actualizarEstadoVehiculo(long posByte, int nuevoEstado) {
    FILE *f; 
    Vehiculo v; 
    int ok = 0;

    if (posByte >= 0 && nuevoEstado >= 1 && nuevoEstado <= 4) {
        f = fopen(ARCH_VEHICULOS, "r+b"); 
        if (f != NULL) {
            if (fseek(f, posByte, SEEK_SET) == 0 && fread(&v, sizeof(v), 1, f) == 1) {
                v.estado = nuevoEstado;
                if (fseek(f, posByte, SEEK_SET) == 0 && fwrite(&v, sizeof(v), 1, f) == 1 && fflush(f) == 0) {
                    ok = 1;
                }
            }
            fclose(f);
        }
    }
    return ok;
}

void listarVehiculosDisponiblesCorto(void) {
    FILE *arch = fopen(ARCH_VEHICULOS, "rb");
    Vehiculo v;
    int count = 0;

    printf("\n--- VEHICULOS DISPONIBLES PARA ALQUILAR ---\n");
    if (arch != NULL) {
        while (fread(&v, sizeof(Vehiculo), 1, arch) == 1) {
            if (v.estado == 1) {
                printf("  -> Patente: %s | %s %s | Tarifa: $%.2f/dia\n",
                       v.patente, v.marca, v.modelo, v.preciopordia);
                count++;
            }
        }
        fclose(arch);
    }
    if (count == 0) {
        printf("  (No hay vehiculos disponibles en este momento)\n");
    }
    printf("-------------------------------------------\n");
}

void listarFlotaCompleta(void) {
    FILE *f = fopen(ARCH_VEHICULOS, "rb"); 
    Vehiculo v; 
    int n = 0;

    puts("\n--- FLOTA VEHICULAR REGISTRADA ---");
    if (f != NULL) {
        while (fread(&v, sizeof(v), 1, f) == 1) { 
            imprimirFilaVehiculo(&v); 
            ++n; 
        }
        fclose(f); 
    } else {
        puts("No se pudo abrir vehiculos.dat.");
    }
    printf("Total: %d vehiculo(s)\n", n);
}

int vehiculoTieneAlquilerActivo(const char *patente) {
    FILE *f; 
    Alquiler a; 
    int encontrado = 0;

    if (patente != NULL && patente[0] != '\0') {
        f = fopen(ARCH_ALQUILERES, "rb"); 
        if (f != NULL) {
            while (!encontrado && fread(&a, sizeof(a), 1, f) == 1) {
                if (a.estado == 1 && strcmp(a.patenteDelVehiculo, patente) == 0) {
                    encontrado = 1;
                }
            }
            fclose(f); 
        }
    }
    return encontrado;
}

void registrarVehiculo(void) {
    Vehiculo v, aux; 
    char entrada[64], patente[8]; 
    long pos; 
    int anio, cap; 
    float precio; 
    FILE *f;
    int patenteOk = 0, continuar = 1;
    int opcion = -1;

    memset(&v, 0, sizeof(v)); 
    puts("\n=== REGISTRAR NUEVO VEHICULO ===");

    
    while (!patenteOk && continuar) {
        printf("Patente LLNNNLL (0 para cancelar): "); 
        if (!leerLinea(entrada, sizeof(entrada))) {
            continuar = 0;
        } else if (strcmp(entrada, "0") == 0) {
            continuar = 0;
        } else if (!validarPatente(entrada, patente)) { 
            puts("Patente invalida: formato obligatorio de 2 letras, 3 numeros y 2 letras continuas."); 
        } else if (buscarVehiculoPorPatente(patente, &aux, &pos)) { 
            puts("La patente ya existe en la flota."); 
        } else {
            memcpy(v.patente, patente, sizeof(patente)); 
            patenteOk = 1;
        }
    }

    
    if (continuar) {
        if (!validarTXT("Marca", v.marca, sizeof(v.marca))) {
            continuar = 0;
        }
    }

    if (continuar) {
        if (!validarTXT("Modelo", v.modelo, sizeof(v.modelo))) {
            continuar = 0;
        }
    }

    
    if (continuar) {
        int anioOk = 0;
        while (!anioOk && continuar) { 
            anio = validarINT("Anio de fabricacion 1990-2026 (0 cancela): "); 
            if (!anio) {
                continuar = 0;
            } else if (anio >= 1990 && anio <= 2026) {
                v.anio = anio;
                anioOk = 1;
            } else {
                puts("Anio fuera de rango."); 
            }
        }
    }

    
    if (continuar) {
        if (!validarTXT("Tipo de vehiculo (Sedan, Pickup, SUV, etc.)", v.tipodevehiculo, sizeof(v.tipodevehiculo))) {
            continuar = 0;
        }
    }

    if (continuar) {
        int capOk = 0;
        while (!capOk && continuar) { 
            cap = validarINT("Capacidad de pasajeros 1-15 (0 cancela): "); 
            if (!cap) {
                continuar = 0;
            } else if (cap >= 1 && cap <= 15) {
                v.capacidad = cap;
                capOk = 1;
            } else {
                puts("Capacidad fuera de rango."); 
            }
        }
    }

    
    if (continuar) {
        int precioOk = 0;
        while (!precioOk && continuar) { 
            precio = validarFLOAT("Precio diario de alquiler (0 cancela): $"); 
            if (precio == 0.0f) {
                continuar = 0;
            } else if (precio > 0.0f) {
                v.preciopordia = precio;
                precioOk = 1;
            }
        }
    }

    
    if (continuar) {
        v.estado = 1; 

        printf("\n===============================================\n");
        printf("        RESUMEN DE DATOS A REGISTRAR           \n");
        printf("===============================================\n");
        printf("  Patente       : %s\n", v.patente);
        printf("  Marca         : %s\n", v.marca);
        printf("  Modelo        : %s\n", v.modelo);
        printf("  Anio fabric.  : %d\n", v.anio);
        printf("  Tipo vehiculo : %s\n", v.tipodevehiculo);
        printf("  Capacidad     : %d pasajeros\n", v.capacidad);
        printf("  Precio / dia  : $%.2f\n", v.preciopordia);
        printf("  Estado inicial: %s\n", obtenerNombreEstadoVehiculo(v.estado));
        printf("===============================================\n");

        printf("1) Confirmar y guardar vehiculo\n0) Cancelar operacion\n");
        opcion = validarINT("Seleccione una opcion: ");
        if (opcion == 1) {
            f = fopen(ARCH_VEHICULOS, "ab"); 
            if (f != NULL) { 
                if (fwrite(&v, sizeof(v), 1, f) == 1 && fflush(f) == 0) {
                    printf("\nVehiculo %s (%s %s) registrado exitosamente en %s.\n", 
                           v.patente, v.marca, v.modelo, ARCH_VEHICULOS);
                } else {
                    puts("\nError critico: No se pudo escribir en el archivo.");
                }
                fclose(f);
            } else {
                puts("\nError: No se pudo abrir vehiculos.dat para escritura."); 
            }
        } else {
            printf("\nOperacion cancelada. No se aplicaron cambios.\n");
        }
    }
}

void modificarVehiculo(void) {
    Vehiculo v; 
    char entrada[64], patente[8]; 
    long pos; 
    int estado;
    int patenteOk = 0, continuar = 1;

    puts("\n=== MODIFICAR ESTADO DEL VEHICULO ==="); 
    listarFlotaCompleta();

    
    while (!patenteOk && continuar) {
        printf("Patente del vehiculo a modificar (0 cancela): "); 
        if (!leerLinea(entrada, sizeof(entrada))) {
            continuar = 0;
        } else if (strcmp(entrada, "0") == 0) {
            continuar = 0;
        } else if (!validarPatente(entrada, patente)) { 
            puts("Formato invalido. Ingrese LLNNNLL sin espacios."); 
        } else if (!buscarVehiculoPorPatente(patente, &v, &pos)) { 
            puts("No se encontro un vehiculo con esa patente."); 
        } else {
            patenteOk = 1;
        }
    }

    
    if (continuar) {
        if (v.estado == 2 || vehiculoTieneAlquilerActivo(v.patente)) { 
            puts("\nAccion denegada: El vehiculo se encuentra actualmente ALQUILADO.");
            puts("Su estado solo puede modificarse a traves del proceso de devolucion."); 
            continuar = 0;
        }
    }

    
    if (continuar) {
        printf("\nVehiculo: %s (%s %s) | Estado actual: %s\n", 
               v.patente, v.marca, v.modelo, obtenerNombreEstadoVehiculo(v.estado));
        printf("Seleccione nuevo estado:\n");
        printf("1 - Disponible\n");
        printf("3 - Baja\n");
        printf("4 - En Mantenimiento\n");
        printf("0 - Cancelar\n");
        estado = validarINT("Opcion: "); 

        if (estado == 0) {
            printf("\nModificacion cancelada por el usuario.\n");
        } else if (estado != 1 && estado != 3 && estado != 4) { 
            puts("\nError: Estado no permitido. No se puede asignar 'Alquilado' manualmente ni opciones fuera de rango."); 
        } else if (v.estado == estado) {
            
            if (estado == 1) {
                printf("\nAviso: El vehiculo ya figura DISPONIBLE en la flota. No requiere ser habilitado nuevamente.\n");
            } else if (estado == 3) {
                printf("\nAviso: El vehiculo ya se encuentra dado de BAJA. No se puede dar de baja si ya lo esta.\n");
            } else if (estado == 4) {
                printf("\nAviso: El vehiculo ya figura EN MANTENIMIENTO en el sistema.\n");
            }
        } else {
            
            if (!actualizarEstadoVehiculo(pos, estado)) {
                puts("\nError critico: No se pudo actualizar el archivo vehiculos.dat.");
            } else {
                printf("\nEstado del vehiculo actualizado con exito a: %s\n", obtenerNombreEstadoVehiculo(estado));
            }
        }
    }
}


void buscarVehiculo(void) {
    Vehiculo v; 
    char entrada[64], patente[8]; 
    long pos;
    int patenteOk = 0, continuar = 1;

    puts("\n=== BUSCAR VEHICULO ==="); 
    listarFlotaCompleta();

    while (!patenteOk && continuar) {
        printf("Patente a consultar (0 cancela): "); 
        if (!leerLinea(entrada, sizeof(entrada))) {
            continuar = 0;
        } else if (strcmp(entrada, "0") == 0) {
            continuar = 0;
        } else if (!validarPatente(entrada, patente)) { 
            puts("Patente invalida: use LLNNNLL sin espacios."); 
        } else if (!buscarVehiculoPorPatente(patente, &v, &pos)) { 
            puts("No se encontro esa patente. Intente nuevamente."); 
        } else {
            patenteOk = 1;
        }
    }

    if (continuar && patenteOk) {
        printf("\n================ FICHA DEL VEHICULO ================\n");
        printf("Patente           : %s\n", v.patente);
        printf("Marca             : %s\n", v.marca);
        printf("Modelo            : %s\n", v.modelo);
        printf("Anio              : %d\n", v.anio);
        printf("Tipo              : %s\n", v.tipodevehiculo);
        printf("Capacidad         : %d pasajeros\n", v.capacidad);
        printf("Tarifa por dia    : $%.2f\n", v.preciopordia);
        printf("Estado            : %s\n", obtenerNombreEstadoVehiculo(v.estado));
        printf("====================================================\n");
    }
}

void listarVehiculosDisponibles(void) {
    FILE *arch = fopen(ARCH_VEHICULOS, "rb");
    Vehiculo v;
    int contador = 0;

    printf("\n========================================================================================\n");
    printf("                         LISTADO DE VEHICULOS DISPONIBLES                               \n");
    printf("========================================================================================\n");

    if (arch != NULL) {
        printf("%-8s | %-15s | %-20s | %-4s | %-15s | %-2s | %-11s | %-14s\n",
               "Patente", "Marca", "Modelo", "Anio", "Tipo", "Cp", "Precio/Dia", "Estado");
        printf("----------------------------------------------------------------------------------------\n");

        while (fread(&v, sizeof(Vehiculo), 1, arch) == 1) {
            if (v.estado == 1) {
                imprimirFilaVehiculo(&v);
                contador++;
            }
        }
        fclose(arch);
        printf("----------------------------------------------------------------------------------------\n");
        printf("Total de vehiculos disponibles: %d\n", contador);
    }
}

void listarVehiculosAlquilados(void) {
    FILE *archV = NULL;
    FILE *archA = NULL;
    Vehiculo v;
    Alquiler a;
    Cliente c;
    int contador = 0;
    char marcaModelo[60];
    char clienteNombre[120];

    printf("\n=========================================================================================================================\n");
    printf("                                              VEHICULOS ALQUILADOS                                                       \n");
    printf("=========================================================================================================================\n");

    archV = fopen(ARCH_VEHICULOS, "rb");
    if (archV != NULL) {
        printf("%-9s | %-30s | %-35s | %-12s | %-12s\n",
               "Patente", "Marca y Modelo", "Cliente", "F. Inicio", "F. Prev. Dev.");
        printf("-------------------------------------------------------------------------------------------------------------------------\n");

        while (fread(&v, sizeof(Vehiculo), 1, archV) == 1) {
            if (v.estado == 2) {
                int contratoEncontrado = 0;

                
                snprintf(marcaModelo, sizeof(marcaModelo), "%s %s", v.marca, v.modelo);

                
                strcpy(clienteNombre, "[Sin contrato activo]");
                a.fechadeinicio = (Fecha){0, 0, 0};
                a.fechaprevistadedevolucion = (Fecha){0, 0, 0};

                
                archA = fopen(ARCH_ALQUILERES, "rb");
                if (archA != NULL) {
                    while (!contratoEncontrado && fread(&a, sizeof(Alquiler), 1, archA) == 1) {
                        if (a.estado == 1 && strcmp(a.patenteDelVehiculo, v.patente) == 0) {
                            contratoEncontrado = 1;
                        }
                    }
                    fclose(archA);
                }

                
                if (contratoEncontrado) {
                    memset(&c, 0, sizeof(Cliente));
                    if (buscarCualquierClientePorDNI(a.dnidelcliente, &c)) {
                        snprintf(clienteNombre, sizeof(clienteNombre), "%s, %s (DNI: %ld)", 
                                 c.apellido, c.nombre, c.dni);
                    } else {
                        snprintf(clienteNombre, sizeof(clienteNombre), "DNI: %ld", a.dnidelcliente);
                    }

                    printf("%-9s | %-30s | %-35s | %02d/%02d/%04d   | %02d/%02d/%04d\n",
                           v.patente,
                           marcaModelo,
                           clienteNombre,
                           a.fechadeinicio.dia, a.fechadeinicio.mes, a.fechadeinicio.anio,
                           a.fechaprevistadedevolucion.dia, a.fechaprevistadedevolucion.mes, a.fechaprevistadedevolucion.anio);
                } else {
                    
                    printf("%-9s | %-30s | %-35s | --/--/----   | --/--/----\n",
                           v.patente, marcaModelo, clienteNombre);
                }

                contador++;
            }
        }

        fclose(archV);
        printf("-------------------------------------------------------------------------------------------------------------------------\n");
        if (contador == 0) {
            printf("No hay vehiculos alquilados actualmente en la flota.\n");
        } else {
            printf("Total de vehiculos alquilados: %d\n", contador);
        }
    } else {
        printf("Aviso: No se pudo abrir %s o no existen vehiculos registrados.\n", ARCH_VEHICULOS);
    }
}

void menuVehiculos(void) {
    int op = -1, salir = 0;

    while (!salir) {
    	limpiar();
        printf("\n-----------------------------------------------\n");
        printf("              GESTION DE VEHICULOS             \n");
        printf("-----------------------------------------------\n");
        printf("1. Registrar nuevo vehiculo\n");
        printf("2. Modificar estado del vehiculo\n");
        printf("3. Buscar vehiculo por patente\n");
        printf("4. Listar vehiculos disponibles\n");
        printf("5. Listar vehiculos alquilados\n");
        printf("0. Volver al Menu Principal\n");
        printf("-----------------------------------------------\n");

        op = validarINT("Seleccione una opcion: ");
        switch (op) {
            case 1:
                uiTransicionEntrada("Alta de Vehiculo");
                registrarVehiculo();
                pausarConsola();
                break;
            case 2:
                uiTransicionEntrada("Modificar Vehiculo");
                modificarVehiculo();
                pausarConsola();
                break;
            case 3:
                uiTransicionEntrada("Buscar Vehiculo");
                buscarVehiculo();
                pausarConsola();
                break;
            case 4:
                uiTransicionEntrada("Vehiculos Disponibles");
                listarVehiculosDisponibles();
                pausarConsola();
                break;
            case 5:
                uiTransicionEntrada("Vehiculos Alquilados");
                listarVehiculosAlquilados();
                pausarConsola();
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

/* ==============================================================
 * IMPLEMENTACIÓN: MÓDULO ALQUILERES
 * ============================================================== */

int guardarAlquiler(const Alquiler *alq) {
    FILE *f; 
    int ok = 0;
    if (alq != NULL) {
        f = fopen(ARCH_ALQUILERES, "ab"); 
        if (f != NULL) {
            if (fwrite(alq, sizeof(*alq), 1, f) == 1 && fflush(f) == 0) {
                ok = 1;
            }
            fclose(f);
        }
    }
    return ok;
}

int buscarAlquilerPorDniYNumero(long dni, int numAlquiler, Alquiler *alqOut, long *posByteOut) {
    int encontrado = 0;
    FILE *f = fopen(ARCH_ALQUILERES, "rb");
    Alquiler a;
    long posActual = 0;

    if (f != NULL) {
        while (!encontrado) {
            posActual = ftell(f);
            if (posActual < 0) break;
            if (fread(&a, sizeof(Alquiler), 1, f) != 1) break;
            if (a.dnidelcliente == dni && a.numerodealquiler == numAlquiler) {
                encontrado = 1;
                if (alqOut != NULL) *alqOut = a;
                if (posByteOut != NULL) *posByteOut = posActual;
            }
        }
        fclose(f);
    }
    return encontrado;
}

int buscarAlquilerPorNumero(int numAlquiler, Alquiler *alqOut, long *posByteOut) {
    int encontrado = 0;
    FILE *f = fopen(ARCH_ALQUILERES, "rb");
    Alquiler a;
    long posActual = 0;

    if (f != NULL) {
        while (!encontrado) {
            posActual = ftell(f);
            if (posActual < 0) break;
            if (fread(&a, sizeof(Alquiler), 1, f) != 1) break;
            if (a.numerodealquiler == numAlquiler) {
                encontrado = 1;
                if (alqOut != NULL) *alqOut = a;
                if (posByteOut != NULL) *posByteOut = posActual;
            }
        }
        fclose(f);
    }
    return encontrado;
}

int sobreescribirAlquiler(long posByte, const Alquiler *alq) {
    FILE *f; 
    int ok = 0;
    if (posByte >= 0 && alq != NULL) {
        f = fopen(ARCH_ALQUILERES, "r+b"); 
        if (f != NULL) {
            if (fseek(f, posByte, SEEK_SET) == 0 && fwrite(alq, sizeof(*alq), 1, f) == 1 && fflush(f) == 0) {
                ok = 1;
            }
            fclose(f);
        }
    }
    return ok;
}

void imprimirEncabezadoConsultaAlquileres(void) {
    printf("\n%-7s | %-10s | %-30s | %-8s | %-10s | %-10s | %-5s | %-12s | %-12s | %-11s\n",
           "Nro", "DNI", "Cliente", "Patente", "F.Inicio", "F.Dev.Prev", "Dias", "Total", "Pagado", "Estado");
    puts("---------------------------------------------------------------------------------------------------------------------------------");
}

void mostrarFilaAlquiler(const Alquiler *a) {
    Cliente c; 
    char nombreCompleto[110]; 
    const char *estado = "Desconocido";

    if (a != NULL) {
        memset(&c, 0, sizeof(c));
        buscarCualquierClientePorDNI(a->dnidelcliente, &c);
        
        if (c.apellido[0]) {
            snprintf(nombreCompleto, sizeof(nombreCompleto), "%s, %s", c.apellido, c.nombre);
        } else {
            strcpy(nombreCompleto, "[Sin datos]");
        }

        if (a->estado == 1) estado = "Activo"; 
        else if (a->estado == 2) estado = "Finalizado"; 
        else if (a->estado == 3) estado = "Cancelado";

        printf("%-7d | %-10ld | %-30s | %-8s | %02d/%02d/%04d | %02d/%02d/%04d | %-5d | $%10.2f | $%10.2f | %-11s\n",
               a->numerodealquiler, a->dnidelcliente, nombreCompleto, a->patenteDelVehiculo,
               a->fechadeinicio.dia, a->fechadeinicio.mes, a->fechadeinicio.anio,
               a->fechaprevistadedevolucion.dia, a->fechaprevistadedevolucion.mes, a->fechaprevistadedevolucion.anio,
               a->cantdedias, a->importetotal, a->importepagado, estado);
    }
}

void listarAlquileresActivosConDetalle(void) {
    FILE *f = fopen(ARCH_ALQUILERES, "rb"); 
    Alquiler a; 
    Cliente c; 
    Vehiculo v; 
    long pos; 
    int n = 0;

    puts("\n--- ALQUILERES ACTIVOS EN CURSO ---");
    if (f != NULL) {
        while (fread(&a, sizeof(a), 1, f) == 1) {
            if (a.estado == 1) {
                memset(&c, 0, sizeof(c)); 
                memset(&v, 0, sizeof(v));
                buscarCualquierClientePorDNI(a.dnidelcliente, &c);
                buscarVehiculoPorPatente(a.patenteDelVehiculo, &v, &pos);
                printf("Alquiler #%d | DNI %ld (%s, %s) | Auto: %s %s (%s) | Saldo: $%.2f\n",
                       a.numerodealquiler, a.dnidelcliente, c.apellido, c.nombre, 
                       v.marca, v.modelo, a.patenteDelVehiculo, a.importetotal - a.importepagado); 
                ++n;
            }
        }
        fclose(f); 
    } else {
        puts("No hay archivo de alquileres.");
    }
    if (!n) puts("No hay alquileres activos.");
}

void registrarAlquiler(void) {
    Alquiler a; 
    Cliente c; 
    Vehiculo v; 
    long posV = -1; 
    char entrada[64], patente[8];
    int dias;
    float senia, minimo;
    int clienteOk = 0, autoOk = 0, continuar = 1;
    int opcion = -1;

    memset(&a, 0, sizeof(a)); 
    memset(&c, 0, sizeof(c)); 
    memset(&v, 0, sizeof(v));
    puts("\n=== REGISTRAR ALQUILER ==="); 

    /* 1. Selección de cliente */
    listarClientesActivosCorto();
    while (!clienteOk && continuar) {
        a.dnidelcliente = validarLONG("DNI del cliente titular (0 cancela): ");
        if (!a.dnidelcliente) { 
            puts("Operacion cancelada."); 
            continuar = 0; 
        } else if (buscarClientePorDNI(a.dnidelcliente, &c)) {
            printf("Cliente seleccionado: %s, %s\n", c.apellido, c.nombre);
            clienteOk = 1;
        } else {
            puts("No existe un cliente activo con ese DNI. Intente nuevamente.");
        }
    }

    
    if (continuar) {
        listarVehiculosDisponiblesCorto();
        while (!autoOk && continuar) {
            printf("Patente del vehiculo LLNNNLL (0 cancela): ");
            if (!leerLinea(entrada, sizeof(entrada))) {
                continuar = 0;
            } else if (strcmp(entrada, "0") == 0) { 
                puts("Operacion cancelada."); 
                continuar = 0; 
            } else if (!validarPatente(entrada, patente)) { 
                puts("Patente invalida: formato obligatorio LLNNNLL sin espacios."); 
            } else if (!buscarVehiculoPorPatente(patente, &v, &posV)) { 
                puts("Patente inexistente en la base de datos."); 
            } else if (v.estado != 1) { 
                printf("Vehiculo no disponible (Estado actual: %s).\n", obtenerNombreEstadoVehiculo(v.estado)); 
            } else if (vehiculoTieneAlquilerActivo(patente)) { 
                puts("Inconsistencia: El vehiculo ya posee un alquiler activo."); 
            } else {
                autoOk = 1;
            }
        }
    }

   
    if (continuar) {
        a.fechadeinicio = obtenerFechaActual();
        if (!validarFechaStruct(a.fechadeinicio)) { 
            puts("Error: Fecha del sistema fuera del periodo valido (2020-2026)."); 
            continuar = 0;
        } else {
            printf("Fecha de inicio automatica (Hoy): %02d/%02d/%04d\n", a.fechadeinicio.dia, a.fechadeinicio.mes, a.fechadeinicio.anio);
        }
    }

    if (continuar) {
        int diasOk = 0;
        while (!diasOk && continuar) { 
            dias = validarINT("Cantidad de dias de alquiler (0 cancela): "); 
            if (!dias) {
                continuar = 0;
            } else if (dias > 0 && dias <= 365) {
                a.cantdedias = dias; 
                a.fechaprevistadedevolucion = sumarDiasAFecha(a.fechadeinicio, dias);
                diasOk = 1;
            } else {
                puts("Duracion invalida: ingrese entre 1 y 365 dias."); 
            }
        }
    }

    
    if (continuar) {
        a.numerodealquiler = generarCOD(ARCH_ALQUILERES, sizeof(a), offsetof(Alquiler, numerodealquiler));
        if (a.numerodealquiler <= 0) { 
            puts("No se pudo generar un codigo valido."); 
            continuar = 0;
        } else {
            a.importetotal = (float)a.cantdedias * v.preciopordia; 
            minimo = a.importetotal * 0.50f;
            printf("Importe total: $%.2f | Sena obligatoria minima (50%%): $%.2f\n", a.importetotal, minimo);
        }
    }

    
    if (continuar) {
        int pagoOk = 0;
        while (!pagoOk && continuar) { 
            senia = validarFLOAT("Monto abonado en concepto de sena (0 cancela): $"); 
            if (senia == 0.0f) {
                continuar = 0;
            } else if (senia < minimo - EPSILON || senia > a.importetotal + EPSILON) {
                puts("La sena debe cubrir entre el 50% y el 100% del total presupuestado.");
            } else {
                a.importepagado = senia;
                pagoOk = 1;
            }
        }
    }

    
    if (continuar) {
        a.estado = 1; /* 1: Activo */
        memcpy(a.patenteDelVehiculo, patente, sizeof(patente));

        printf("\n===============================================\n");
        printf("        RESUMEN DEL ALQUILER A REGISTRAR       \n");
        printf("===============================================\n");
        printf("  Alquiler Nro      : #%d\n", a.numerodealquiler);
        printf("  Cliente           : %s, %s (DNI: %ld)\n", c.apellido, c.nombre, a.dnidelcliente);
        printf("  Vehiculo          : %s (%s %s)\n", a.patenteDelVehiculo, v.marca, v.modelo);
        printf("  Fecha de Inicio   : %02d/%02d/%04d\n", a.fechadeinicio.dia, a.fechadeinicio.mes, a.fechadeinicio.anio);
        printf("  Fecha Prev. Entrega: %02d/%02d/%04d\n", a.fechaprevistadedevolucion.dia, a.fechaprevistadedevolucion.mes, a.fechaprevistadedevolucion.anio);
        printf("  Dias contratados  : %d dia(s)\n", a.cantdedias);
        printf("  Importe Total     : $%.2f\n", a.importetotal);
        printf("  Sena Abonada      : $%.2f\n", a.importepagado);
        printf("  Saldo Pendiente   : $%.2f\n", a.importetotal - a.importepagado);
        printf("===============================================\n");

        printf("1) Confirmar y registrar alquiler\n0) Cancelar operacion\n");
        opcion = validarINT("Seleccione una opcion: ");
        if (opcion == 1) {
            
            if (!actualizarEstadoVehiculo(posV, 2)) { 
                puts("Error critico: No se pudo reservar el vehiculo en la flota."); 
            } else {
                
                if (!guardarAlquiler(&a)) { 
                    actualizarEstadoVehiculo(posV, 1); 
                    puts("Error al guardar contrato; se cancelo la reserva del vehiculo."); 
                } else {
                    
                    if (a.importepagado > EPSILON) {
                        registrarMovimiento(a.numerodealquiler, MOV_PAGO, a.importepagado, a.fechadeinicio, a.dnidelcliente, a.patenteDelVehiculo);
                    }
                    printf("\nAlquiler #%d registrado exitosamente en %s.\n", a.numerodealquiler, ARCH_ALQUILERES);
                }
            }
        } else {
            printf("\nOperacion cancelada. No se aplicaron cambios ni se modifico el vehiculo.\n");
        }
    }
}

void modificarAlquiler(void) {
    FILE *f; 
    Alquiler a, elegido; 
    long dni, pos = -1, posV = -1; 
    int n = 0, num = 0, estadoNuevo; 
    Vehiculo v;
    int continuar = 1;

    puts("\n=== MODIFICAR ESTADO DE ALQUILER ==="); 
    listarAlquileresActivosConDetalle();

    
    dni = validarLONG("DNI del cliente titular (0 cancela): "); 
    if (!dni) {
        continuar = 0;
    }

    
    if (continuar) {
        f = fopen(ARCH_ALQUILERES, "rb"); 
        if (f != NULL) {
            while (fread(&a, sizeof(a), 1, f) == 1) {
                if (a.estado == 1 && a.dnidelcliente == dni) { 
                    elegido = a; 
                    pos = ftell(f) - (long)sizeof(a); 
                    ++n; 
                    printf("%d) Alquiler #%d | Patente %s | Total $%.2f | Pagado $%.2f\n", 
                           n, a.numerodealquiler, a.patenteDelVehiculo, a.importetotal, a.importepagado); 
                }
            }
            fclose(f);
        } else {
            puts("No se pudo abrir alquileres.dat."); 
            continuar = 0;
        }
    }

    if (continuar) {
        if (!n) { 
            puts("El cliente no posee alquileres activos."); 
            continuar = 0;
        } else if (pos < 0) { 
            puts("Error al localizar el contrato."); 
            continuar = 0;
        }
    }

    
    if (continuar && n > 1) { 
        num = validarINT("Numero de alquiler de la lista (0 cancela): "); 
        if (!num) {
            continuar = 0;
        } else if (!buscarAlquilerPorDniYNumero(dni, num, &elegido, &pos) || elegido.estado != 1 || pos < 0) { 
            puts("El alquiler seleccionado no corresponde al cliente o no esta activo."); 
            continuar = 0;
        }
    }

    
    if (continuar) {
        if (!buscarVehiculoPorPatente(elegido.patenteDelVehiculo, &v, &posV)) { 
            puts("No se encontro el vehiculo vinculado al contrato."); 
            continuar = 0;
        }
    }

    
    if (continuar) {
        printf("\nAlquiler seleccionado: #%d | Vehiculo: %s (%s %s) | Estado actual: ACTIVO\n", 
               elegido.numerodealquiler, v.patente, v.marca, v.modelo);
        printf("Opciones de estado:\n");
        printf("1 - Activo\n");
        printf("2 - Finalizado\n");
        printf("3 - Cancelado (Libera el vehiculo)\n");
        printf("0 - Salir sin modificar\n");
        estadoNuevo = validarINT("Seleccione una opcion: "); 

        if (estadoNuevo == 0) {
            printf("\nModificacion cancelada por el usuario.\n");
        } else if (estadoNuevo == 1) {
            printf("\nAviso: El alquiler #%d ya figura en estado ACTIVO en el sistema.\n", elegido.numerodealquiler);
        } else if (estadoNuevo == 2) {
            printf("\nAviso: No se puede finalizar un alquiler desde esta opcion.\n");
            printf("Para finalizarlo, debe registrar la devolucion del vehiculo y liquidar el saldo en el modulo de Devoluciones.\n");
        } else if (estadoNuevo == 3) {
            
            if (v.estado != 2) { 
                puts("\nInconsistencia: El vehiculo vinculado no figura como Alquilado en la flota."); 
            } else {
                
                if (!actualizarEstadoVehiculo(posV, 1)) {
                    puts("\nError critico: No se pudo liberar el vehiculo.");
                } else {
                    elegido.estado = 3; 
                    
                    
                    if (!sobreescribirAlquiler(pos, &elegido)) {
                        
                        actualizarEstadoVehiculo(posV, 2);
                        puts("\nError critico: No se pudo actualizar el contrato; se revirtio el estado del vehiculo."); 
                    } else {
                        printf("\nAlquiler #%d CANCELADO exitosamente.\n", elegido.numerodealquiler);
                        printf("El vehiculo %s (%s %s) queda nuevamente DISPONIBLE para alquilar.\n", 
                               v.patente, v.marca, v.modelo);
                    }
                }
            }
        } else {
            printf("\nError: Opcion invalida (%d). Los alquileres activos solo pueden cancelarse (opcion 3) o salir (opcion 0).\n", estadoNuevo);
        }
    }
}

void registrarDevolucion(void) {
    FILE *f = NULL; 
    Alquiler a, sel; 
    Cliente c; 
    Vehiculo v; 
    long dni = 0, posA = -1, posV = -1; 
    int n = 0, num = 0;
    int diasPactados = 0, diasReales = 0, diasAtraso = 0;
    Fecha hoy; 
    float total = 0.0f, saldo = 0.0f, pago = 0.0f, ajuste = 0.0f;
    int continuar = 1;

    puts("\n=== REGISTRAR DEVOLUCION ==="); 
    listarAlquileresActivosConDetalle();

    
    dni = validarLONG("DNI del titular del alquiler (0 cancela): "); 
    if (!dni) {
        continuar = 0;
    }

    
    if (continuar) {
        f = fopen(ARCH_ALQUILERES, "rb"); 
        if (f != NULL) {
            while (fread(&a, sizeof(a), 1, f) == 1) {
                if (a.estado == 1 && a.dnidelcliente == dni) {
                    sel = a; 
                    posA = ftell(f) - (long)sizeof(a); 
                    ++n;
                    printf("%d) Alquiler #%d | Patente %s | Fecha Prevista %02d/%02d/%04d | Saldo $%.2f\n",
                           n, a.numerodealquiler, a.patenteDelVehiculo, 
                           a.fechaprevistadedevolucion.dia, a.fechaprevistadedevolucion.mes, a.fechaprevistadedevolucion.anio, 
                           a.importetotal - a.importepagado);
                }
            }
            fclose(f);
        } else {
            puts("No se pudo abrir alquileres.dat."); 
            continuar = 0;
        }
    }

    if (continuar) {
        if (!n) { 
            puts("Ese DNI no posee alquileres activos para devolver."); 
            continuar = 0;
        } else if (posA < 0) { 
            puts("Error de posicionamiento en archivo."); 
            continuar = 0;
        }
    }

    
    if (continuar && n > 1) { 
        num = validarINT("Seleccione numero de alquiler (0 cancela): "); 
        if (!num) {
            continuar = 0;
        } else if (!buscarAlquilerPorDniYNumero(dni, num, &sel, &posA) || sel.estado != 1 || posA < 0) { 
            puts("Seleccion no valida."); 
            continuar = 0;
        }
    }

    
    if (continuar) {
        if (!buscarVehiculoPorPatente(sel.patenteDelVehiculo, &v, &posV)) { 
            puts("No se encontro el vehiculo asignado."); 
            continuar = 0;
        } else if (v.estado != 2) { 
            puts("Inconsistencia: El contrato esta activo pero el vehiculo no figura Alquilado."); 
            continuar = 0;
        }
    }

    
    if (continuar) {
        memset(&c, 0, sizeof(c)); 
        buscarCualquierClientePorDNI(dni, &c);

        hoy = obtenerFechaActual();
        if (!validarFechaStruct(hoy) || compararFechas(hoy, sel.fechadeinicio) < 0) { 
            puts("Error: Fecha actual invalida o previa a la fecha de inicio del alquiler."); 
            continuar = 0;
        }
    }

    
    if (continuar) {
        diasPactados = sel.cantdedias;
        diasReales = diferenciaDias(hoy, sel.fechadeinicio); 
        if (diasReales < 1) diasReales = 1;

        total = (float)diasReales * v.preciopordia;
        saldo = total - sel.importepagado;

        printf("\nTitular: %s, %s | Auto: %s %s (%s)\n", c.apellido, c.nombre, v.marca, v.modelo, v.patente);
        printf("Fecha efectiva (Hoy): %02d/%02d/%04d\n", hoy.dia, hoy.mes, hoy.anio);

        if (diasReales > diasPactados) {
            diasAtraso = diasReales - diasPactados;
            printf("\n>>> AVISO DE MORA <<<\n");
            printf("Los dias ingresados fueron (%d) pero el cliente lo devolvio en (%d), se le sumaran los dias de atraso al precio.\n",
                   diasPactados, diasReales);
            printf("Dias adicionales: %d | Recargo por mora: $%.2f\n", 
                   diasAtraso, (float)diasAtraso * v.preciopordia);
        } else if (diasReales < diasPactados) {
            printf("\n>>> AVISO DE DEVOLUCION ANTICIPADA <<<\n");
            printf("El cliente devolvio el vehiculo en %d dia(s) (pactados originalmente: %d dias).\n",
                   diasReales, diasPactados);
            printf("Se recalculan los costos en base a los dias reales de uso.\n");
        } else {
            printf("\nEl vehiculo fue devuelto en el plazo exacto pactado (%d dias).\n", diasPactados);
        }

        printf("Total recalculado : $%.2f\n", total);
        printf("Abonado previamente: $%.2f\n", sel.importepagado);
        printf("Saldo a regularizar: $%.2f\n", saldo);

        
        if (saldo > EPSILON) {
            int pagoValido = 0;
            printf("\nPara completar la devolucion debe saldar el monto pendiente exacto de $%.2f\n", saldo);

            while (!pagoValido) {
                pago = validarFLOAT("Monto a abonar para liquidar cuenta: $");
                
                if (pago <= 0.0f) {
                    puts("Error: Debe ingresar un valor numerico positivo para saldar la deuda.");
                } else if (pago < saldo - EPSILON) {
                    printf("Error: Monto insuficiente. Ingreso $%.2f y el saldo pendiente es $%.2f. Ingrese el monto exacto.\n", 
                           pago, saldo);
                } else if (pago > saldo + EPSILON) {
                    printf("Error: Monto excedido. Ingreso $%.2f y el saldo pendiente es $%.2f. Ingrese el monto exacto.\n", 
                           pago, saldo);
                } else {
                    pagoValido = 1;
                }
            }
        } else if (saldo < -EPSILON) {
            
            ajuste = -saldo;
            printf("\n>>> SALDO A FAVOR DEL CLIENTE <<<\n");
            printf("Corresponde reintegro al cliente por devolucion anticipada: $%.2f\n", ajuste);
        }
    }

    
    if (continuar) {
        sel.cantdedias = diasReales; 
        sel.importetotal = total;
        sel.importepagado = total; 
        sel.fechaefectivadedevolucion = hoy; 
        sel.estado = 2; 

        if (!sobreescribirAlquiler(posA, &sel)) { 
            puts("Error critico: No se pudo actualizar el registro de alquiler."); 
        } else if (!actualizarEstadoVehiculo(posV, 1)) {
            sel.estado = 1;
            sobreescribirAlquiler(posA, &sel); 
            puts("Error critico: No se pudo liberar el vehiculo."); 
        } else {
            
            if (pago > EPSILON) {
                registrarMovimiento(sel.numerodealquiler, MOV_PAGO, pago, hoy, dni, sel.patenteDelVehiculo);
            }
            if (ajuste > EPSILON) {
                registrarMovimiento(sel.numerodealquiler, MOV_AJUSTE, ajuste, hoy, dni, sel.patenteDelVehiculo);
            }
            registrarMovimiento(sel.numerodealquiler, MOV_DEVOLUCION, 0.0f, hoy, dni, sel.patenteDelVehiculo);

            printf("\nDevolucion completada con exito.\n");
            printf("Vehiculo %s (%s %s) DISPONIBLE nuevamente para la flota.\n", 
                   v.patente, v.marca, v.modelo);
        }
    }
}

void listarAlquileresActivos(void) {
    FILE *arch = fopen(ARCH_ALQUILERES, "rb");
    Alquiler a;
    int cant = 0;

    printf("\n========================================================================================\n");
    printf("                             LISTADO DE ALQUILERES ACTIVOS                              \n");
    printf("========================================================================================\n");

    if (arch != NULL) {
        imprimirEncabezadoConsultaAlquileres();
        while (fread(&a, sizeof(Alquiler), 1, arch) == 1) {
            if (a.estado == 1) {
                mostrarFilaAlquiler(&a);
                cant++;
            }
        }
        fclose(arch);
        printf("------------------------------------------------------------------------------------------------------------------\n");
        printf("Total alquileres activos: %d\n", cant);
    }
}

void consultarAlquileresPorRangoFechas(void) {
    FILE *f = fopen(ARCH_ALQUILERES, "rb");
    Alquiler alq;
    Fecha fDesde, fHasta;
    long cDesde = 0, cHasta = 0, cAlq = 0;
    int contador = 0;
    int continuar = 1;

    printf("\n=======================================================\n");
    printf("         CONSULTA: ALQUILERES POR RANGO DE FECHAS      \n");
    printf("=======================================================\n");

    fDesde = leerFechaInicio(); 
    if (!validarFechaStruct(fDesde)) { 
        puts("Consulta cancelada."); 
        continuar = 0;
    }
    
    if (continuar) {
        fHasta = leerFechaInicio(); 
        if (!validarFechaStruct(fHasta)) { 
            puts("Consulta cancelada."); 
            continuar = 0;
        }
    }

    if (continuar) {
        cDesde = convertirFechaAClave(fDesde);
        cHasta = convertirFechaAClave(fHasta);

        if (cHasta < cDesde) {
            printf("Error: La fecha final no puede ser previa a la inicial.\n");
        } else if (f != NULL) {
            imprimirEncabezadoConsultaAlquileres();
            while (fread(&alq, sizeof(Alquiler), 1, f) == 1) {
                cAlq = convertirFechaAClave(alq.fechadeinicio);
                if (cAlq >= cDesde && cAlq <= cHasta) {
                    mostrarFilaAlquiler(&alq);
                    contador++;
                }
            }
            printf("------------------------------------------------------------------------------------------------------------------\n");
            printf("Total encontrados en el rango: %d\n", contador);
        }
    }

    if (f != NULL) {
        fclose(f);
    }
}

void consultarAlquileresPorCliente(void) {
    FILE *f = NULL;
    Alquiler alq;
    Cliente c;
    long dni = 0;
    int contador = 0;
    int dniValido = 0;
    int continuar = 1;

    printf("\n=======================================================\n");
    printf("             CONSULTA: ALQUILERES POR CLIENTE          \n");
    printf("=======================================================\n");

    
    listarClientesActivosCorto();

    
    while (!dniValido && continuar) {
        dni = validarLONG("Ingrese DNI del cliente (0 para cancelar): ");
        if (dni == 0) {
            printf("Consulta cancelada por el usuario.\n");
            continuar = 0;
        } else if (!buscarCualquierClientePorDNI(dni, &c)) {
            printf("Error: No existe ningun cliente registrado con el DNI %ld. Reintente.\n", dni);
        } else if (c.estado != 1) {
            printf("Error: El cliente con DNI %ld figura INACTIVO en el sistema. Solo se permite consultar clientes activos.\n", dni);
        } else {
            printf("Cliente seleccionado: %s, %s\n", c.apellido, c.nombre);
            dniValido = 1;
        }
    }

    
    if (continuar) {
        f = fopen(ARCH_ALQUILERES, "rb");
        if (f != NULL) {
            imprimirEncabezadoConsultaAlquileres();
            while (fread(&alq, sizeof(Alquiler), 1, f) == 1) {
                if (alq.dnidelcliente == dni) {
                    mostrarFilaAlquiler(&alq);
                    contador++;
                }
            }
            fclose(f);
            printf("------------------------------------------------------------------------------------------------------------------\n");
            printf("Total de alquileres asociados al cliente: %d\n", contador);
        } else {
            printf("Aviso: No se pudo abrir el archivo %s.\n", ARCH_ALQUILERES);
        }
    }
}

void consultarAlquileresPorVehiculo(void) {
    FILE *f = NULL;
    Alquiler alq;
    Vehiculo v;
    long posV = -1;
    char bufferEntrada[50], patente[8];
    int contador = 0;
    int patenteOk = 0;
    int continuar = 1;

    printf("\n===============================================\n");
    printf("            CONSULTA POR VEHICULO              \n");
    printf("===============================================\n");

    listarFlotaCompleta();

    while (!patenteOk && continuar) {
        printf("Ingrese patente sin espacios (0 para cancelar): ");
        if (!leerLinea(bufferEntrada, sizeof(bufferEntrada))) {
            continuar = 0;
        } else if (strcmp(bufferEntrada, "0") == 0) {
            printf("Consulta cancelada por el usuario.\n");
            continuar = 0;
        } else if (!validarPatente(bufferEntrada, patente)) {
            puts("Error: Formato de patente invalido o con espacios.");
            puts("Debe contener exactamente 7 caracteres: 2 letras, 3 numeros y 2 letras continuas (ej: AB123CD). Reintente.");
        } else if (!buscarVehiculoPorPatente(patente, &v, &posV)) {
            printf("Aviso: La patente %s no existe en la flota vehicular. Reintente.\n", patente);
        } else {
            patenteOk = 1;
        }
    }


    if (continuar && patenteOk) {
        printf("\nVehiculo seleccionado: %s (%s %s)\n", v.patente, v.marca, v.modelo);

        f = fopen(ARCH_ALQUILERES, "rb");
        if (f != NULL) {
            imprimirEncabezadoConsultaAlquileres();
            while (fread(&alq, sizeof(Alquiler), 1, f) == 1) {
                if (strcmp(alq.patenteDelVehiculo, patente) == 0) {
                    mostrarFilaAlquiler(&alq);
                    contador++;
                }
            }
            fclose(f);
            printf("------------------------------------------------------------------------------------------------------------------\n");
            if (contador == 0) {
                printf("El vehiculo %s no registra contratos de alquiler asociados hasta el momento.\n", patente);
            } else {
                printf("Total de operaciones vinculadas al vehiculo: %d\n", contador);
            }
        } else {
            printf("Aviso: No se pudo abrir el archivo %s o aun no hay contratos cargados.\n", ARCH_ALQUILERES);
        }
    }
}

void consultarAlquileresPorEstado(void) {
    FILE *f = fopen(ARCH_ALQUILERES, "rb");
    Alquiler alq;
    int est = 0, contador = 0;

    printf("\n===============================================\n");
    printf("             CONSULTA POR ESTADO               \n");
    printf("===============================================\n");
    printf("1 - Activo\n2 - Finalizado\n3 - Cancelado\n0 - Cancelar\n");
    est = validarINT("Estado a consultar: ");

    if (est >= 1 && est <= 3 && f != NULL) {
        imprimirEncabezadoConsultaAlquileres();
        while (fread(&alq, sizeof(Alquiler), 1, f) == 1) {
            if (alq.estado == est) {
                mostrarFilaAlquiler(&alq);
                contador++;
            }
        }
        printf("------------------------------------------------------------------------------------------------------------------\n");
        printf("Total encontrados: %d\n", contador);
    }
    if (f != NULL) {
        fclose(f);
    }
}

void consultarAlquileresPorImporte(void) {
    FILE *f = fopen(ARCH_ALQUILERES, "rb");
    Alquiler alq;
    float corte = 0.0f;
    int crit = 0, contador = 0;

    printf("\n===============================================\n");
    printf("            CONSULTA POR IMPORTE               \n");
    printf("===============================================\n");

    corte = validarFLOAT("Ingrese importe de referencia (0 para cancelar): $");
    if (corte > 0.0f) {
        printf("1 - Superiores a $%.2f\n2 - Inferiores a $%.2f\n", corte, corte);
        crit = validarINT("Criterio (1 o 2): ");

        if ((crit == 1 || crit == 2) && f != NULL) {
            imprimirEncabezadoConsultaAlquileres();
            while (fread(&alq, sizeof(Alquiler), 1, f) == 1) {
                int cumple = (crit == 1) ? (alq.importetotal > corte + EPSILON)
                                         : (alq.importetotal < corte - EPSILON);
                if (cumple) {
                    mostrarFilaAlquiler(&alq);
                    contador++;
                }
            }
            printf("------------------------------------------------------------------------------------------------------------------\n");
            printf("Total encontrados: %d\n", contador);
        }
    }
    if (f != NULL) {
        fclose(f);
    }
}

void menuAlquileres(void) {
    int op = -1, salir = 0;

    while (!salir) {
    	limpiar();
        printf("\n-----------------------------------------------\n");
        printf("              GESTION DE ALQUILERES            \n");
        printf("-----------------------------------------------\n");
        printf("1. Realizar Alquiler \n");
        printf("2. Modificar Estado de Alquiler\n");
        printf("3. Listar Todos los Alquileres (Activos e Inactivos)\n");
        printf("4. Consultas de Alquileres\n");
        printf("0. Volver al Menu Principal\n");
        printf("-----------------------------------------------\n");

        op = validarINT("Seleccione una opcion: ");
        switch (op) {
            case 1:
                uiTransicionEntrada("Nuevo Alquiler");
                registrarAlquiler();
                pausarConsola();
                break;
            case 2:
                uiTransicionEntrada("Modificar Alquiler");
                modificarAlquiler();
                pausarConsola();
                break;
            case 3:
                uiTransicionEntrada("Padron de Alquileres");
                listarTodosAlquileres();
                pausarConsola();
                break;
            case 4:
                uiTransicionEntrada("Menu Consultas");
                menuConsultasAlquileres();
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

void listarTodosAlquileres(void) {
    FILE *arch = NULL;
    Alquiler a;
    int cantTotal = 0;
    int cantActivos = 0;
    int cantInactivos = 0;

    printf("\n=================================================================================================================================\n");
    printf("                                       PADRON GENERAL DE ALQUILERES (TODOS LOS ESTADOS)                                          \n");
    printf("=================================================================================================================================\n");

    arch = fopen(ARCH_ALQUILERES, "rb");
    if (arch != NULL) {
        imprimirEncabezadoConsultaAlquileres();
        
        while (fread(&a, sizeof(Alquiler), 1, arch) == 1) {
            mostrarFilaAlquiler(&a);
            cantTotal++;
            
            if (a.estado == 1) {
                cantActivos++;
            } else {
                cantInactivos++;
            }
        }
        
        fclose(arch);
        
        printf("---------------------------------------------------------------------------------------------------------------------------------\n");
        printf("Total registrados: %d | Activos en curso: %d | Finalizados / Cancelados: %d\n", 
               cantTotal, cantActivos, cantInactivos);
    } else {
        printf("Aviso: No se pudo abrir el archivo %s o no existen alquileres registrados hasta el momento.\n", ARCH_ALQUILERES);
    }
}

void menuDevoluciones(void) {
    int op = -1, salir = 0;

    while (!salir) {
    	limpiar();
        printf("\n-----------------------------------------------\n");
        printf("             GESTION DE DEVOLUCIONES           \n");
        printf("-----------------------------------------------\n");
        printf("1. Registrar Devolucion de Vehiculo \n");
        printf("0. Volver al Menu Principal\n");
        printf("-----------------------------------------------\n");

        op = validarINT("Seleccione una opcion: ");
        switch (op) {
            case 1:
                uiTransicionEntrada("Registrar Devolucion");
                registrarDevolucion();
                pausarConsola();
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

void menuConsultasAlquileres(void) {
    int op = -1, salir = 0;

    while (!salir) {
    	limpiar();
        printf("\n-----------------------------------------------\n");
        printf("           CONSULTAS DE ALQUILERES             \n");
        printf("-----------------------------------------------\n");
        printf("1. Alquileres por Rango de Fechas (2020-2026)\n");
        printf("2. Alquileres por Cliente (DNI)\n");
        printf("3. Alquileres por Vehiculo (Patente)\n");
        printf("4. Alquileres por Estado\n");
        printf("5. Alquileres por Importe\n");
        printf("0. Volver al Menu de Alquileres\n");
        printf("-----------------------------------------------\n");

        op = validarINT("Seleccione una opcion: ");
        switch (op) {
            case 1:
                uiTransicionEntrada("Rango de Fechas");
                consultarAlquileresPorRangoFechas();
                pausarConsola();
                break;
            case 2:
                uiTransicionEntrada("Alquileres x Cliente");
                consultarAlquileresPorCliente();
                pausarConsola();
                break;
            case 3:
                uiTransicionEntrada("Alquileres x Patente");
                consultarAlquileresPorVehiculo();
                pausarConsola();
                break;
            case 4:
                uiTransicionEntrada("Filtro por Estado");
                consultarAlquileresPorEstado();
                pausarConsola();
                break;
            case 5:
                uiTransicionEntrada("Filtro por Importe");
                consultarAlquileresPorImporte();
                pausarConsola();
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

/* ==============================================================
 * IMPLEMENTACIÓN: MÓDULO MOVIMIENTOS Y PAGOS
 * ============================================================== */

const char *nombreTipo(int tipo) {
    const char *nombre = "Otro";
    if (tipo == MOV_ALQUILER) nombre = "Alquiler";
    else if (tipo == MOV_PAGO) nombre = "Pago";
    else if (tipo == MOV_DEVOLUCION) nombre = "Devolucion";
    else if (tipo == MOV_AJUSTE) nombre = "Ajuste";
    return nombre;
}

void imprimirEncabezadoMov(void) {
    printf("\n%-5s %-9s %-12s %-11s %-11s %-9s\n",
           "Mov", "Alquiler", "Tipo", "Monto", "Fecha", "Patente");
    printf("------------------------------------------------------------\n");
}

void imprimirMovimiento(const Movimiento *m) {
    printf("%-5d %-9d %-12s $%-10.2f %02d/%02d/%04d  %-9s\n",
           m->numeroDeMovimiento, m->numeroDeAlquiler, nombreTipo(m->tipoDeMovimiento),
           m->monto, m->fecha.dia, m->fecha.mes, m->fecha.anio, m->patenteDelVehiculo);
}

void actualizarArchivoCliente(const Movimiento *m) {
    char nombre[50];
    FILE *f = NULL;

    snprintf(nombre, sizeof(nombre), "movimientos_%ld.txt", m->dniCliente);
    f = fopen(nombre, "a");
    if (f != NULL) {
        if (fprintf(f, "Mov %d | Alquiler %d | %s | $%.2f | %02d/%02d/%04d | %s\n",
                m->numeroDeMovimiento, m->numeroDeAlquiler, nombreTipo(m->tipoDeMovimiento),
                m->monto, m->fecha.dia, m->fecha.mes, m->fecha.anio, m->patenteDelVehiculo) < 0) {
            fprintf(stderr, "Advertencia: Error al escribir archivo historico TXT.\n");
        }
        fclose(f);
    }
}

int registrarMovimiento(int numeroAlquiler, int tipo, float monto, Fecha fecha, long dni, const char *patente) {
    Movimiento m; 
    FILE *f; 
    int numero = 0;

    if (patente != NULL && monto >= 0.0f && isfinite(monto)) {
        memset(&m, 0, sizeof(m));
        m.numeroDeMovimiento = generarCOD(ARCH_MOVIMIENTOS, sizeof(m), offsetof(Movimiento, numeroDeMovimiento));
        if (m.numeroDeMovimiento > 0) {
            m.numeroDeAlquiler = numeroAlquiler; 
            m.tipoDeMovimiento = tipo; 
            m.monto = monto; 
            m.fecha = fecha; 
            m.dniCliente = dni;
            strncpy(m.patenteDelVehiculo, patente, sizeof(m.patenteDelVehiculo) - 1);

            f = fopen(ARCH_MOVIMIENTOS, "ab"); 
            if (f != NULL) {
                if (fwrite(&m, sizeof(m), 1, f) == 1 && fflush(f) == 0) {
                    numero = m.numeroDeMovimiento;
                }
                fclose(f);

                if (numero > 0) {
                    actualizarArchivoCliente(&m);
                }
            }
        }
    }
    return numero;
}

void listarAlquileresConDeuda(void) {
    FILE *f = fopen(ARCH_ALQUILERES, "rb"); 
    Alquiler a; 
    Cliente c; 
    float saldo = 0.0f;
    int n = 0;

    puts("\n--- ALQUILERES ACTIVOS CON SALDO PENDIENTE ---");
    if (f != NULL) {
        while (fread(&a, sizeof(a), 1, f) == 1) {
            saldo = a.importetotal - a.importepagado;
            /* Se listan exclusivamente contratos ACTIVOS (estado == 1) con deuda real */
            if (a.estado == 1 && saldo > EPSILON) {
                memset(&c, 0, sizeof(c)); 
                buscarCualquierClientePorDNI(a.dnidelcliente, &c);
                printf("DNI: %-10ld | %-15s, %-15s | Alquiler #%d | Auto: %s | Debe: $%.2f\n",
                       a.dnidelcliente, c.apellido, c.nombre, a.numerodealquiler, 
                       a.patenteDelVehiculo, saldo); 
                ++n;
            }
        }
        fclose(f); 
    } else {
        puts("No hay archivo de alquileres registrado.");
    }
    if (!n) puts("No se registran alquileres activos con saldo deudor.");
}

void registrarPago(void) {
    FILE *f = NULL; 
    Alquiler a, sel; 
    long dni = 0, pos = -1; 
    int n = 0, num = 0, mov = 0; 
    float saldo = 0.0f, monto = 0.0f, pagadoAnterior = 0.0f; 
    Fecha hoy;
    int continuar = 1;

    puts("\n=== REGISTRAR PAGO DE ALQUILER ==="); 
    listarAlquileresConDeuda();

    dni = validarLONG("DNI del cliente (0 cancela): "); 
    if (!dni) {
        continuar = 0;
    }

    
    if (continuar) {
        f = fopen(ARCH_ALQUILERES, "rb"); 
        if (f != NULL) {
            while (fread(&a, sizeof(a), 1, f) == 1) {
                saldo = a.importetotal - a.importepagado;
                if (a.dnidelcliente == dni && a.estado == 1 && saldo > EPSILON) {
                    sel = a; 
                    pos = ftell(f) - (long)sizeof(a); 
                    ++n;
                    printf("%d) Alquiler #%d | Patente %s | Total $%.2f | Pagado $%.2f | Saldo Deudor: $%.2f\n",
                           n, a.numerodealquiler, a.patenteDelVehiculo, 
                           a.importetotal, a.importepagado, saldo);
                }
            }
            fclose(f);
        } else {
            puts("No se pudo abrir alquileres.dat."); 
            continuar = 0;
        }
    }

    if (continuar) {
        if (!n) { 
            puts("El cliente no tiene alquileres activos con saldo deudor pendiente."); 
            continuar = 0;
        } else if (pos < 0) { 
            puts("Error al ubicar el contrato en el archivo."); 
            continuar = 0;
        }
    }

    
    if (continuar && n > 1) {
        num = validarINT("Numero de alquiler a pagar (0 cancela): "); 
        if (!num) {
            continuar = 0;
        } else {
            long posTemp = -1;
            Alquiler aTemp;
            if (!buscarAlquilerPorNumero(num, &aTemp, &posTemp) || aTemp.dnidelcliente != dni) {
                puts("Error: El numero de alquiler ingresado no pertenece a este cliente.");
                continuar = 0;
            } else if (aTemp.estado == 2) {
                printf("Aviso: El alquiler #%d ya se encuentra FINALIZADO (el vehiculo fue devuelto y la cuenta saldada; no admite mas pagos).\n", num);
                continuar = 0;
            } else if (aTemp.estado == 3) {
                printf("Aviso: El alquiler #%d se encuentra CANCELADO.\n", num);
                continuar = 0;
            } else if ((aTemp.importetotal - aTemp.importepagado) <= EPSILON) {
                printf("Aviso: El alquiler #%d ya fue pagado en su totalidad ($0.00 de deuda).\n", num);
                continuar = 0;
            } else {
                sel = aTemp;
                pos = posTemp;
            }
        }
    }

    
    if (continuar) {
        saldo = sel.importetotal - sel.importepagado;
        printf("\nAlquiler seleccionado: #%d | Patente: %s\n", sel.numerodealquiler, sel.patenteDelVehiculo);
        printf("Importe Total Contrato : $%.2f\n", sel.importetotal);
        printf("Abonado hasta el momento: $%.2f\n", sel.importepagado);
        printf("Saldo Deudor Pendiente  : $%.2f\n", saldo);

        int montoValido = 0;
        while (!montoValido && continuar) {
            monto = validarFLOAT("Monto que abona el cliente (0 cancela): $"); 
            if (monto == 0.0f) {
                puts("Operacion de pago cancelada.");
                continuar = 0;
            } else if (monto > saldo + EPSILON) { 
                printf("Error: El monto ingresado ($%.2f) excede el saldo pendiente ($%.2f). Ingrese un valor menor o igual.\n", monto, saldo); 
            } else {
                montoValido = 1;
            }
        }
    }

    
    if (continuar) {
        hoy = obtenerFechaActual(); 
        if (!validarFechaStruct(hoy)) { 
            puts("Error en la fecha del sistema."); 
            continuar = 0;
        }
    }

    if (continuar) {
        pagadoAnterior = sel.importepagado; 
        sel.importepagado += monto;

        if (!sobreescribirAlquiler(pos, &sel)) { 
            puts("Error critico al asentar pago en alquileres.dat."); 
        } else {
            mov = registrarMovimiento(sel.numerodealquiler, MOV_PAGO, monto, hoy, dni, sel.patenteDelVehiculo);
            if (!mov) {
                sel.importepagado = pagadoAnterior;
                sobreescribirAlquiler(pos, &sel);
                puts("No se pudo registrar el asiento contable; se revirtio la actualizacion de saldo."); 
            } else {
                printf("\nPago registrado con exito (Movimiento Contable #%d).\n", mov);
                printf("Nuevo saldo restante del alquiler #%d: $%.2f\n", 
                       sel.numerodealquiler, sel.importetotal - sel.importepagado);
            }
        }
    }
}

void mostrarMovimientosCliente(long dni) {
    char nombre[50], linea[200];
    FILE *f = NULL;

    snprintf(nombre, sizeof(nombre), "movimientos_%ld.txt", dni);
    f = fopen(nombre, "r");
    if (f == NULL) {
        printf("\nHistorial: No hay archivo de movimientos previo para el cliente con DNI %ld.\n", dni);
    } else {
        printf("\n--- HISTORIAL DE MOVIMIENTOS (Archivo: %s) ---\n", nombre);
        while (fgets(linea, sizeof(linea), f) != NULL) {
            printf("  %s", linea);
        }
        fclose(f);
    }
}

void consultarMovimientos(void) {
    FILE *f = NULL;
    Movimiento m;
    Cliente c;
    int cant = 0;
    long dni = 0;
    int dniValido = 0;
    int continuar = 1;

    printf("\n===============================================\n");
    printf("            CONSULTAR MOVIMIENTOS              \n");
    printf("===============================================\n");

    
    listarClientesActivosCorto();

    
    while (!dniValido && continuar) {
        dni = validarLONG("DNI del cliente a consultar (0 para cancelar): ");
        
        if (dni == 0) {
            printf("Consulta cancelada por el usuario.\n");
            continuar = 0;
        } else if (!buscarCualquierClientePorDNI(dni, &c)) {
            printf("Error: No existe ningun cliente registrado con el DNI %ld. Reintente.\n", dni);
        } else if (c.estado != 1) {
            printf("Error: El cliente con DNI %ld figura INACTIVO en el sistema. Solo se permite consultar clientes activos.\n", dni);
        } else {
            printf("Cliente seleccionado: %s, %s (DNI: %ld)\n", c.apellido, c.nombre, c.dni);
            dniValido = 1;
        }
    }

    
    if (continuar) {
        f = fopen(ARCH_MOVIMIENTOS, "rb");
        if (f == NULL) {
            printf("No existen movimientos contables registrados en el sistema.\n");
        } else {
            imprimirEncabezadoMov();
            while (fread(&m, sizeof(Movimiento), 1, f) == 1) {
                if (m.dniCliente == dni) {
                    imprimirMovimiento(&m);
                    cant++;
                }
            }
            fclose(f);
            printf("------------------------------------------------------------\n");
            printf("Total de movimientos listados para el cliente: %d\n", cant);
        }
    }
}

void listarPagos(void) {
    FILE *f = fopen(ARCH_MOVIMIENTOS, "rb");
    Movimiento m;
    int cant = 0;
    float total = 0.0f;
    Fecha desde, hasta;
    long cDesde = 0, cHasta = 0, cMov = 0;
    int continuar = 1;

    printf("\n===============================================\n");
    printf("         LISTADO DE PAGOS POR PERIODO          \n");
    printf("===============================================\n");

    if (f == NULL) {
        printf("No hay pagos registrados.\n");
        continuar = 0;
    }

    if (continuar) {
        printf("Fecha Desde:\n");
        desde = leerFechaInicio();
        if (!validarFechaStruct(desde)) { 
            puts("Consulta cancelada."); 
            continuar = 0;
        }
    }

    if (continuar) {
        printf("Fecha Hasta:\n");
        hasta = leerFechaInicio();
        if (!validarFechaStruct(hasta)) { 
            puts("Consulta cancelada."); 
            continuar = 0;
        }
    }

    if (continuar) {
        cDesde = convertirFechaAClave(desde);
        cHasta = convertirFechaAClave(hasta);

        if (cHasta < cDesde) {
            printf("Error: La fecha final es previa a la inicial.\n");
        } else {
            imprimirEncabezadoMov();
            while (fread(&m, sizeof(Movimiento), 1, f) == 1) {
                cMov = convertirFechaAClave(m.fecha);
                if ((m.tipoDeMovimiento == MOV_PAGO || m.tipoDeMovimiento == MOV_ALQUILER) &&
                    cMov >= cDesde && cMov <= cHasta) {
                    imprimirMovimiento(&m);
                    cant++;
                    total += m.monto;
                }
            }
            printf("------------------------------------------------------------\n");
            printf("Total recaudado en el periodo: %d pagos por un total de $%.2f\n", cant, total);
        }
    }

    if (f != NULL) {
        fclose(f);
    }
}

void listarClientesConDeuda(void) {
    FILE *f = fopen(ARCH_ALQUILERES, "rb");
    Alquiler a;
    Cliente c;
    float saldo = 0.0f, totalDeuda = 0.0f;
    char nombreCompleto[120];
    int hay = 0;

    printf("\n========================================================================================\n");
    printf("                        REPORTE DE CLIENTES CON SALDO PENDIENTE                         \n");
    printf("========================================================================================\n");

    if (f != NULL) {
        printf("%-10s %-25s %-9s %-11s %-11s %-11s\n",
               "DNI", "Cliente", "Alquiler", "Total", "Pagado", "Saldo");
        printf("----------------------------------------------------------------------------------------\n");

        while (fread(&a, sizeof(Alquiler), 1, f) == 1) {
            saldo = a.importetotal - a.importepagado;
            if (a.estado != 3 && saldo > EPSILON) {
                memset(&c, 0, sizeof(Cliente));
                buscarCualquierClientePorDNI(a.dnidelcliente, &c);
                if (c.apellido[0] != '\0') {
                    snprintf(nombreCompleto, sizeof(nombreCompleto), "%s, %s", c.apellido, c.nombre);
                } else {
                    strcpy(nombreCompleto, "[Sin datos]");
                }

                printf("%-10ld | %-25s | %-9d | $%-10.2f | $%-10.2f | $%-10.2f\n",
                       a.dnidelcliente, nombreCompleto, a.numerodealquiler,
                       a.importetotal, a.importepagado, saldo);
                totalDeuda += saldo;
                hay = 1;
            }
        }
        fclose(f);
        printf("----------------------------------------------------------------------------------------\n");
        if (!hay) {
            printf("Excelente: No se registran clientes con saldo deudor en el sistema.\n");
        } else {
            printf("Deuda total acumulada a cobrar: $%.2f\n", totalDeuda);
        }
    }
}

void estadisticasEmpresa(void) {
    FILE *fv = fopen(ARCH_VEHICULOS, "rb");
    FILE *fa = fopen(ARCH_ALQUILERES, "rb");
    Vehiculo v;
    Alquiler a;
    int totalVeh = 0, vehDisp = 0, vehAlq = 0;
    int totalAlq = 0, activos = 0;
    float facturado = 0.0f, cobrado = 0.0f;

    if (fv != NULL) {
        while (fread(&v, sizeof(Vehiculo), 1, fv) == 1) {
            totalVeh++;
            if (v.estado == 1) vehDisp++;
            else if (v.estado == 2) vehAlq++;
        }
        fclose(fv);
    }

    if (fa != NULL) {
        while (fread(&a, sizeof(Alquiler), 1, fa) == 1) {
            totalAlq++;
            if (a.estado == 1) activos++;
            cobrado += a.importepagado;
            if (a.estado != 3) {
                facturado += a.importetotal;
            }
        }
        fclose(fa);
    }

    printf("\n=======================================================\n");
    printf("              ESTADISTICAS DE LA EMPRESA               \n");
    printf("=======================================================\n");
    printf("Vehiculos en flota total   : %d\n", totalVeh);
    printf("Vehiculos disponibles      : %d\n", vehDisp);
    printf("Vehiculos alquilados       : %d\n", vehAlq);
    printf("-------------------------------------------------------\n");
    printf("Operaciones de alquiler    : %d\n", totalAlq);
    printf("Alquileres en curso        : %d\n", activos);
    printf("-------------------------------------------------------\n");
    printf("Total facturado pactado    : $%.2f\n", facturado);
    printf("Total efectivamente cobrado: $%.2f\n", cobrado);
    printf("Pendiente de cobro         : $%.2f\n", (facturado > cobrado) ? (facturado - cobrado) : 0.0f);
    printf("=======================================================\n");
}

void menuPagos(void) {
    int op = -1, salir = 0;

    while (!salir) {
    	limpiar();
        printf("\n-----------------------------------------------\n");
        printf("                 GESTION DE PAGOS              \n");
        printf("-----------------------------------------------\n");
        printf("1. Registrar Pago de Alquiler\n");
        printf("2. Consultar Movimientos Contables\n");
        printf("3. Listar Pagos por Periodo (2020-2026)\n");
        printf("0. Volver al Menu Principal\n");
        printf("-----------------------------------------------\n");

        op = validarINT("Seleccione una opcion: ");
        switch (op) {
            case 1:
                uiTransicionEntrada("Registrar Pago");
                registrarPago();
                pausarConsola();
                break;
            case 2:
                uiTransicionEntrada("Movimientos");
                consultarMovimientos();
                pausarConsola();
                break;
            case 3:
                uiTransicionEntrada("Listado de Pagos");
                listarPagos();
                pausarConsola();
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

/* ==============================================================
 * IMPLEMENTACIÓN: REPORTES Y MENÚ PRINCIPAL
 * ============================================================== */

void menuConsultasYReportes(void) {
    int op = -1, salir = 0;

    while (!salir) {
    	limpiar();
        printf("\n===============================================\n");
        printf("             CONSULTAS Y REPORTES              \n");
        printf("===============================================\n");
        printf("1.  Vehiculos Disponibles\n");
        printf("2.  Vehiculos Alquilados\n");
        printf("3.  Alquileres por Rango de Fechas (2020-2026)\n");
        printf("4.  Alquileres por Cliente\n");
        printf("5.  Alquileres por Vehiculo\n");
        printf("6.  Alquileres por Estado\n");
        printf("7.  Alquileres por Importe\n");
        printf("8.  Pagos Realizados por Periodo\n");
        printf("9.  Clientes con Deuda Pendiente\n");
        printf("10. Estadisticas Globales de la Empresa\n");
        printf("0.  Volver al Menu Principal\n");
        printf("===============================================\n");

        op = validarINT("Seleccione una opcion: ");
        switch (op) {
            case 1:
                uiTransicionEntrada("Vehiculos Libres");
                listarVehiculosDisponibles();
                pausarConsola();
                break;
            case 2:
                uiTransicionEntrada("Vehiculos Alquilados");
                listarVehiculosAlquilados();
                pausarConsola();
                break;
            case 3:
                uiTransicionEntrada("Reporte Fechas");
                consultarAlquileresPorRangoFechas();
                pausarConsola();
                break;
            case 4:
                uiTransicionEntrada("Reporte Cliente");
                consultarAlquileresPorCliente();
                pausarConsola();
                break;
            case 5:
                uiTransicionEntrada("Reporte Vehiculo");
                consultarAlquileresPorVehiculo();
                pausarConsola();
                break;
            case 6:
                uiTransicionEntrada("Reporte Estados");
                consultarAlquileresPorEstado();
                pausarConsola();
                break;
            case 7:
                uiTransicionEntrada("Reporte Importes");
                consultarAlquileresPorImporte();
                pausarConsola();
                break;
            case 8:
                uiTransicionEntrada("Reporte Pagos");
                listarPagos();
                pausarConsola();
                break;
            case 9:
                uiTransicionEntrada("Clientes en Deuda");
                listarClientesConDeuda();
                pausarConsola();
                break;
            case 10:
                uiTransicionEntrada("Estadisticas Globales");
                estadisticasEmpresa();
                pausarConsola();
                break;
            case 0:
                salir = 1;
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    }
}

void menuPrincipal(void) {
    int op = -1, salir = 0;

    pantallaCarga();

    while (!salir) {
        limpiar();
        dibujarMarco(10, 2, 70, 14, 11, 0);
        setColor(14, 0);
        gotoxy(22, 3);
        printf("SISTEMA DE ALQUILER DE VEHICULOS");
        setColor(15, 0);

        gotoxy(20, 5);  printf("1. Clientes");
        gotoxy(20, 6);  printf("2. Vehiculos");
        gotoxy(20, 7);  printf("3. Alquileres");
        gotoxy(20, 8);  printf("4. Devoluciones");
        gotoxy(20, 9);  printf("5. Pagos");
        gotoxy(20, 10); printf("6. Consultas y Reportes");
        gotoxy(20, 11); printf("0. Salir");

        gotoxy(16, 16);
        op = validarINT("Seleccione una opcion: ");

        switch (op) {
            case 1:
                uiTransicionEntrada("Modulo Clientes");
                menuClientes();
                break;
            case 2:
                uiTransicionEntrada("Modulo Vehiculos");
                menuVehiculos();
                break;
            case 3:
                uiTransicionEntrada("Modulo Alquileres");
                menuAlquileres();
                break;
            case 4:
                uiTransicionEntrada("Modulo Devoluciones");
                menuDevoluciones();
                break;
            case 5:
                uiTransicionEntrada("Modulo Pagos");
                menuPagos();
                break;
            case 6:
                uiTransicionEntrada("Consultas y Reportes");
                menuConsultasYReportes();
                break;
            case 0:
                pantallaSalida();
                salir = 1;
                break;
            default:
                printf("\nOpcion invalida. Reintente.\n");
                pausarConsola();
                break;
        }
    }
}

#endif /* FUNCIONES_H */