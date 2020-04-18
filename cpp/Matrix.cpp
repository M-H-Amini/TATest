#include "Matrix.h"

Matrix::Matrix (std::unique_ptr<std::unique_ptr<double[]>[]>& data, size_t m, size_t n)
{
    size[0] = m;
    size[1] = n;
    for (size_t i = 0; i < m; i++)                //copying values of data in matrix.data
    {
        std::vector<double> temp;
        for (size_t j = 0; j < n; j++)
            temp.push_back(data[i][j]);
        this->data.push_back(temp);
    }  
}

Matrix::Matrix(const Matrix& mat)                  //copy constructor
{
    data = mat.data;
    size = mat.size;
}

Matrix::Matrix(std::vector<std::vector<double>> Data ) 
{
    size[0] = Data.size();
    size[1] = Data[0].size();
    data = Data;
}

Matrix::Matrix (size_t m, size_t n, bool ones)
{
    int a = 0;
    if( ones )
        a = 1;
    for(size_t i{}; i < m; i++)
    {
        std::vector<double> column;
        for (size_t j{}; j < n; j++)
            column.push_back(a);
        data.push_back(column);
    } 
    size[0] = m;
    size[1] = n;
}

Matrix::Matrix (std::optional<Matrix> m)                   //to work with funcions with output "optioanl".
{
    if( m.has_value() )
    {
        data = m.value().data;
        size[0] = m.value().size[0];
        size[1] = m.value().size[1];
    }
    else 
        std::cout<<"constructor: unable to creat! \n";
}

std::array<size_t, 2> Matrix::getSize()
{
    return size;
}

void Matrix::show()
{
    for (size_t i = 0; i < size[0]; i++)
    {
        for (size_t j = 0; j < size[1]; j++)
            std::cout<<std::setiosflags(std::ios::left)<<std::setw(6)<<std::setprecision(3)<<data[i][j];
        std::cout<<std::endl;
    }
}

Matrix Matrix::delCol(size_t j)
{
    for (size_t i = 0; i < size[0]; i++)
        data[i].erase(data[i].begin()+j);            //sets the iterator where we want to remove a thing.
    size[1]--;
    return Matrix(data);
}

Matrix Matrix::myDelCol(size_t j)
{
    std::vector<std::vector<double>> Data;
    Data = data;
    for (size_t i = 0; i < size[0]; i++)
        Data[i].erase(Data[i].begin()+j-1);               //sets the iterator to the (j-1)th element of vector.
    //data = Data;                     
    return Matrix(Data);
}

Matrix Matrix::col(size_t j)
{
    std::vector<std::vector<double>> Data;
    for (size_t i = 0; i < size[0]; i++)
    {
        std::vector<double> temp;
        temp.push_back(data[i][j]);
        Data.push_back(temp);
    }
    return Matrix(Data);
}

Matrix Matrix::delRow(size_t j)        //to help calculating determinate.
{
    std::vector<std::vector<double>> Data;
    Data = data;
    Data.erase(Data.begin()+j-1);
    //data = Data;
    return Matrix(Data);
}

Matrix Matrix::T()
{
    std::vector<std::vector<double>> Data;
    for (size_t i = 0; i < size[1]; i++)
    {
        std::vector<double> temp;
        for (size_t j = 0; j < size[0]; j++)
            temp.push_back(data[j][i]);
        Data.push_back(temp);
    }
    return Matrix(Data);
}

Matrix Matrix::inv()
{
    std::vector<std::vector<double>> Data;
    double Det = this->det();
    for (size_t i{}; i < size[0]; i++)
    {
    std::vector<double> temp;
    Matrix m = this->delRow(i+1);
        for (size_t j{}; j < size[0]; j++)
        {
            Matrix m2 = m.myDelCol(j+1);
            temp.push_back( (m2.det() * pow(-1,i+j)) / Det );
        }
        Data.push_back(temp);
    }
    Matrix m3 = Matrix(Data);
    return m3.T();
}

Matrix Matrix::operator * (Matrix b)
{
    if( this->size[1] != b.size[0] )
    {
        std::cout<<"*: cant operate! "<<std::endl;
        return Matrix(this->size[0], this->size[1], false);
    }

    std::vector<std::vector<double>> Data;
    for (size_t z{}; z < this->size[0]; z++)
    {
        std::vector<double> Temp;
        for (size_t j{}; j < b.size[1]; j++)
        {
            double temp{};
            for (size_t i{}; i < this->size[1]; i++)
                temp += this->data[z][i] * b.data[i][j];
            Temp.push_back(temp);
        }
        Data.push_back(Temp);
    }
    return Matrix(Data);
}

Matrix Matrix::operator + (Matrix a)
{
    if( size[0] != a.size[0] || size[1] != a.size[1] ) 
    {
        std::cout<<"+: cant oprate! \n";
        return Matrix(this->size[0], this->size[1], false);
    }
    std::vector<std::vector<double>> Data;
    for (size_t i = 0; i < size[0]; i++)
    {
        std::vector<double> temp;
        for (size_t j = 0; j < size[1]; j++)
            temp.push_back( data[i][j] + a.data[i][j]);
        Data.push_back(temp);
    }
    return Matrix(Data);
}

Matrix Matrix::operator - (Matrix a)
{
    if( size[0] != a.size[0] || size[1] != a.size[1] ) 
    {
        std::cout<<"-: cant oprate! \n";
        return Matrix(this->size[0], this->size[1], false);
    }
    std::vector<std::vector<double>> Data;
    for (size_t i = 0; i < size[0]; i++)
    {
        std::vector<double> temp;
        for (size_t j = 0; j < size[1]; j++)
            temp.push_back( data[i][j] - a.data[i][j]);
        Data.push_back(temp);
    }
    return Matrix(Data);
}

std::vector<double>& Matrix::operator [] (size_t i)
{
    return data[i];
}

void Matrix::save(const char* fileName)
{
    std::ofstream matrixFile(fileName);
    for (size_t i{}; i < size[0]; i++)
    {
        for (size_t j{}; j < size[1]; j++)
            matrixFile << data[i][j] <<" ,";
            //matrixFile << std::endl;
    }
    matrixFile.close();
}

void Matrix::load(const char* fileName)
{
    std::ifstream matrixFile(fileName);
    std::vector<std::vector<double>> Data;
    char va;
    double param;
    for (size_t i{}; i < size[0]; i++)
    {
        std::vector<double> parameters;
        for (size_t j{}; j < size[1]; j++)
        {
            matrixFile >> param >> va;
            parameters.push_back(param);
        }
        Data.push_back(parameters);
    }
    matrixFile.close();
    data = Data;
}

std::vector<std::vector<double>> Matrix::getMatData()     //enables accessing matrix data (cuz private!)
{
    return data;
}




