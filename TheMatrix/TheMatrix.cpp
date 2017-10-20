#include <iostream>
#include "Matrix.h"
#include <random>
#include <ctime>
std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

template<typename T,bool Major = true,typename B>
Eagle::Matrix<T,Major> RandomFill(unsigned int rows,unsigned int cols,B& generator)
{
    Eagle::Matrix<T, Major> rhs{ rows,cols };
    for(auto& v : rhs)
    {
        v = generator(rng);
    }
    return rhs;
}
template<typename T,bool Major>
std::ostream& operator<<( std::ostream& os, Eagle::Matrix<T, Major>& rhs)
{
    auto acc = 0;
    os << "| ";
    for (const auto& v : rhs)
    {
        if ((acc % rhs.cols()) == 0 && acc != 0)
            os << "|"<< '\n';
        if ((acc % rhs.rows()) == 0 && acc != 0)
            os << "| ";
        os << v << " ";
        
        acc++;
    }
    os << '|';
    return os;
}
template<typename T>
std::ostream& operator<<(std::ostream& os, Eagle::Matrix<T, false>& rhs)
{
    auto acc = 0;
    
    for (auto i = 0; i < rhs.rows(); i++)
    {
        os << "| ";
        for (auto j = 0; j < rhs.cols(); j++)
            os << rhs(i, j) << " ";
        os << "|\n";

    }
    
    return os;
}

void main()
{
    /*Eagle::Matrix<int> NaizuMatrix{1,3,3};
    Eagle::Matrix<int> Other{ 2,2,-1 };
    std::cout << Other(1, 1) << '\n';
    
    Eagle::swap(NaizuMatrix, Other);
    Other(0, 1) = 4;
    std::cout << Other(0, 1) << '\n';
    Eagle::TMatrix<int> Ttp = Other.transpose();
    std::cout << Ttp(1, 0) << '\n';
    Eagle::Matrix<int> Det{ 3,3,0 };
    Det(0, 1) = 2;
    Det(0, 2) = 3;
    Det(0, 0) = 1;
    Det(1, 1) = -4;
    Det(1, 2) = 1;
    Det(2, 1) = 3;
    Det(2, 2) = -1;
    std::cout << Eagle::determinant(Det);
    std::cout << '\n' << "Original:\n";
    std::cout << Det << '\n';
    std::cout << '\n' <<  "Adj: \n" << Eagle::Adjugate( Det ) << '\n';
    std::cout << '\n' << "TAdj: \n" << Eagle::Adjugate(RandomFill<int>(2,2)).transpose() << '\n';*/
    int max, min = 0;
    std::cout << "Insert [min,max] number range separated by a space (i.e \"1 3\"):\n ";
    std::cin >> min >> max;
    std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
    start:

        
        Eagle::Matrix<float> Exercise = RandomFill<float>(3, 3,uni);
        std::cout << "Find Adjugate and determinant of:\n";
        std::cout << Exercise << std::endl;
        // time
        auto StartTime = std::time(nullptr);
        std::cin.get();

        std::cout << "Adjugate:\n" << Eagle::Adjugate(Exercise) << '\n';
        std::cout << "Determinant:\n" << Eagle::determinant(Exercise) << '\n';
        if(Eagle::determinant(Exercise) != 0)
        std::cout << "Inverse:\n" <<  Eagle::Adjugate(Exercise).transpose() * (1.0/Eagle::determinant(Exercise));
        // print elapsed time
        StartTime = std::time(nullptr) - StartTime;
        auto time = std::localtime(&StartTime);
        std::cout << time->tm_min << ":" << time->tm_sec << " elapsed!" << '\n';

        std::cin.get();

        goto start;

}