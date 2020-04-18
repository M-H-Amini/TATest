#include "aphw2.h"

std::optional<double> det(Matrix& m)
{
    std::array<size_t, 2> size = m.getSize();
    if( size[1] == size[0] )
        return m.det();
    std::cout<<"det: non square! \n";
    return std::nullopt;
}

std::optional<Matrix> inv(Matrix&& m)
{
    std::array<size_t, 2> size = m.getSize();
    if( size[1] == size[0] )
        return m.inv();
    std::cout<<"inv: unable! \n";
    return std::nullopt;
}

std::optional<Matrix> transpose(Matrix& m)
{
    return m.T();
}

size_t findMin(std::vector<size_t> v)
{
    size_t min{v[0]};
    for (size_t i = 0; i < v.size(); i++)
        if( v[i] < min )
            min = v[i];
    return min;
}

size_t opt(size_t i, size_t k, std::vector<size_t> size, std::vector<std::vector<size_t>>& save)
{
    if( i == k )
        return 0;
    if( save[i][k] != 0 )                       //checks if calculated before. (dynamic programming)
        return save[i][k];
    std::vector<size_t> temp;
    for (size_t z = i; z < k; z++)              //calculates possibilaties for multipliction Ith o Kth matrix.
        temp.push_back( opt(i, z, size, save) + opt(z+1, k, size, save) + size[i-1]*size[z]*size[k] );

    size_t min{findMin(temp)};                  //finds the min value of all the possibilaies at this turn.
    save[i][k] = min;                           //saves the calculated value. (dynamic programming)
    return min;
}

size_t findMinNoOfMultiplications(std::vector<Matrix>& v)
{
    std::vector<size_t> size;                      //creats a vector including size of matrixes.
    size_t matNo = v.size();
    for (size_t i = 0; i < matNo; i++)
    {
        std::array<size_t,2> Size{v[i].getSize()};
        size.push_back(Size[0]);
    }
    std::array<size_t,2> Size{v[matNo-1].getSize()};
    size.push_back(Size[1]);

    std::vector<std::vector<size_t>> save;         //creating a 2D vector to save operations result in opt function. (dynamic programming)
    for (size_t i = 0; i < matNo+1; i++)
        save.push_back(std::vector<size_t>(matNo+1, 0));

    size_t ans{opt(1, matNo, size, save)};
    return ans;
}

std::vector<std::vector<double>> getData(const char* filename, bool add_bias)
{
    std::vector<std::vector<double>> Data;
    std::ifstream dataFile1(filename);
    char* line{new char[200]};
    int j{};
    size_t columnCounter{};
    double param;
    char va;

    dataFile1.getline(line, 200);           //to count how many culomns are there in the file.
    while( j < 200 )
    {
        if( line[j] == ',' ) 
            columnCounter++;
        j++;
    }
    delete[] line;
    dataFile1.close();

    std::ifstream dataFile(filename);       //open the file again to refuse missing the first line.
    while( !dataFile.eof() )
    {
        std::vector<double> temp;
        if( add_bias ) 
            temp.push_back(1);
        for (size_t i{}; i < columnCounter+1 ; i++)
        {
            if( i == columnCounter ) 
                dataFile >> param;
            else
                dataFile >> param >> va;
            temp.push_back(param);
        }
        Data.push_back(temp);
    }
    dataFile.close();
    Data.pop_back();
    return Data;
}

Matrix findWeights(const char* fileName)
{
    std::vector<std::vector<double>> data{getData(fileName)};
    size_t rowN = data.size();
    size_t columnN = data[0].size();
    std::vector<std::vector<double>> yData;
    for (size_t i = 0; i < rowN; i++)                  //creating y matrix.
    {
        std::vector<double> temp;
        temp.push_back(data[i][columnN-1]);
        yData.push_back(temp);
    }
    Matrix y(yData);

    std::vector<std::vector<double>> xData;
    for (size_t i = 0; i < rowN; i++)                  //creating x matrix.
    {
        std::vector<double> temp;
        for (size_t j = 0; j < columnN-1; j++)
            temp.push_back(data[i][j]);
        xData.push_back(temp);
    }
    Matrix x(xData);

    Matrix t{x.T() * x};
    Matrix w{t.inv() * x.T() * y};

    return w;
}

Matrix predict(const char* name, Matrix& w, bool disp)
{
    std::vector<std::vector<double>> Data, mData;
    mData = w.getMatData();                       //cuz matrix data is pritave.
    Data = getData(name);
    size_t columnN = Data[0].size();
    size_t rowN = Data.size();
    double grade{};
    std::vector<std::vector<double>> ans;

    for (size_t i = 0; i < rowN; i++)
    {
        std::vector<double> temp;
        for (size_t j = 0; j < columnN-1; j++)
            grade += Data[i][j] * mData[j][0];
        temp.push_back(grade);
        ans.push_back(temp);
        grade = 0;
    }
    Matrix Ans(ans);

    if( disp )
    {
        std::cout<<std::setiosflags(std::ios::left)<<std::setw(12)<<"Estimated";
        std::cout<<std::setiosflags(std::ios::left)<<std::setw(12)<<"Real Grade"<<std::endl;
        for(size_t i{}; i < rowN; i++)
        {
            std::cout<<std::setiosflags(std::ios::left)<<std::setw(12)<<std::setprecision(3)<<ans[i][0];
            std::cout<<std::setiosflags(std::ios::left)<<std::setw(12)<<std::setprecision(3)<<Data[i][columnN-1];
            std::cout<<std::endl;
        }
    }
    return Ans;
}

