#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include "heap.hh"

using namespace std;

#define HEAP_SIZE 32
/* Test the heap. */
int main(int argc, char **argv)
{
    int i;
    Heap<float, 32> h;
    float lastval;


    /* Use a seed so that the sequence of random values
     * is always the same.
     */
    srand(11);

    /* Fill up the heap with random values. */
    for (i = 0; i < HEAP_SIZE; i++) {
        try {
            h.add_value(rand() % 1000);
        } catch (const underflow_error &e) {
            cerr << "Warning: " << e.what() << endl;
            return 0;
        } catch (...) { return 0;}
    }

    /*
     * Print out the results.  If anything is out of order,
     * flag it.  (Pull the very first value separately so that
     * we don't have to set lastval to something strange just
     * to get the test to work...)
     */

    try {
        lastval = h.get_first_value();
    } catch (const overflow_error &e) {
        cerr << "Warning: " << e.what() << endl;
    } catch (...) {return 0;}

    printf("Value 0 = %f\n", lastval);

    float val;
    for (i = 1; i < HEAP_SIZE; i++)
    {
        try {
            val = h.get_first_value();
        } catch (const overflow_error &e) {
            cerr << "Warning: " << e.what() << endl;
            return 0;
        } catch (...) {return 0;}

        printf("Value %d = %f\n", i, val);
        if (val < lastval)
            printf("  ERROR:  OUT OF ORDER.\n");

        lastval = val;
    }

    return 0;
}

