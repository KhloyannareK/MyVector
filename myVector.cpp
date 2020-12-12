#include "myVector.h"
#include <iostream>

namespace mvec {
	
	myVector::myVector()
	try 
	: m_capacity{DEFAULT_CAPACITY},
	  m_data{new valueType[m_capacity]},
	  m_size{0}
	{}
	catch (std::bad_alloc& ex) {
		std::cerr << "No enough memory\n";
		throw;
	}
	catch (...) {
		std::cerr << "Vector object creating failed\n";
		throw;
	}

	myVector::myVector(Count c)
	try
	: m_capacity{ c.size == 0 ? DEFAULT_CAPACITY : 2 * c.size},
	  m_data{ new valueType[m_capacity] },
	  m_size{c.size}
	{}
	catch (std::bad_alloc& ex) {
		std::cerr << "No enough memory\n";
		throw;
	}
	catch (...) {
		std::cerr << "Vector object creating failed\n";
		throw;
	}

	myVector::myVector(Count c, valueType defaultValue) 
	try
	: m_capacity{ c.size == 0 ? DEFAULT_CAPACITY : 2 * c.size },
	  m_data{ new valueType[m_capacity]{defaultValue} },
	  m_size{ c.size }
	{}
	catch (std::bad_alloc& ex) {
		std::cerr << "No enough memory\n";
		throw;
	}
	catch (...) {
		std::cerr << "Vector object creating failed\n";
		throw;
	}

	myVector::myVector(std::initializer_list<valueType> l) 
		: m_size{ l.size() }
	{
		try {
			m_capacity = (m_size ? 2 * m_size : DEFAULT_CAPACITY);
			m_data = new valueType[m_capacity];
			sizeType i = 0;
			for (valueType x : l)
				m_data[i++] = x;
		}
		catch (std::bad_alloc& ex) {
			std::cerr << "No enough memory\n";
			throw;
		}
		catch (...) {
			std::cerr << "Vector object creating failed\n";
			throw;
		}
	}

	myVector::myVector(const myVector& vec) 
	try
		: m_capacity{vec.m_capacity},
		  m_data{new valueType[m_capacity]},
		  m_size{vec.m_size}
	{
		for (sizeType i = 0; i < m_size; ++i)
			m_data[i] = vec[i];
	}
	catch (std::bad_alloc& ex) {
		std::cerr << "No enough memory\n";
		throw;
	}
	catch (...) {
		std::cerr << "Vector object creating failed\n";
		throw;
	}

	myVector::myVector(myVector&& vec)
		: m_capacity{vec.m_capacity},
		  m_data{vec.m_data},
		  m_size{vec.m_size}
	{ 
		vec.m_data = NULL; 
	}

	myVector& myVector::operator=(const myVector& vec)
	{
		delete[] m_data;
		m_capacity = vec.m_capacity;
		m_data = new valueType[m_capacity];
		m_size = vec.m_size;

		for (sizeType i = 0; i < m_size; ++i)
			m_data[i] = vec[i];

		return *this;
	}

	myVector& myVector::operator=(myVector&& vec)
	{
		m_capacity = vec.m_capacity;
		m_data = vec.m_data;
		m_size = vec.m_size;

		vec.m_data = NULL;

		return *this;
	}

	myVector::~myVector()
	{
		delete[] m_data;
	}

	
	myVectorIterator myVector::begin()
	{	return myVectorIterator{ m_data }; }

	myVectorIterator myVector::end()
	{	return myVectorIterator{ m_data + m_size }; }

	
	bool myVector::operator==(const myVector& vec) const
	{
		if (m_size != vec.m_size)
			return false;
		
		for (sizeType i = 0; i < m_size; ++i)
			if (m_data[i] != vec.m_data[i])
				return false;

		return true;
	}

	bool myVector::operator!=(const myVector& vec) const
	{
		return !(*this == vec);
	}

	
	valueType& myVector::operator[](sizeType i) const
	{
		if (i >= m_size)
			throw outOfBounds(i);

		return m_data[i];
	}


	void myVector::pop_back()
	{
		if (empty()) {
			throw popOnEmptyVector{};
		}

		--m_size;
	}

	void myVector::push_back(valueType value)
	{
		if (m_size != m_capacity) {
			m_data[m_size++] = value;
			return;
		}

		try {
		    m_capacity = m_capacity << 2;
			valueType* holder = new valueType[m_capacity];
			for (sizeType i = 0; i < m_size; ++i)
				holder[i] = m_data[i];
			holder[m_size++] = value;
			
			delete[] m_data;
			m_data = holder;
		}
		catch (std::bad_alloc& ex) {
			m_capacity = m_capacity >> 2;
			std::cerr << "No enough memory\n";
			throw;
		}
	}

	void myVector::reserve(sizeType sz)
	{
		if (sz == m_capacity - m_size)
			return;

		try{
			valueType* holder = new valueType[m_size + sz];
		    m_capacity = m_size + sz;

			for (sizeType i = 0; i < m_size; ++i)
				holder[i] = m_data[i];

			delete m_data;
			m_data = holder;
	    }
	    catch (std::bad_alloc& ex) {
	    	std::cerr << "No enough memory\n";
	    	throw;
	    }
	}

	void myVector::resize(sizeType sz)
	{	
		if (sz < m_capacity) {
		    m_size = sz;
			return;
		}

		try {
			valueType* holder = new valueType[sz * 2];
			m_size = sz;
			m_capacity = 2 * m_size;
			for (sizeType i = 0; i < m_size; ++i)
				holder[i] = m_data[i];

			delete[] m_data;
			m_data = holder;
		}
		catch (std::bad_alloc& ex) {
			std::cerr << "No enough memory\n";
			throw;
		}
		
	}

	void myVector::erase(sizeType index)
	{
		if (index >= m_size)
			throw outOfBounds(index);
		--m_size;
		for (sizeType i = index; i < m_size; ++i)
			m_data[i] = m_data[i + 1];
	}


	sizeType myVector::size() const
	{
		return m_size;
	}

	bool myVector::empty() const
	{
		return m_size == 0;
	}

	std::ostream& operator<<(std::ostream& out, const myVector& vec)
	{
		sizeType size = vec.size();
		for (sizeType i = 0; i < size; ++i)
			out << vec[i] << ' ';
		out << std::endl;

		return out;
	}

}