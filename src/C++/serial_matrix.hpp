#include "matrix.hpp"
#include <cstdlib>
#include <algorithm>
#include <iterator>

template<typename T>
class serial_matrix: public matrix<T>
{
private:
    T* m_matrix; //array containing the actual matrix data
public:
    //interaface methods
    std::unique_ptr<matrix<T>> operator*(const matrix<T>& matrix) override;
    T& get_val(const size_t i,const size_t j) override;
	const T& get(const size_t i, const size_t j)const override;

    //class methods
    serial_matrix(const size_t width,const size_t height);
	serial_matrix(const size_t width, const size_t height, const T matrix_data[]);
	serial_matrix(const size_t width, const size_t height, T*&& matrix_data);//move constructor for dynamic array
    ~serial_matrix() override;
};

template<typename T>
serial_matrix<T>::serial_matrix(const size_t width,const size_t height):matrix<T>(width,height)
{
	this->m_matrix = new T[width*height]();
	std::fill(this->m_matrix, this->m_matrix + (width*height), T());
}

template<typename T>
serial_matrix<T>::serial_matrix(const size_t width, const size_t height, const T matrix_data[]):matrix<T>(width, height)
{
	this->m_matrix = new T[width * height]();
	std::copy(matrix_data, matrix_data+(width*height),this->m_matrix);
}

template<typename T>
inline serial_matrix<T>::serial_matrix(const size_t width, const size_t height, T*&& matrix_data):matrix<T>(width, height)
{
	this->m_matrix = matrix_data;
}

template<typename T>
serial_matrix<T>::~serial_matrix()
{
	delete this->m_matrix;
}

template<typename T>
inline std::unique_ptr<matrix<T>> serial_matrix<T>::operator*(const matrix<T>& matrix)
{
	T* res = new T[this->m_width * this->m_height]();
	std::fill(res, res + (this->m_width*this->m_height), T());

	//calculate the matrix
	{
		for (size_t line = 0; line < this->get_height(); ++line)
		{
			for (size_t col = 0; col < this->get_width(); ++col)
			{
				for (size_t second_col = 0; second_col < this->get_width(); ++second_col)
				{
					res[line * this->get_width() + col] += this->get_val(line, col) * matrix.get(line, second_col);
				}
			}
		}
	}

	return std::make_unique<serial_matrix<T>>(this->m_width,this->m_height,std::move(res));
}

template<typename T>
inline T& serial_matrix<T>::get_val(const size_t i,const size_t j)
{
	return this->m_matrix[i * this->m_width + j];
}

template<typename T>
inline const T& serial_matrix<T>::get(const size_t i, const size_t j)const
{
	return this->m_matrix[i * this->m_width + j];
}
