/**************************************************************
 * TPI PE 2026 - Sistema de Gestión de Alquiler de Vehículos
 * Archivo: structs.h
 **************************************************************/

#ifndef STRUCTS_H
#define STRUCTS_H

// Tipos de movimientos económicos 
#define MOV_ALQUILER        1
#define MOV_PAGO            2
#define MOV_DEVOLUCION      3
#define MOV_AJUSTE          4
/*

MOV_ALQUILER (1) — Alta operativa del contrato:
Representa el hecho económico inicial del alquiler. Registra la creación del contrato junto con el importe total pactado en base a los días previstos de uso.

MOV_PAGO (2) — Ingreso real de dinero:
Registra cualquier entrada de dinero en efectivo o transferencia. Se genera en tres instancias:

1-Al iniciar el alquiler, si el cliente abona una seña parcial o el 100% del total.

2-En pagos intermedios registrados voluntariamente mediante la opción Registrar Pago.

3-En la devolución efectiva del vehículo, cuando el cliente cancela el saldo restante (sea el remanente original o el resultante de días adicionales por mora).

MOV_DEVOLUCION (3) — Cierre operativo y restitución del bien:
Es un movimiento administrativo de importe $0.00. Certifica la entrega física del vehículo en la fecha actual, el cambio del auto a estado 1 (Disponible) y el pase del contrato a estado 2 (Finalizado).

MOV_AJUSTE (4) — Corrección contable o reintegro:
Registra variaciones sobre el precio base cuando la devolución no ocurre en el plazo exacto:

Devolución tardía: Puede emplearse para asentar contablemente el cargo extra por mora generado antes de exigir el cobro final.

*/

// Manejo de fechas 
typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

//Domicilio de clientes 
typedef struct {
    char calle[50];
    int numeracion;
} Direccion;



// Entidad Cliente 
typedef struct {
    long dni;
    char apellido[50];
    char nombre[50];
    char email[100];
    char telefono[20];
    Direccion direccion;
    Fecha fechadealta;
    short estado; // 1: Activo, 0: Inactivo 
} Cliente;

// Entidad Vehículo 
typedef struct {
    char patente[8];          // 7 caracteres útiles + '\0' 
    char marca[21];              
    char modelo[31];
    int anio;
    char tipodevehiculo[31];
    int capacidad;
    float preciopordia;
    int estado; // 1: Disponible, 2: Alquilado, 3: Baja, 4: En mantenimiento 
} Vehiculo;

// Entidad Alquiler 
typedef struct {
    int numerodealquiler;
    long dnidelcliente;
    char patenteDelVehiculo[8];
    Fecha fechadeinicio;
    Fecha fechaprevistadedevolucion;
    Fecha fechaefectivadedevolucion;
    int cantdedias;
    float importetotal;
    float importepagado;
    int estado; // 1: Activo, 2: Finalizado, 3: Cancelado 
} Alquiler;

// Entidad Movimiento 
typedef struct {
    int numeroDeMovimiento;
    int numeroDeAlquiler;
    int tipoDeMovimiento; // 1: Alquiler, 2: Pago, 3: Devolucion, 4: Ajuste 
    float monto;
    Fecha fecha;
    long dniCliente;
    char patenteDelVehiculo[8];
} Movimiento;

#endif /* STRUCTS_H */