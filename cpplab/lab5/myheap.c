#include "myheap.h"
#include <stdlib.h>
#include <assert.h>

#define LEFT_CHILD(i)  ((i) * 2 + 1)
#define RIGHT_CHILD(i) ((i) * 2 + 2)
#define PARENT(i)      (((i) - 1) / 2)

void init_heap(heap_t *h)
{
    int i;
    h->curr_max = 0;
    h->head = (float *)malloc(HEAP_SIZE * sizeof(float));
    for (i = 0; i < HEAP_SIZE; i++) {
        h->head[i] = -1;
    }
}

void add_value(heap_t *h, float v)
{
    assert(h->curr_max < HEAP_SIZE);
    h->head[h->curr_max++] = v;
    upward(h);
}

float get_first_value(heap_t *h)
{
    assert(h->curr_max > 0);
    float result = h->head[0];
    swap(h->head, 0, h->curr_max - 1);
    h->curr_max--;
    downward(h);
    return result;
}

float get_top_value(heap_t *h)
{
    assert(h->curr_max > 0);
    return h->head[0];
}

void upward(heap_t *h)
{
    int i = h->curr_max - 1;
    while (i > 0 && h->head[i] < h->head[PARENT(i)]) {
        swap(h->head, i, PARENT(i));
        i = PARENT(i);
    }
}

void downward(heap_t *h)
{
    int i = 0;
    int t;
    while (LEFT_CHILD(i) < h->curr_max) {
        if (RIGHT_CHILD(i) < h->curr_max && h->head[RIGHT_CHILD(i)] < h->head[LEFT_CHILD(i)]) {
            t = RIGHT_CHILD(i);
        } else {
            t = LEFT_CHILD(i);
        }
        if (h->head[i] < h->head[t]) break;
        swap(h->head, i, t);
        i = t;
    }
}

void swap(float *p, int i, int j)
{
    int t = p[i];
    p[i] = p[j];
    p[j] = t;
}
