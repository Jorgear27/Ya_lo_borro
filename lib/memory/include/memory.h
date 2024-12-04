/**
 * @file memory.h
 * @brief Memory management library with custom allocation functions.
 *
 * Esta biblioteca define funciones de asignación de memoria dinámica
 * y gestión de bloques para crear un asignador de memoria personalizado.
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief Macro para alinear una cantidad de bytes al siguiente múltiplo de 8.
 *
 * @param x Cantidad de bytes a alinear.
 */
#define align(x) (((((x)-1) >> 3) << 3) + 8)

/** Tamaño mínimo de un bloque de memoria. */
#define BLOCK_SIZE 40
/** Tamaño de página en memoria. */
#define PAGESIZE 4096
/** Política de asignación First Fit. */
#define FIRST_FIT 0
/** Política de asignación Best Fit. */
#define BEST_FIT 1
/** Política de asignación Worst Fit. */
#define WORST_FIT 2
/** Tamaño del bloque */
#define DATA_START 1
/** Número máximo de operaciones de asignación y liberación de memoria. */
#define MAX_OPERATIONS 100000
/** Nombre del archivo de registro de operaciones. */
#define LOG_FILE getenv("LOG_FILE_PATH")

/** Puntero al primer bloque de memoria. */
extern void* base;

/**
 * @struct s_block
 * @brief Estructura para representar un bloque de memoria.
 *
 * Contiene la información necesaria para gestionar la asignación y
 * liberación de un bloque de memoria.
 */
struct s_block
{
    size_t size;           /**< Tamaño del bloque de datos. */
    struct s_block* next;  /**< Puntero al siguiente bloque en la lista enlazada. */
    struct s_block* prev;  /**< Puntero al bloque anterior en la lista enlazada. */
    int free;              /**< Indicador de si el bloque está libre (1) o ocupado (0). */
    void* ptr;             /**< Puntero a la dirección de los datos almacenados. */
    char data[DATA_START]; /**< Área donde comienzan los datos del bloque. */
};

/** Tipo de puntero para un bloque de memoria. */
typedef struct s_block* t_block;

/**
 * @struct Operation
 * @brief Estructura para representar una operación de asignación o liberación de memoria.
 *
 * Contiene información sobre el tipo de operación, la dirección de memoria
 * asignada o liberada y el tamaño de la memoria asignada o liberada.
 */
struct Operation
{
    char type[10]; /**< Tipo de operación (malloc, free, etc.). */
    void* address; /**< Dirección de memoria asignada o liberada. */
    size_t size;   /**< Tamaño de la memoria asignada o liberada. */
};

/** Arreglo de operaciones de asignación y liberación de memoria. */
typedef struct Operation Operation;

/**
 * @brief Obtiene el bloque que contiene una dirección de memoria dada.
 *
 * @param p Puntero a la dirección de datos.
 * @return t_block Puntero al bloque de memoria correspondiente.
 */
t_block get_block(void* p);

/**
 * @brief Verifica si una dirección de memoria es válida.
 *
 * @param p Dirección de memoria a verificar.
 * @return int Retorna 1 si la dirección es válida, 0 en caso contrario.
 */
int valid_addr(void* p);

/**
 * @brief Encuentra un bloque libre que tenga al menos el tamaño solicitado.
 *
 * @param last Puntero al último bloque.
 * @param size Tamaño solicitado.
 * @return t_block Puntero al bloque encontrado, o NULL si no se encuentra
 * ninguno.
 */
t_block find_block(t_block* last, size_t size);

/**
 * @brief Expande el heap para crear un nuevo bloque de memoria.
 *
 * @param last Último bloque del heap.
 * @param s Tamaño del nuevo bloque.
 * @return t_block Puntero al nuevo bloque creado.
 */
t_block extend_heap(t_block last, size_t s);

/**
 * @brief Divide un bloque de memoria en dos, si el tamaño solicitado es menor
 * que el bloque disponible.
 *
 * @param b Bloque a dividir.
 * @param s Tamaño del nuevo bloque.
 */
void split_block(t_block b, size_t s);

/**
 * @brief Fusiona un bloque libre con su siguiente bloque si también está libre.
 *
 * @param b Bloque a fusionar.
 * @return t_block Puntero al bloque fusionado.
 */
t_block fusion(t_block b);

/**
 * @brief Copia el contenido de un bloque de origen a un bloque de destino.
 *
 * @param src Bloque de origen.
 * @param dst Bloque de destino.
 */
void copy_block(t_block src, t_block dst);

/**
 * @brief Asigna un bloque de memoria del tamaño solicitado.
 *
 * @param size Tamaño en bytes del bloque a asignar.
 * @return void* Puntero al área de datos asignada.
 */
void* malloc(size_t size);

/**
 * @brief Libera un bloque de memoria previamente asignado.
 *
 * @param p Puntero al área de datos a liberar.
 */
void free(void* p);

/**
 * @brief Asigna un bloque de memoria para un número de elementos,
 * inicializándolo a cero.
 *
 * @param number Número de elementos.
 * @param size Tamaño de cada elemento.
 * @return void* Puntero al área de datos asignada e inicializada.
 */
void* calloc(size_t number, size_t size);

/**
 * @brief Cambia el tamaño de un bloque de memoria previamente asignado.
 *
 * @param p Puntero al área de datos a redimensionar.
 * @param size Nuevo tamaño en bytes.
 * @return void* Puntero al área de datos redimensionada.
 */
void* realloc(void* p, size_t size);

/**
 * @brief Verifica el estado del heap y detecta bloques libres consecutivos.
 *
 * @param data Información adicional para la verificación.
 */
void check_heap(void* data);

/**
 * @brief Configura el modo de asignación de memoria (First Fit o Best Fit).
 *
 * @param mode Modo de asignación (0 para First Fit, 1 para Best Fit).
 */
void malloc_control(int mode);

/**
 * @brief Obtiene el modo de asignación de memoria actual.
 *
 * @param m Modo de asignación actual.
 */
void get_method(int m);

/**
 * @brief Imprime el estado actual de la memoria.
 *
 * @param allocated Memoria asignada.
 * @param free Memoria libre.
 *
 */
void memory_usage(size_t* allocated, size_t* free);

/**
 * @brief Registra una operación de asignación o liberación de memoria.
 *
 * @param operation Tipo de operación.
 * @param size Tamaño de la memoria asignada o liberada.
 * @param ptr Dirección de memoria asignada o liberada.
 */
void log_operation(const char* operation, size_t size, void* ptr);

/**
 * @brief Borra todos los bloques de memoria asignados.
 *
 */
void clear_all_blocks(void);
