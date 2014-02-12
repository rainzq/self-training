#include <cstddef>
using namespace std;

class Matrix {
    private:
        int *m;
        int row;
        int col;

    public:
        Matrix(): m(0), row(0), col(0) {}
        Matrix(int r, int c);

        ~Matrix();

        int get_elem(int r, int c) const;
        void set_elem(int r, int c, int v);

        // get the number of rows or columns of Matrix
        int get_rows() const;
        int get_cols() const;
        
        // copy constructor
        Matrix(const Matrix &another);

        // operator
        Matrix & operator=(const Matrix &rhs);
        Matrix & operator+=(const Matrix &rhs);
        Matrix & operator-=(const Matrix &rhs);
        Matrix & operator*=(const Matrix &rhs);
        const Matrix operator+(const Matrix &rhs) const;
        const Matrix operator-(const Matrix &rhs) const;
        const Matrix operator*(const Matrix &rhs) const;
        bool operator==(const Matrix &rhs) const;
        bool operator!=(const Matrix &rhs) const;

        // help functions
        // compatibal to simplematrix test we change to public
        // should have been private
    public:
        void add(const Matrix &rhs);
        void sub(const Matrix &rhs);

        // Will change the dimension of Matrix
        void mul(const Matrix &rhs);

        bool equal(const Matrix &rhs) const;
        void copy(const Matrix &other);
        void cleanup();
};
