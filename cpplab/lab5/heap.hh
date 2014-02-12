#include <stdexcept>

template<typename T, int size>
class Heap {
    private:
        T values[size];
        int num_values; // number of valid elements in heap

    public:
        Heap(): num_values(0) {}
        // add a value in the heap
        void add_value(const T &v) throw (std::overflow_error)
        {
            if (num_values == size) {
                throw std::overflow_error("heap is full");
            }
            // add to the tail of values 
            // then adjust to keep in heap order
            values[num_values++] = v;
            sift_up();
        }
        // get the first value of the heap
        // and then delete it
        T get_first_value() throw (std::underflow_error)
        {
            if (num_values == 0) {
                throw std::underflow_error("heap is empty");
            }
            T result = values[0];
            // overwrite the first element using the last value
            // then adjust to keep in heap order
            values[0] = values[--num_values];
            sift_down(); 
            return result;
        }

        // get the first value of the heap
        // don't change the heap
        T get_top_value() const throw(std::underflow_error)
        {
            if (num_values == 0) {
                throw std::underflow_error("heap is empty");
            }
            return values[0];
        }

    // helper functions
    private:
        int parent(int i) { return (i - 1) / 2; }
        int left_child(int i) { return i * 2 + 1; }
        int right_child(int i) { return i * 2 + 2; }
        void swap(int i, int j) 
        {
            T t = values[i];
            values[i] = values[j];
            values[j] = t;
        }

        void sift_up()
        {
            int i = num_values - 1;
            while (i > 0 && values[i] < values[parent(i)]) {
                // loop until up to the root (i = 0) 
                // or the value keep in heap order (values[i] >= values[parent(i)]
                swap(i, parent(i));
                i = parent(i);
            }
        }

        void sift_down()
        {
            int t; // to record smaller child index
            int i = 0;
            while (left_child(i) < num_values) {
                // loop until down to the leave
                if (right_child(i) < num_values
                    && values[right_child(i)] < values[left_child(i)]) {
                    // right child exist and is smaller than left child
                    // we record it
                    t = right_child(i);
                } else {
                    // opposite
                    t = left_child(i);
                }
                if (values[i] <= values[t]) {
                    // the values remain in heap order, we stop here
                    return;
                } else {
                    // keep on sifting
                    swap(i, t);
                    i = t;
                }
            }
        }
};
