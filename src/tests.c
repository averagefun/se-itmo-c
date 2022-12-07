#include "tests.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mem.h"
#include "mem_internals.h"

#define DEFAULT_OUTPUT stdout

static void heap_destroy(void* heap, size_t sz) {
    munmap(heap, size_from_capacity((block_capacity){.bytes = sz}).bytes);
}

void print_heap(void const* ptr, char* message) {
    printf("* %s:\n", message);
    debug_heap(DEFAULT_OUTPUT, ptr);
}

static struct block_header* get_block_header(void* malloc_addr) {
    return (struct block_header*)(malloc_addr -
                                  offsetof(struct block_header, contents));
}

bool test_malloc() {
    printf("[[ Malloc test ]]\n");

    // initialize heap
    size_t init_heap_size = 10000;
    void* heap = heap_init(init_heap_size);
    size_t real_heap_size = ((struct block_header*)heap)->capacity.bytes;
    print_heap(heap, "Init heap");
    if (!heap) return false;

    size_t malloc_size = real_heap_size / 4;
    void* malloc_addr = _malloc(malloc_size);
    print_heap(heap, "Malloc");
    if (!malloc_addr) return false;
    struct block_header* block = get_block_header(malloc_addr);
    if (block->capacity.bytes != malloc_size) return false;

    _free(malloc_addr);
    print_heap(heap, "Free");

    heap_destroy(heap, real_heap_size);
    return true;
}

bool test_malloc_one_free() {
    printf("[[ Malloc some blocks and free one random ]]\n");

    srand(time(NULL));

    // initialize heap
    size_t init_heap_size = 10000;
    void* heap = heap_init(init_heap_size);
    size_t real_heap_size = ((struct block_header*)heap)->capacity.bytes;
    print_heap(heap, "Init heap");
    if (!heap) return false;

    size_t malloc_numbers = 5;
    void* malloc_addrs[malloc_numbers];
    for (size_t i = 0; i < malloc_numbers; i++) {
        size_t malloc_size = real_heap_size * (i + 1) / 50;
        void* malloc_addr = _malloc(malloc_size);
        malloc_addrs[i] = malloc_addr;

        if (!malloc_addr) return false;
        struct block_header* block = get_block_header(malloc_addr);
        if (block->capacity.bytes != malloc_size) return false;
    }
    print_heap(heap, "Malloc");

    size_t r = rand() % malloc_numbers;
    _free(malloc_addrs[r]);
    print_heap(heap, "Free random malloc");
    struct block_header* block = get_block_header(malloc_addrs[r]);
    if (!block->is_free) {
        return false;
    }

    heap_destroy(heap, real_heap_size);
    return true;
}

bool test_malloc_two_free() {
    printf("[[ Malloc some blocks and free two random ]]\n");

    srand(time(NULL));

    // initialize heap
    size_t init_heap_size = 10000;
    void* heap = heap_init(init_heap_size);
    size_t real_heap_size = ((struct block_header*)heap)->capacity.bytes;
    print_heap(heap, "Init heap");
    if (!heap) return false;

    size_t malloc_numbers = 6;
    void* malloc_addrs[malloc_numbers];
    for (size_t i = 0; i < malloc_numbers; i++) {
        size_t malloc_size = real_heap_size * (i + 1) / 50;
        void* malloc_addr = _malloc(malloc_size);
        malloc_addrs[i] = malloc_addr;

        if (!malloc_addr) return false;
        struct block_header* block = get_block_header(malloc_addr);
        if (block->capacity.bytes != malloc_size) return false;
    }
    print_heap(heap, "Malloc");

    for (int i = 0; i < 2; i++) {
        size_t r = rand() % malloc_numbers;
        _free(malloc_addrs[r]);
        struct block_header* block = get_block_header(malloc_addrs[r]);
        if (!block->is_free) {
            return false;
        }
    }

    print_heap(heap, "Free 2 random mallocs");

    heap_destroy(heap, real_heap_size);
    return true;
}

bool test_malloc_new_region_extended() {
    printf("[[ Malloc new extended region ]]\n");

    // initialize heap
    size_t init_heap_size = 10000;
    void* heap = heap_init(init_heap_size);
    size_t real_heap_size = ((struct block_header*)heap)->capacity.bytes;
    print_heap(heap, "Init heap");
    if (!heap) return false;

    size_t malloc_size = real_heap_size * 2;
    void* malloc_addr = _malloc(malloc_size);
    print_heap(heap, "Malloc");
    if (!malloc_addr) return false;
    struct block_header* block = get_block_header(malloc_addr);
    if (block->capacity.bytes != malloc_size) return false;

    _free(malloc_addr);
    print_heap(heap, "Free");

    heap_destroy(heap, real_heap_size);
    return true;
}

bool test_malloc_new_region_not_extended() {
    printf("[[ Malloc new not extended region ]]\n");

    // initialize heap
    size_t init_heap_size = 10000;
    void* heap = heap_init(init_heap_size);

    struct block_header* heap_block = (struct block_header*)heap;
    size_t real_heap_size = heap_block->capacity.bytes;
    print_heap(heap, "Init heap");
    if (!heap) return false;

    void* region_brake = mmap(heap, REGION_MIN_SIZE * 5, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_FIXED, -1, 0);

    size_t malloc_size = real_heap_size * 2;
    void* malloc_addr = _malloc(malloc_size);
    print_heap(heap, "Malloc");
    if (!malloc_addr) return false;
    struct block_header* block = get_block_header(malloc_addr);
    if (!heap_block->is_free || block->is_free) return false;

    _free(malloc_addr);
    print_heap(heap, "Free");

    heap_destroy(heap, real_heap_size);
    return true;
}

predicate global_tests[] = {
    test_malloc, test_malloc_one_free, test_malloc_two_free,
    test_malloc_new_region_extended, test_malloc_new_region_not_extended};