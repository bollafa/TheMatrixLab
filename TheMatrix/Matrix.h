#pragma once
/*
    Compile-time size matrix.
*/
#include <iterator>
#include <vector>
#include <algorithm>
#include <cassert>
namespace Eagle
{
    template<typename T,bool RowMajor = true>
    class Matrix
    {
    private:

        using data_type = std::vector<T>;
        using size_type = typename std::vector<T>::size_type;
        using value_type = typename std::vector<T>::value_type;
        using const_reference = typename std::vector<T>::const_reference;
        using reference = typename std::vector<T>::reference;
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
    public:
        // constructors
        Matrix(size_type rows, size_type cols)
            : m_rows(rows), m_cols(cols), m_data(rows*cols)
        {}

        Matrix(size_type rows, size_type cols, const_reference val)
            : m_rows(rows), m_cols(cols), m_data( (rows*cols),val )
        {}
        Matrix(size_type rows, size_type cols, data_type val)
            : m_rows(rows), m_cols(cols), m_data(val)
        {}
        // operator
        reference operator()(const size_type row,
            const size_type column)
        {
            if constexpr(RowMajor)
            {
                return m_data[row*m_cols + column];
            }
            else
            {
                return m_data[column*m_rows+row];
            }
            
        }
        const_reference operator()(const size_type row,
            const size_type col) const
        {
            if constexpr(RowMajor)
            {
                return m_data[row*m_cols + col];
            }
            else
            {
                return m_data[column*m_rows + row];
            }
        }
        // iterators
        iterator begin() { return m_data.begin(); }
        iterator end() { return m_data.end(); }
        const_iterator cbegin() { return m_data.cbegin(); }
        const_iterator cend() { return m_data.cend(); }
        // size and capacity
        const size_type size() const { return m_data.size(); }
        const size_type max_size() const { return m_data.max_size(); }
        const size_type rows() const { return m_rows; }
        const size_type cols() const { return m_cols; }
        // utils
        void swap(Matrix & rhs)
        {
            m_data.swap(rhs.m_data);
            std::swap(m_cols, rhs.m_cols);
            std::swap(m_rows, rhs.m_rows);
        }
        Matrix<T, !RowMajor> transpose() const;
        /*{
            return Matrix<T, false>(m_cols, m_rows, m_data);
        }*/
        size_type determinant() const
        {
            
            if(m_cols != m_rows)
                throw std::exception("cannot find determinant of non-square matrix!");
            auto size = m_cols;
            auto det = 0;
            for (int col = 0, i = 1; col < size; col++, i = -i)
                det = det + i * (0);
            return det;
        }
    private:
        size_type m_cols{ 0u };
        size_type m_rows{ 0u };
        data_type m_data{};
    };
    template <typename T>
    void swap(Matrix<T>& lhs, Matrix<T>& rhs)
    {
        lhs.swap(rhs);
    }
    
    template<typename T>
    using TMatrix = Matrix<T, false>;
    template<typename T, bool RowMajor>
    inline Matrix<T, !RowMajor> Matrix<T, RowMajor>::transpose() const
    {
        return Matrix<T, !RowMajor>(m_cols, m_rows, m_data);
    }
    template<typename T, bool Major>
    int determinant(const Matrix<T, Major>& rhs)
    {
        if (rhs.cols() != rhs.rows())
            throw std::exception("cannot find determinant of non-square matrix!");
        auto size = rhs.cols();
        if (size == 1)
            return rhs(0, 0);
        auto det = 0;
        for (int col = 0, t = 1; col < size; col++, t = -t)
        {
            Matrix<T, Major> menor{ size - 1,size - 1 };
            // iterate over menor - create menor array
            for (int i = 0; i < size-1; i++)
            {

                for (int j = 0; j < size - 1; j++)
                {
                    auto destCol = col + t + t*i;
                    if (destCol < 0) destCol = rhs.cols() + destCol;
                    menor(j, i) = rhs((1 + j) % (size), (destCol) % (size));
                }
                    
            }
            det = det + t * (determinant(menor)) * rhs(0,col);
        }
            
        return det;
    }
    template<typename T, bool Major>
    Matrix<T,Major> Adjugate(const Matrix<T, Major>& rhs)
    {
       
        if (rhs.cols() != rhs.rows())
            throw std::exception("cannot find determinant of non-square matrix!");
        Matrix<T, Major> lhs{ rhs.rows(),rhs.cols() };
        auto size = rhs.cols();
        
        for (int col = 0, t = 1; col < size; col++, t = -t)
        {
            
            
            for (int row = 0, w = 1; row < size; row++, w = -w)
            {
                Matrix<T, Major> menor{ size - 1,size - 1 };
                // iterate over menor - create menor array
                for ( int i = 0; i < size - 1; i++)
                {
                    for (int j = 0; j < size - 1; j++)
                    {
                        auto destRow = row + w + w*j;
                        auto destCol = col + 1 + i;
                        if (destRow < 0) destRow = rhs.rows() + destRow;
                        if (destCol < 0) destCol = rhs.cols() + destCol;
                        menor(j, i) = rhs((destRow) % (size), (destCol) % (size));
                    }
                        
                }
                lhs(row,col) =  w * (determinant(menor));
                
            }
            
        }

        return lhs;
    }
    template<typename T,bool Major,typename R>
    Matrix<T, Major> operator*(const Matrix<T, Major>& lhs, const R& rhs)
    {
        Matrix<T, Major> ret{ lhs };
        for (auto& v : ret)
        {
            v *= rhs;
        }
        return ret;
    }
}
