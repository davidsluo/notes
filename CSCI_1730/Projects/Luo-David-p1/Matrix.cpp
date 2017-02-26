#include "Matrix.h"

/**
 * Create a matrix with `rows` rows and `cols` columns, with all
 * elements initialized to 0.
 * @param rows The number of rows
 * @param cols The number of columns
 */
Matrix::Matrix(uint rows, uint cols) {

  this->rows = rows;
  this->cols = cols;

  array = new double * [rows];

  for(uint i = 0; i < rows; i++) {
  array[i] = new double [cols];
  }
}

/*
 * Create a matrix from a list.
 * @param i_list The list to turn into a matrix
 */
Matrix::Matrix(const i_list & list) {

    this->rows = list.size();
    this->cols = (*list.begin()).size();

    array = new double * [rows];

    for (uint i = 0; i < rows; i++) {
        array[i] = new double [cols];
        for (uint j = 0; j < cols; j++) {
            array[i][j] = (*(list.begin() + i) -> begin()) + j;
        }
    }
}

/**
 * Create a Matrix from another Matrix.
 * @param m The matrix to copy
 */
Matrix::Matrix(const Matrix& m){
    this->rows = m.numRows();
    this->cols = m.numCols();

    array = new double * [this->rows];

    for (uint i = 0; i < this->rows; i++) {
            array[i] = new double [this->cols];
    }

  for (uint i = 0; i < m.numRows(); i++) {
        for (uint j = 0; j < m.numCols(); j++) {
            at(i,j) = m.at(i,j);
        }
    }

}

/**
 * Destructor.
 */
Matrix::~Matrix() {
  for (uint i = 0; i < rows; i++) {
  delete [] array[i];
  }

  delete [] array;
}

/**
 * Add a scalar to this matrix.
 * @param s Scalar to add to this matrix
 * @return The resulting matrix
 */
Matrix Matrix::add(double s) const {
   Matrix result(*this);

   for (uint i = 0; i < result.numRows(); i++) {
       for(uint j = 0; j < result.numCols(); j++) {
           result.at(i,j) += s;
       }
    }

   return result;
}

/**
 * Add another matrix to this matrix.
 * @param m The matrix to add to this matrix
 * @return The resulting matrix
 */
Matrix Matrix::add(const Matrix& m) const {
   Matrix result(*this);

   for (uint i = 0; i < result.numRows(); i++) {
       for (uint j = 0; j < result.numCols(); j++) {
           result.at(i,j) += m.at(i,j);
       }
   }

   return result;
}

/**
 * Subtract a scalar from this matrix.
 * @param s The scalar to subtract from this matrix
 * @return The resulting matrix
 */
Matrix Matrix::subtract(double s) const {
    return add(-s);
}

/**
 * Subtract another matrix from this matrix.
 * @param m The matrix to subtract from this matrix
 * @return The resulting matrix
 */
Matrix Matrix::subtract(const Matrix& m) const {
    return add(-m);
}

/**
 * Multiply this matrix by a scalar.
 * @param s The scalar to multiply this matrix
 * @return The resulting matrix
 */
Matrix Matrix::multiply(double s) const {
    Matrix result(*this);

    for (uint i = 0; i < result.numRows(); i++) {
        for (uint j = 0; j < result.numCols(); j++) {
            result.at(i,j) *= s;
        }
    }

    return result;
}


/**
 * Multiply this matrix by another matrix. This matrix is the
 * first matrix in the multiplication operation.
 * @param m The matrix to multiply this matrix by
 * @return The resulting matrix
 */
Matrix Matrix::multiply(const Matrix& m) const {
  Matrix result(rows, m.numCols());

  // for each row in the first matrix
    for (uint i = 0; i < rows; i++) {
      // for each column in the second matrix
        for (uint j = 0; j < m.numCols(); j++) {
          // for each element in the resulting pairing
          for (uint k = 0; k < cols; k++) {
            result.at(i,j) += (this->at(i,k) * m.at(k,j));
          }
      }
  }

    return result;
}

/**
 * Divide this matrix by a scalar.
 * @param s The scalar to divide this scalar by.
 * @return The resulting matrix
 */
Matrix Matrix::divide(double s) const {
    return multiply(1.0/s);
}

/**
 * Divide a scalar by this matrix.
 * @param s The scalar.
 * @return The resulting matrix.
 */
Matrix Matrix::s_divide(double s) const {
    Matrix result(rows, cols);

    for (uint i = 0; i < result.numRows(); i++) {
        for (uint j = 0; j < result.numCols(); j++) {
            result.at(i,j) = s / at(i,j);
        }
    }

    return result;

}

/**
 * Get the transpose of this matrix.
 * @return The transpose of this matrix.
 * @return The transposed matrix
 */
Matrix Matrix::t() const {
    Matrix result(numCols(), numRows());

    for (uint i = 0; i < numRows(); i++) {
        for (uint j = 0; j < numCols(); j++) {
            result.at(j,i) = at(i,j);
        }
    }

    return result;
}

//////////// OPERATOR OVERLOADING ////////////

/**
 * Makes the unary minus operator act like it should
 * @return the negative of this matrix.
 */
Matrix Matrix::operator-() const {
  Matrix result(*this);
  for (uint i = 0; i < result.numRows(); i++) {
  for (uint j = 0; j < result.numCols(); j++) {
            result.at(i,j) *= -1;
        }
    }

    return result;
}

/**
 * Makes the function call operator act like the `at()` function.`
 * @param row The row of the matrix
 * @param col The column of the matrix
 * @return The element at (row,col)
 */
double & Matrix::operator()(uint row, uint col) const {
    return this->array[row][col];
}

/**
 * Makes the copy operator copy matricies of the same dimension.
 * @param m The matrix to be copied
 * @return The copied matrix.
 */
Matrix& Matrix::operator=(const Matrix& m) {
    for (uint i = 0; i < m.numRows(); i++) {
        for (uint j = 0; j < m.numCols(); j++) {
            at(i,j) = m.at(i,j);
        }
    }

    return *this;
}

/**
 * Makes the + operator on two matricies act like matrix addition.
 * @param The matrix to be added.
 * @return The sum of the two matricies.
 */
Matrix Matrix::operator+(const Matrix& m) const {
    return add(m);
}

/**
 * Makes the - operator on two matricies act like matrix subtraction.
 * @param m The matrix to be subtracted.
 * @return The difference of the two matricies.
 */
Matrix Matrix::operator-(const Matrix& m) const {
    return subtract(m);
}

/**
 * Makes the * operator on two matricies act like matrix multiplication.
 * @param m The matrix to be multiplied.
 * @return The product of the two matriciies.
 */
Matrix Matrix::operator*(const Matrix& m) const {
    return multiply(m);
}

/**
 * Get the number of rows.
 * @return The number of rows
 */
const uint Matrix::numRows() const { return this->rows; }

/**
 * Get the number of columns
 * @return The number of columns
 */
const uint Matrix::numCols() const { return this->cols; }

/**
 * Get/set element at `row`,`col`
 * @param row The row
 * @param col The column
 */
double & Matrix::at(uint row, uint col) { return this->array[row][col]; }

/**
 * Get/set element at `row`,`col` when using a const object
 * @param row The row
 * @param col the column
 */
const double & Matrix::at(uint row, uint col) const {
  return this->array[row][col];
}

/**
 * Makes the + operator on a matrix act like (matrix + scalar)
 * @param m The matrix
 * @param x The scalar
 * @return The scalar added to every element in the matrix.
 */
Matrix operator+(const Matrix& m, const double x) { return m.add(x); }

/**
 * Makes the + operator on a matrix act like (scalar + matrix)
 * @param x The scalar
 * @param m The matrix
 * @return The scalar added to every element in the matrix.
 */
Matrix operator+(const double x, const Matrix& m) { return m.add(x); }

/**
 * Makes the - operator on a matrix act like (matrix - scalar)
 * @param m The matrix
 * @param x The scalar
 * @return The scalar subtracted from every element in the matrix.
 */
Matrix operator-(const Matrix& m, const double x) { return m.subtract(x); }

/**
 * Makes the - operator on a matrix act like (scalar - matrix)
 * @param x The scalar
 * @param m The matrix
 * @return Every element in the matrix subtracted from the scalar.
 */
Matrix operator-(const double x, const Matrix& m) { return (-m).add(x); }

/**
 * Makes the * operator on a matrix act like (matrix * scalar)
 * @param m The matrix
 * @param x The scalar
 * @return Every element in the matrix multiplied by the scalar.
 */
Matrix operator*(const Matrix& m, const double x) { return m.multiply(x); }

/**
 * Makes the * operator on a matrix act like (scalar * matrix)
 * @param x The scalar
 * @param m The matrix
 * @return Every element in the matrix multiplied by the scalar.
 */
Matrix operator*(const double x, const Matrix& m) { return m.multiply(x); }

/**
 * Makes the / operator on a matrix act like (matrix / scalar)
 * @param m The matrix
 * @param x The scalar
 * @return Every element in the matrix divided by the scalar.
 */
Matrix operator/(const Matrix& m, const double x) { return m.divide(x); }


/**
 * Makes the / operator on a matrix act like (scalar / matrix)
 * @param x The scalar
 * @param m The matrix
 * @return The scalar divided by each element in the matrix.
 */
Matrix operator/(const double x, const Matrix& m) { return m.s_divide(x); }

/**
 * Allows for stream insertion of the matrix directly.
 * @param os The stream
 * @param obj The matrix
 * @return The stream with the matrix inserted.
 */
ostream& operator<<(ostream& os, const Matrix& obj) {
    os << "{";
    for (uint i = 0; i < obj.numRows(); i++) {
        os << "{";
        for (uint j = 0; j < obj.numCols(); j++) {
            os << obj.at(i,j);
            if (j != obj.numCols() - 1)
                os << ",";
        }
        os << "}";
        if (i != obj.numRows() - 1)
            os << "," << endl;
    }
    os << "}";

    return os;
}


