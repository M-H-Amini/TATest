#include <limits.h>
#include "aphw2.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include "gtest/gtest.h"
namespace
{

TEST(APHW2Test, AddressingTest1)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 4});
    data.push_back(std::vector<double>{6, 7});
    Matrix a(data);
    EXPECT_EQ(2, a[0][0]) << std::setw(100) <<" ********minus 5 points********";
}

TEST(APHW2Test, smartPointerTest)
{
    auto pa{std::make_unique<std::unique_ptr<double[]>[]>(5)};
    for (int i = 0; i < 5; ++i) {
        pa[i] = std::make_unique<double[]>(10);
        pa[i][i] = 1;
    }
    Matrix a {pa, 5, 10};
    a[1][2] = 5;
    a.show();
    EXPECT_EQ(5, a[1][2]) << std::setw(100) <<" ********minus 10 points********";
}

TEST(APHW2Test, SetterOperatorTest1)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 4});
    data.push_back(std::vector<double>{6, 7});
    Matrix a(data);
    a[1][1] = 8;
    EXPECT_EQ(8, a[1][1]) << std::setw(100) <<" ********minus 10 points********";
}

TEST(APHW2Test, SumTest)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 4});
    data.push_back(std::vector<double>{6, 7});
    Matrix a(data);
    Matrix b(data);
    Matrix c = a + b;
    EXPECT_EQ(12, c[1][0])<< std::setw(100) <<" ********minus 2 points********";
}

TEST(APHW2Test, MultiplicationTest)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 4});
    Matrix a(data);
    data.push_back(std::vector<double>{6, 7});
    Matrix b(data);
    Matrix c = a * b;
    EXPECT_EQ(28, c[0][0])<< std::setw(100) <<" ********minus 2 points********";
}

TEST(APHW2Test, TransposeTest)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 4});
    Matrix a(data);
    Matrix b{a.T()};
    EXPECT_EQ(4, b[1][0])<< std::setw(100) <<" ********minus 2 points********";
    EXPECT_EQ(2, b.getSize()[0])<< std::setw(100) <<" ********minus 3 points********";
}

TEST(APHW2Test, InversionTest)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 8, 15, 1024, 5});
    data.push_back(std::vector<double>{0, 1, 5, 4, 5});
    data.push_back(std::vector<double>{0, 0, 15, 2, 9.1});
    data.push_back(std::vector<double>{0, 0, 0, 2, 9.1});
    data.push_back(std::vector<double>{0, 0, 0, 0, -1});
    Matrix a(data);
    Matrix ainv{a.inv()};
    EXPECT_EQ(0.5, ainv[3][3])<< std::setw(100) <<" ********minus 2 points********";
}

TEST(APHW2Test, DeterminantTest)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 8, 15, 1024, 5});
    data.push_back(std::vector<double>{0, 1, 5, 4, 5});
    data.push_back(std::vector<double>{0, 0, 15, 2, 9.1});
    data.push_back(std::vector<double>{0, 0, 0, 2, 9.1});
    data.push_back(std::vector<double>{0, 0, 0, 0, -1});
    Matrix a(data);
    double det{a.det()};
    EXPECT_EQ(-60, det)<< std::setw(100) <<" ********minus 4 points********";
}

TEST(APHW2Test, MultiplicationTest2)
{
    std::vector<std::vector<double>> data;
    data.push_back(std::vector<double>{2, 4});
    data.push_back(std::vector<double>{6, 7});
    data.push_back(std::vector<double>{2, 5});
    Matrix a(data);
    Matrix b{a * a.T()};
    EXPECT_EQ(20, b[0][0])<< std::setw(100) <<" ********minus 5 points********";
}

TEST(APHW2Test, SubtractionTest1)
{
    Matrix a{2, 2};
    a[1][0] = 0;
    Matrix b{2, 2, false};
    b[1][1] = 1;
    Matrix c{a - b};
    EXPECT_EQ(0, c[1][1])<< std::setw(100) <<" ********minus 2 points********";
}

TEST(APHW2Test, OptionalTest1)
{
    Matrix a{2, 2};
    a[1][0] = 0;
    Matrix b{2, 2, false};
    b[0][1] = 1;
    Matrix c{inv(a - b)};
    EXPECT_TRUE(0.99<c[1][1] && c[1][1]<1.01)<< std::setw(100) <<" ********minus 15 points********";
}

TEST(APHW2Test, getDataTest)
{
    std::vector<std::vector<double>> data{getData("AP-Data.csv")};
    EXPECT_EQ(1, data[0][0])<< std::setw(100) <<" ********minus 3 points********";
}

TEST(APHW2Test, getDataTest1)
{
    std::vector<std::vector<double>> data{getData("APHW2TestData.csv", false)};
    EXPECT_TRUE(-3.89 > data[0][1] && -3.9 < data[0][1])<< std::setw(100) <<" ********minus 5 points********";
}

TEST(APHW2Test, estimationTest)
{
    std::vector<std::vector<double>> my_data{getData("AP-Data.csv")};
    Matrix X{my_data};
    Matrix y{X.col(7)};
    X.delCol(7);
    Matrix w{(X.T()*X).inv()*X.T()*y};
    EXPECT_TRUE((w[1][0] > 8) && (w[1][0] < 9))<< std::setw(100) <<" ********minus 5 points********";
    EXPECT_TRUE((w[2][0] > 4.5) && (w[2][0] < 5.5))<< std::setw(100) <<" ********minus 5 points********";
}

TEST(APHW2Test, estimationTest1)
{
    std::vector<std::vector<double>> data{getData("APHW2TestData.csv", false)};
    Matrix w{findWeights("APHW2TestData.csv")};
    Matrix o {predict("APHW2TestData.csv", w, true)};
    EXPECT_TRUE(-3.8 > o[0][0] && -3.9 < o[0][0])<< std::setw(100) <<" ********minus 10 points********";
}

TEST(APHW2Test, dynamicProgTest)
{
    Matrix a1{10, 5}, a2{5, 20}, a3{20, 2};
    std::vector<Matrix> vecs{a1, a2, a3};
    size_t ans{findMinNoOfMultiplications(vecs)};
    EXPECT_EQ(300, ans)<< std::setw(100) <<" ********minus 10 points********";
}

} 
