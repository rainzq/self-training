#include "matrix.hh"
#include <cassert>
#include <cstddef>

using namespace std;

Matrix::Matrix(int r, int c):
    row(r), col(c)
{
    m = new int[col * row];
    int index;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++) {
            index = i * col + j;
            m[index] = 0;
        }
}

Matrix::~Matrix()
{
    delete[] m;
}

int Matrix::get_elem(int r, int c) const 
{
    assert(r >= 0 && r <= row);
    assert(c >= 0 && c <= col);

    int index = r * col + c;
    return m[index];
}

void Matrix::set_elem(int r, int c, int v)
{
    assert(r >= 0 && r <= row);
    assert(c >= 0 && c <= col);

    int index = r * col + c;
    m[index] = v;
}

int Matrix::get_rows() const
{
    return row;
}

int Matrix::get_cols() const
{
    return col;
}

// help functions
void Matrix::copy(const Matrix &another)
{
    int index;
    col = another.col;
    row = another.row;
    m = new int[col * row];
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++) {
            index = i * col + j;
            m[index] = another.m[index];
        }

}

void Matrix::cleanup()
{
    delete[] m;
}

void Matrix::add(const Matrix &rhs)
{
    assert(row == rhs.row);
    assert(col == rhs.col);

    int index;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++) {
            index = i * col + j;
            m[index] += rhs.m[index];
        }
}

void Matrix::sub(const Matrix &rhs)
{
    assert(row == rhs.row);
    assert(col == rhs.col);

    int index;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++) {
            index = i * col + j;
            m[index] -= rhs.m[index];
        }
}

bool Matrix::equal(const Matrix &rhs) const
{
    if (row != rhs.row || col != rhs.col)
        return false;
    
    int index;
    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++) {
            index = i * col + j;
            if (m[index] != rhs.m[index])
                return false;
        }
    return true;
}

// [m by n] * [n by k] = [m by k]
// this: [m by n]
// rhs : [n by k]
// result: [m by k]
// algrothm:
//
// for i in m:
//   for j in k:
//     res = 0
//     for t in n:
//       res = res + this[i,t] * rhs[t,j]
//     result[i,j] = res
//
void Matrix::mul(const Matrix &rhs)
{
    assert(col == rhs.row);
    
    int index;
    int result = 0;
    Matrix result_matrix(row, rhs.col);
    for (int i = 0; i < row; i++) 
        for (int j = 0; j < rhs.col; j++) {
            result = 0;
            for (int k = 0; k < col; k++) {
                result += (m[i * col + k] * rhs.m[k * rhs.col + j]);
            }
            index = i * rhs.col + j;
            result_matrix.m[index] = result;
        }
    *this = result_matrix;
}

// copy constructor
Matrix::Matrix(const Matrix &another)
{
    copy(another);
}

// operators
Matrix & Matrix::operator=(const Matrix &rhs)
{
    cleanup();
    copy(rhs);
    return *this;
}

Matrix & Matrix::operator+=(const Matrix &rhs)
{
    add(rhs);
    return *this;
}

Matrix & Matrix::operator-=(const Matrix &rhs)
{
    sub(rhs);
    return *this;
}

Matrix & Matrix::operator*=(const Matrix &rhs)
{
    mul(rhs);
    return *this;
}

const Matrix Matrix::operator+(const Matrix &rhs) const
{
    Matrix result(*this);
    result += rhs;
    return result;
}

const Matrix Matrix::operator-(const Matrix &rhs) const
{
    Matrix result(*this);
    result -= rhs;
    return result;
}

const Matrix Matrix::operator*(const Matrix &rhs) const
{
    Matrix result(*this);
    result *= rhs;
    return result;
}

bool Matrix::operator==(const Matrix &rhs) const
{
    return this->equal(rhs);
}

bool Matrix::operator!=(const Matrix &rhs) const
{
    return !(*this == rhs);
}
