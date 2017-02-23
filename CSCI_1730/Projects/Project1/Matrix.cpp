#include "Matrix.h"

/**
 * Create a matrix with `rows` rows and `cols` columns.
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
 * Copy a Matrix.
 * @param m The matrix to copy
 */
Matrix::Matrix(const Matrix & m){
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
 * Allows unary minus
 */
Matrix Matrix::operator-() const {
	Matrix result(*this);
	for (uint i = 0; i < result.numRows(); i++) {
		for (uint j = 0; j < result.numCols(); i++) {
            result.at(i,j) *= -1;
        }
    }

    return result;    
}

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
            os << ",";
    }
    os << "}";

    return os;
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
       for(uint j = 0; i < result.numCols(); j++) {
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
Matrix Matrix::add(const Matrix & m) const {
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
Matrix Matrix::subtract(const Matrix & m) const {
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
Matrix Matrix::multiply(const Matrix & m) const {
    Matrix result(rows, m.numCols());

    for (uint i = 0; i < rows; i++) { // for each row in the first matrix 
        for (uint j = 0; j < m.numCols(); j++) { // for each column in the second matrix
            for (uint k = 0; k < cols; k++) { // for each element in the resulting pairing
                result.at(i,j) += this->at(i,k) * m.at(j,k);
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

const uint Matrix::numRows() const{
    return this->rows;
}

const uint Matrix::numCols() const{
    return this->cols;
}

double & Matrix::at(uint row, uint col) {
    return this->array[row][col];
}

const double & Matrix::at(uint row, uint col) const{
    return this->array[row][col];
}
