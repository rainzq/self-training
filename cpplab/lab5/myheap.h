#define HEAP_SIZE 100
struct heap_t {
    int curr_max; // current valid elements in heap
    float *head;  
};
typedef struct heap_t heap_t;

void init_heap(heap_t *h);

void add_value(heap_t *h, float v);

float get_first_value(heap_t *h);

void upward(heap_t *h);

void downward(heap_t *h);

void swap(float *p, int i, int j);

float get_top_value(heap_t *h);
