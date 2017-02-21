#include "Matrix.h"

Matrix::Matrix(uint rows, uint cols) {

	this->rows = rows;
	this->cols = cols;

	array = new double * [rows];
	
	for(uint i = 0; i < rows; i++) {
		array[i] = new double [cols];
	}	
}

Matrix::Matrix(const Matrix & m) {
	Matrix::Matrix(m.numRows(), m.numCols);
}

Matrix::~Matrix() {
	for (uint i = 0; i < rows; i++) {
		delete [] array[i];
	}

	delete [] array;
}

const uint Matrix::numRows() const{
	return rows;
}

const uint Matrix::numCols() const{
	return cols;
}

double & at(uint row, uint col) {
	return array[row][col];
}

const double & at(uint row, uint col) const{
	return array[row][col];
}
