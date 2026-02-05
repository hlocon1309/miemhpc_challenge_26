#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <omp.h>

#define CHUNK_SIZE (1024 * 1024 * 64) // 64 млн чисел (~256 МБ RAM на один поток)
#define MAX_RUNS 1024

typedef struct {
    uint32_t value;
    int run_idx;
} HeapNode;

// Функция сравнения для быстрой сортировки
int compare_uint32(const void* a, const void* b) {
    uint32_t arg1 = *(const uint32_t*)a, arg2 = *(const uint32_t*)b;
    return (arg1 > arg2) - (arg1 < arg2);
}

// Управление Min-Heap для слияния
void heapify(HeapNode* heap, int size, int i) {
    int smallest = i, l = 2 * i + 1, r = 2 * i + 2;
    if (l < size && heap[l].value < heap[smallest].value) smallest = l;
    if (r < size && heap[r].value < heap[smallest].value) smallest = r;
    if (smallest != i) {
        HeapNode temp = heap[i]; heap[i] = heap[smallest]; heap[smallest] = temp;
        heapify(heap, size, smallest);
    }
}

int main() {
    FILE *fin = fopen("input.txt", "r"), *fout = fopen("output.txt", "w");
    uint32_t n;
    if (fscanf(fin, "%u", &n) != 1) return 1;

    // --- ФАЗА 1: ГЕНЕРАЦИЯ ОРСОРТИРОВАННЫХ СЕРИЙ (RUNS) ---
    int run_count = 0;
    uint32_t* buffer = malloc(CHUNK_SIZE * sizeof(uint32_t));
    
    while (n > 0) {
        uint32_t count = (n < CHUNK_SIZE) ? n : CHUNK_SIZE;
        for (uint32_t i = 0; i < count; i++) fscanf(fin, "%u", &buffer[i]);
        qsort(buffer, count, sizeof(uint32_t), compare_uint32);
        char filename[32]; sprintf(filename, "run_%d.bin", run_count++);
        FILE* tmp = fopen(filename, "wb");
        fwrite(buffer, sizeof(uint32_t), count, tmp); // Бинарная запись быстрее текстовой
        fclose(tmp);
        n -= count;
    }
    free(buffer); fclose(fin);

    // --- ФАЗА 2: СЛИЯНИЕ K-ПУТЕЙ ЧЕРЕЗ MIN-HEAP ---
    FILE* runs[MAX_RUNS];
    HeapNode heap[MAX_RUNS];
    int active_runs = 0;

    for (int i = 0; i < run_count; i++) {
        char filename[32]; sprintf(filename, "run_%d.bin", i);
        runs[active_runs] = fopen(filename, "rb");
        if (fread(&heap[active_runs].value, sizeof(uint32_t), 1, runs[active_runs])) {
            heap[active_runs].run_idx = active_runs;
            active_runs++;
        }
    }

    // Строим начальную кучу (O(K))
    for (int i = (active_runs - 2) / 2; i >= 0; i--) heapify(heap, active_runs, i);

    while (active_runs > 0) {
        // Пишем минимальный элемент в итоговый текстовый файл
        fprintf(fout, "%u%s", heap[0].value, (active_runs == 1 && feof(runs[heap[0].run_idx])) ? "" : " ");
        
        // Читаем следующее число из того же файла
        if (fread(&heap[0].value, sizeof(uint32_t), 1, runs[heap[0].run_idx])) {
            heapify(heap, active_runs, 0); // O(log K)
        } else {
            // Файл закончился, удаляем его из кучи
            fclose(runs[heap[0].run_idx]);
            heap[0] = heap[--active_runs];
            heapify(heap, active_runs, 0);
        }
    }
    fclose(fout);
    return 0;
}