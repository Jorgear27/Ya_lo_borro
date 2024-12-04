#include "memory.h"
#include <sys/mman.h>

typedef struct s_block* t_block;
void* base = NULL;
int method = 0;

t_block find_block(t_block* last, size_t size)
{
    t_block b = base;

    if (method == FIRST_FIT)
    {
        while (b && !(b->free && b->size >= size))
        {
            *last = b;
            b = b->next;
        }
        return (b);
    }
    else if (method == BEST_FIT)
    {
        size_t dif = PAGESIZE;
        t_block best = NULL;

        while (b)
        {
            if (b->free)
            {
                if (b->size == size)
                {
                    return b;
                }
                if (b->size > size && (b->size - size) < dif)
                {
                    dif = b->size - size;
                    best = b;
                }
            }
            *last = b;
            b = b->next;
        }
        return best;
    }
    else if (method == WORST_FIT)
    {
        size_t max_size = 0;
        t_block worst = NULL;

        while (b)
        {
            if (b->free && b->size >= size && b->size > max_size)
            {
                max_size = b->size;
                worst = b;
            }
            *last = b;
            b = b->next;
        }
        return worst;
    }
    return NULL;
}

void copy_block(t_block src, t_block dst)
{
    int *sdata, *ddata;
    size_t i;
    sdata = src->ptr;
    ddata = dst->ptr;
    for (i = 0; i * 4 < src->size && i * 4 < dst->size; i++)
        ddata[i] = sdata[i];
}

t_block get_block(void* p)
{
    char* tmp = NULL;
    tmp = p;

    return (t_block)(tmp -= BLOCK_SIZE);
}

int valid_addr(void* p)
{
    if (base)
    {
        if (p > base && p < sbrk(0))
            return (p == &((get_block(p))->data));
    }

    return (0);
}

void split_block(t_block b, size_t s)
{
    t_block new;
    new = (t_block)(b->data + s);
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->prev = b;
    new->free = 1;
    new->ptr = new->data;
    b->size = s;
    b->next = new;

    if (new->next)
        new->next->prev = new;
}

t_block fusion(t_block b)
{
    while (b->next && b->next->free)
    {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next)
        {
            b->next->prev = b;
        }
    }
    if (!b->next)
    {
        if (b->prev)
            b->prev->next = NULL;
        else
            base = NULL;
        brk(b);
    }
    return b;
}

t_block extend_heap(t_block last, size_t s)
{
    t_block b;

    b = sbrk(0);

    if (sbrk(BLOCK_SIZE + (long int)s) == (void*)-1)
        return (NULL);

    b->ptr = b->data;
    b->size = s;
    b->prev = last;
    b->next = NULL;

    if (last)
        last->next = b;

    b->free = 0;
    return (b);
}

void get_method(int m)
{
    method = m;
}

void malloc_control(int m)
{
    if (m == FIRST_FIT || m == BEST_FIT || m == WORST_FIT)
    {
        method = m;
    }
    else
    {
        printf("Invalid method\n");
    }
}

void* malloc(size_t size)
{
    t_block b;
    t_block last;
    size_t s;

    s = align(size);

    if (!s)
        return NULL;

    if (base)
    {
        /* First find a block */
        last = base;
        b = find_block(&last, s);
        if (b)
        {
            /* Can we split */
            if ((b->size - s) >= (BLOCK_SIZE + 4))
                split_block(b, s);

            b->free = 0;
        }
        else
        {
            /* No fitting block, extend the heap */
            b = extend_heap(last, s);
            if (!b)
                return NULL;
        }
    }
    else
    {
        /* First time */
        b = extend_heap(NULL, s);
        if (!b)
            return NULL;
        base = b;
    }

    // log_operation("malloc", size, b->data);
    return b->data;
}

void free(void* p)
{
    t_block b;
    if (valid_addr(p))
    {
        b = get_block(p);
        b->free = 1;
        if (b->prev && b->prev->free)
        {
            b = fusion(b->prev);
        }
        if (b->next)
        {
            fusion(b);
        }
        else
        {
            if (b->prev)
                b->prev->next = NULL;
            else
                base = NULL;
            brk(b);
        }
        // log_operation("free", 0, p);
    }
}

void* calloc(size_t number, size_t size)
{
    size_t total_size = number * size;
    void* ptr = malloc(total_size);
    if (ptr)
    {
        memset(ptr, 0, total_size);
        // log_operation("calloc", total_size, ptr);
    }
    return ptr;
}

void* realloc(void* p, size_t size)
{
    size_t s;
    t_block b, new;
    void* newp;

    if (!p)
        return malloc(size);

    if (valid_addr(p))
    {
        s = align(size);
        b = get_block(p);
        if (b->size >= s)
        {
            if (b->size - s >= (BLOCK_SIZE + 4))
                split_block(b, s);
        }
        else
        {
            // Fusionamos con el siguiente si es posible
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b);
                if (b->size - s >= (BLOCK_SIZE + 4))
                    split_block(b, s);
            }
            else
            {
                // No hay bloques libres de espacio suficiente, malloc y luego free
                newp = malloc(s);
                if (!newp)
                    return NULL;
                new = get_block(newp);
                // Copiamos los datos
                copy_block(b, new);
                // Liberamos el bloque anterior
                free(p);
                // log_operation("realloc", size, newp);
                return (newp);
            }
        }
        // log_operation("realloc", size, p);
        return (p);
    }
    return NULL;
}

void check_heap(void* data)
{
    if (data == NULL)
    {
        printf("Data is NULL\n");
        return;
    }

    t_block block = get_block(data);

    if (block == NULL)
    {
        printf("Block is NULL\n");
        return;
    }

    printf("\033[1;33mHeap check\033[0m\n");
    printf("Size: %zu\n", block->size);

    if (block->next != NULL)
    {
        printf("Next block: %p\n", (void*)(block->next));
    }
    else
    {
        printf("Next block: NULL\n");
    }

    if (block->prev != NULL)
    {
        printf("Prev block: %p\n", (void*)(block->prev));
    }
    else
    {
        printf("Prev block: NULL\n");
    }

    printf("Free: %d\n", block->free);

    if (block->ptr != NULL)
    {
        printf("Beginning data address: %p\n", block->ptr);
        printf("Last data address: %p\n", (void*)((char*)(block->ptr) + block->size));
    }
    else
    {
        printf("Data address: NULL\n");
    }

    printf("Heap address: %p\n", sbrk(0));

    // Checks adicionales para detectar inconsistencias
    t_block current = base;
    while (current)
    {
        // Verificamos que los bloques libres adyacentes estén fusionados
        if (current->free && current->next && current->next->free)
        {
            printf("\033[1;31mInconsistency detected: Adjacent free blocks not used at %p and %p\033[0m\n",
                   (void*)current, (void*)(current->next));
        }

        // Verificamos que el tamaño del bloque sea válido
        if (current->size <= 0)
        {
            printf("\033[1;31mInconsistency detected: Invalid block size at %p\033[0m\n", (void*)current);
        }

        current = current->next;
    }
}

void memory_usage(size_t* allocated, size_t* free)
{
    t_block b = base;
    *allocated = 0;
    *free = 0;

    while (b)
    {
        if (b->free)
        {
            *free += b->size;
        }
        else
        {
            *allocated += b->size;
        }
        b = b->next;
    }
}

void log_operation(const char* operation, size_t size, void* ptr)
{
    static int logging = 0;

    if (logging)
    {
        return;
    }

    logging = 1;

    FILE* log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL)
    {
        perror("Error opening log file");
        logging = 0;
        return;
    }

    time_t now = time(NULL);
    char* timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0'; // Remove newline character

    fprintf(log_file, "[%s] Operation: %s, Size: %zu, Pointer: %p\n", timestamp, operation, size, ptr);
    fclose(log_file);

    logging = 0;
}

void clear_all_blocks()
{
    t_block b = base;
    t_block next;

    while (b)
    {
        next = b->next;
        b->free = 1; // Marca el bloque como libre
        b = next;
    }

    base = NULL; // Reinicia la base
}
