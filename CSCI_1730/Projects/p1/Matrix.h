#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <iostream>

using namespace std;

typedef unsigned int uint;
typedef std::initializer_list<std::initializer_list<double>> i_list;

class Matrix {
    public:
        double ** array = nullptr;
        uint rows;
        uint cols;

        Matrix(uint rows, uint cols);
        Matrix(const i_list & list);
        Matrix(const Matrix& m);
        ~Matrix();
        Matrix add(double s) const;
        Matrix add(const Matrix& m) const;
        Matrix subtract(double s) const;
        Matrix subtract(const Matrix& m) const;
        Matrix multiply(double s) const;
        Matrix multiply(const Matrix& m) const;
        Matrix divide(double s) const;
        Matrix s_divide(double s) const;
        Matrix t() const;
        Matrix operator-() const;
        double& operator()(uint row, uint col) const;
        Matrix& operator=(const Matrix& m);
        Matrix operator+(const Matrix& m) const;
        Matrix operator-(const Matrix& m) const;
        Matrix operator*(const Matrix& m) const;
        const uint numRows() const;
        const uint numCols() const;
        double& at(uint row, uint col);
        const double& at (uint row, uint col) const;
}; // Matrix

ostream& operator<<(ostream& os, const Matrix& obj);
Matrix operator+(const Matrix& m, const double x);
Matrix operator+(const double x, const Matrix& m);
Matrix operator-(const Matrix& m, const double x);
Matrix operator-(const double x, const Matrix& m);
Matrix operator*(const Matrix& m, const double x);
Matrix operator*(const double x, const Matrix& m);
Matrix operator/(const Matrix& m, const double x);
Matrix operator/(const double x, const Matrix& m);


#endif
