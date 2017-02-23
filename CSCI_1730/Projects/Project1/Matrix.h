#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>

using namespace std;

typedef unsigned int uint;
typedef std::initializer_list<std::initializer_list<double>> i_list;

class Matrix {
    public:
        double ** array;
        uint rows;
        uint cols;

    	/**
         * Create a matrix with `rows` rows and `cols` columns.
         * @param rows The number of rows
         * @param cols The number of columns
         */
        Matrix(uint rows, uint cols);

        /**
         * Create a matrix from a list.
         * @param i_list The list to turn into a matrix
         */
        Matrix(const i_list & list);

        /**
         * Copy a Matrix.
         * @param m The matrix to copy
         */
        Matrix(const Matrix & m);

        /**
         * Destructor.
         */
        ~Matrix();
        
        /**
         * Allows unary minus
         */
        Matrix operator-() const;

        /**
         * Add a scalar to this matrix.
         * @param s Scalar to add to this matrix
         * @return The resulting matrix
         */
        Matrix add(double s) const;

        /**
         * Add another matrix to this matrix.
         * @param m The matrix to add to this matrix
         * @return The resulting matrix
         */
        Matrix add(const Matrix & m) const;

        /**
         * Subtract a scalar from this matrix.
         * @param s The scalar to subtract from this matrix
         * @return The resulting matrix
         */
        Matrix subtract(double s) const;

        /**
         * Subtract another matrix from this matrix.
         * @param m The matrix to subtract from this matrix
         * @return The resulting matrix
         */
        Matrix subtract(const Matrix & m) const;

        /**
         * Multiply this matrix by a scalar.
         * @param s The scalar to multiply this matrix
         * @return The resulting matrix
         */
        Matrix multiply(double s) const;

        /**
         * Multiply this matrix by another matrix. This matrix is the
         * first matrix in the multiplication operation.
         * @param m The matrix to multiply this matrix by
         * @return The resulting matrix
         */
        Matrix multiply(const Matrix & m) const;

        /**
         * Divide this matrix by a scalar.
         * @param s The scalar to divide this scalar by.
         * @return The resulting matrix
         */
        Matrix divide(double s) const;

        /**
         * Get the transpose of this matrix.
         * @return The transpose of this matrix.
         * @return The transposed matrix
         */
        Matrix t() const;

        /**
         * Get the number of rows.
         * @return The number of rows
         */
        const uint numRows() const;

        /**
         * Get the number of columns
         * @return The number of columns
         */
        const uint numCols() const;

        /**
         * Get/set element at `row`,`col`
         * @param row The row
         * @param col The column
         */
        double & at(uint row, uint col);

        /**
         * Get/set element at `row`,`col` when using a const object
         * @param row The row
         * @param col the column
         */
        const double & at (uint row, uint col) const;

}; // Matrix

#endif
