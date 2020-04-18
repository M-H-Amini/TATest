#ifndef _MAT_H
#define _MAT_H

#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <math.h>
#include <string>
#include <fstream>
#include <optional>

class Matrix
{
private:
    std::array<size_t, 2> size;
    std::vector<std::vector<double>> data;

public:
    Matrix (std::unique_ptr<std::unique_ptr<double[]>[]>& data, size_t m, size_t n);
    Matrix(const Matrix& mat);
    Matrix(std::vector<std::vector<double>> Data );
    Matrix (size_t m, size_t n, bool ones=true);
    Matrix (std::optional<Matrix> m);     //to make it possible to work with functions with std::opatianal output.
    
    std::array<size_t, 2> getSize();
    void show();
    Matrix delCol(size_t j);
    Matrix myDelCol(size_t j);            //returns a matrix eith removed column, without changin the matrix!
    Matrix col(size_t j);
    Matrix delRow(size_t j);
    Matrix T();
    double det();
    Matrix inv();

    Matrix operator * (Matrix b);
    Matrix operator + (Matrix a);
    Matrix operator - (Matrix a);
    std::vector<double>& operator [] (size_t i);

    void save(const char* fileName);
    void load(const char* fileName);
    std::vector<std::vector<double>> getMatData();    //gives access to marix data.
};

#endif