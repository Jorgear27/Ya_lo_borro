#include "policies_stats.h"

int main() {
    while(1){
        // Crear el objeto JSON
        cJSON *json_obj = cJSON_CreateObject();

        // Abrir el archivo para escritura
        FILE *file = fopen(JSON_PATH, "w");
        if (file == NULL) {
            perror("fopen");
            return 1;
        }

        // Test FIRST_FIT policy
        PolicyStats first_fit_stats = test_policy(FIRST_FIT, "FIRST FIT");
        cJSON *first_fit_obj = cJSON_CreateObject();
        cJSON_AddItemToObject(first_fit_obj, "time", cJSON_CreateNumber(first_fit_stats.time));
        cJSON_AddItemToObject(first_fit_obj, "fragmentation", cJSON_CreateNumber(first_fit_stats.fragmentation));
        cJSON_AddItemToObject(json_obj, "FIRST_FIT", first_fit_obj);

        // Test BEST_FIT policy
        PolicyStats best_fit_stats = test_policy(BEST_FIT, "BEST FIT");
        cJSON *best_fit_obj = cJSON_CreateObject();
        cJSON_AddItemToObject(best_fit_obj, "time", cJSON_CreateNumber(best_fit_stats.time));
        cJSON_AddItemToObject(best_fit_obj, "fragmentation", cJSON_CreateNumber(best_fit_stats.fragmentation));
        cJSON_AddItemToObject(json_obj, "BEST_FIT", best_fit_obj);

        // Test WORST_FIT policy
        PolicyStats worst_fit_stats = test_policy(WORST_FIT, "WORST FIT");
        cJSON *worst_fit_obj = cJSON_CreateObject();
        cJSON_AddItemToObject(worst_fit_obj, "time", cJSON_CreateNumber(worst_fit_stats.time));
        cJSON_AddItemToObject(worst_fit_obj, "fragmentation", cJSON_CreateNumber(worst_fit_stats.fragmentation));
        cJSON_AddItemToObject(json_obj, "WORST_FIT", worst_fit_obj);

        // Escribir el objeto JSON en el archivo
        char *json_string = cJSON_Print(json_obj);
        fprintf(file, "%s", json_string);
        free(json_string);

        // Cerrar el archivo
        fclose(file);

        // Liberar el objeto JSON
        cJSON_Delete(json_obj);

        sleep(1);
    }

    return 0;
}

PolicyStats test_policy(int policy, const char* policy_name) {
    malloc_control(policy);

    void* allocations[NUM_ALLOCATIONS] = {0};
    size_t allocated = 0, total_free = 0, total_memory = 0;
    int num_free_blocks = 0, total_blocks = 0;
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
        allocations[i] = malloc(size);
        if (allocations[i]) {
            allocated += size;
        }
    }

    for (int i = 0; i < NUM_ALLOCATIONS / 2; i++) {
        int index = rand() % NUM_ALLOCATIONS;
        if (allocations[index]) {
            free(allocations[index]);
            allocations[index] = NULL;
        }
    }

    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        if (allocations[i] == NULL) {
            size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
            allocations[i] = malloc(size);
            if (allocations[i]) {
                allocated += size;
            }
        }
    }

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    memory_usage(&allocated, &total_free);

    // Calculate the number of free blocks and total blocks
    t_block b = base;
    while (b) {
        total_blocks++;
        if (b->free) {
            num_free_blocks++;
            total_free += b->size;
        }
        total_memory += b->size;
        b = b->next;
    }

    double fragmentation = calculate_fragmentation(total_free, total_memory, num_free_blocks, total_blocks);

    printf("%s - TIME: %f seconds, FRAGMENTATION: %f\n\n", policy_name, cpu_time_used, fragmentation);

    for (int i = 0; i < NUM_ALLOCATIONS; i++) {
        if (allocations[i]) {
            free(allocations[i]);
        }
    }

    PolicyStats stats = {cpu_time_used, fragmentation};
    return stats;
}

double calculate_fragmentation(size_t total_free, size_t total_memory, int num_free_blocks, int total_blocks) {
    double block_fragmentation = (double)num_free_blocks / total_blocks * 100;
    double space_fragmentation = (double)total_free / total_memory * 100;
    return block_fragmentation * 0.5 + space_fragmentation * 0.5;
}
