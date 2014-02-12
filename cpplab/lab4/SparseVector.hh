class SparseVector {
    private:
        int size;
        struct node {
            int value;
            int index;
            node *next;

            // simply initializes the data-members
            node(int value, int index, node *next = 0): value(value), index(index), next(next) {}
        };

        node *start;
    
    public: 
        SparseVector(int size): size(size), start(0) {}
        ~SparseVector();
        // copy constructor
        SparseVector(const SparseVector &sv);

        int getSize() const;
        int getElem(int col) const;
        void setElem(int col, int value);
        void addElem(int col, int value);

        //operator overloads
        SparseVector & operator=(const SparseVector &sv);
        SparseVector & operator+=(const SparseVector &sv);
        SparseVector & operator-=(const SparseVector &sv);
        const SparseVector operator+(const SparseVector &sv) const;
        const SparseVector operator-(const SparseVector &sv) const;
        bool operator==(const SparseVector &sv) const;
        bool operator!=(const SparseVector &sv) const;

        // helper functions
    private:
        void copy(const SparseVector &sv);
        void cleanup();
        void setNoZeroElem(int col, int value);
        void removeElem(int col);
        void addList(const SparseVector &sv, int sign = 1 /* or -1 */);

        // monitor function
        bool checkListOrder() const;
};
