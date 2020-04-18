#ifndef _APHW2_H
#define _APHW2_H

#include <iostream>
#include <array>
#include <vector>
#include <iomanip>
#include <math.h>
#include <string>
#include <fstream>
#include <optional>
#include "Matrix.h"

std::optional<double> det(Matrix& m);           
std::optional<Matrix> inv(Matrix&& m);
std::optional<Matrix> transpose(Matrix& m);

size_t findMin(std::vector<size_t> v);           //finds minimum value of a vectors members.
size_t opt(size_t i, size_t k, std::vector<size_t> size, std::vector<std::vector<size_t>>& save); //explained in report.
size_t findMinNoOfMultiplications(std::vector<Matrix>& v);

std::vector<std::vector<double>> getData(const char* filename, bool add_bias=true);
Matrix findWeights(const char* fileName);
Matrix predict(const char* name, Matrix& w, bool disp=false);

#endif
