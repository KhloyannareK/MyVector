#include <initializer_list>
#include <iostream>
#ifndef MY_VECTOR_H
#define MY_VECTOR_H

namespace mvec {
	using valueType = int;
	using sizeType = size_t;
	struct Count {
		sizeType size;
	};

	class myVectorIterator;
	class myVector
	{
	public:
		// construction & clean-up
		myVector(); //default constructor
		myVector(Count size); //ordinary constructor
		myVector(Count size, valueType defaultValue); //ordinary constructor
		myVector(std::initializer_list<valueType>); //initializer list constructor
		myVector(const myVector&); //copy constructor
		myVector(myVector&&); //move contructor
		myVector& operator=(const myVector&); //copy assignment
		myVector& operator=(myVector&&); //move assignment
		~myVector();

		// iterator related
		myVectorIterator begin();
		myVectorIterator end(); // one past the last element

		// comparision
		bool operator==(const myVector&) const;
		bool operator!=(const myVector&) const;

		// index-access
		valueType& operator[](sizeType) const;

		// operations on the vector
		void pop_back();
		void push_back(valueType);
		void reserve(sizeType); // m_capacity = m_size + <argument>
		void resize(sizeType);
		void erase(sizeType);

		// exploring vector
		sizeType size() const;
		bool empty() const;
	private:
		sizeType m_capacity;
		valueType* m_data{};
		sizeType m_size;
		static const sizeType DEFAULT_CAPACITY{ 8 };
		friend std::ostream& operator<<(std::ostream&, const myVector&);
	};


	struct myVectorException {
		virtual void what() const {
			std::cout << "Vector exception\n";
		}
	};

	struct popOnEmptyVector : public myVectorException {
		virtual void what() const {
			std::cout << "pop_back() is called on empty vector.\n";
		}
	};

	struct outOfBounds : public myVectorException {
		outOfBounds(sizeType index)
			: myVectorException(),
			m_index{ index }
		{}
		void what() const {
			std::cout << "trying to acces element at positioon "
				<< m_index << " which is out of bounds.\n";
		}
	private:
		sizeType m_index;
	};

	enum iteratorCategories { input, bidirectional, random_access };

	class myVectorIterator
	{
	public:
		myVectorIterator() = default;
		myVectorIterator(valueType* ptr)
			: m_ptr{ ptr }
		{}


		valueType& operator*()
		{
			return *m_ptr;
		}

		valueType& operator[](size_t i)
		{
			return m_ptr[i];
		}

		myVectorIterator& operator++()
		{
			if (!m_ptr)
				throw myVectorIteratorException{"operator++ used for uninitialized iterator\n"};
			++m_ptr;
			return *this;
		}

		myVectorIterator operator++(int)
		{
			if (!m_ptr)
				throw myVectorIteratorException{ "operator++ used for uninitialized iterator\n" };
			myVectorIterator tmp{ m_ptr };
			++m_ptr;
			return tmp;
		}

		myVectorIterator& operator--()
		{
			if (!m_ptr)
				throw myVectorIteratorException{ "operator-- used for uninitialized iterator\n" };
			--m_ptr;
			return *this;
		}

		myVectorIterator operator--(int)
		{
			if (!m_ptr)
				throw myVectorIteratorException{ "operator-- used for uninitialized iterator\n" };
			myVectorIterator tmp{ m_ptr };
			--m_ptr;
			return tmp;
		}

		bool operator==(const myVectorIterator&it) { return m_ptr == it.m_ptr; }
		bool operator!=(const myVectorIterator&it) { return m_ptr != it.m_ptr; }
		bool operator<=(const myVectorIterator&it) { return m_ptr <= it.m_ptr; }
		bool operator>=(const myVectorIterator&it) { return m_ptr >= it.m_ptr; }
		bool operator<(const myVectorIterator& it) { return m_ptr < it.m_ptr; }
		bool operator>(const myVectorIterator&it) { return m_ptr > it.m_ptr; }
		bool operator+=(int i) { return m_ptr + i; }
		bool operator-=(int i) { return m_ptr - i; }

		iteratorCategories category() const
		{
			return random_access; 
		}
	private:
		valueType* m_ptr{ NULL };
	public:
		struct myVectorIteratorException {
			myVectorIteratorException(const std::string& s)
				: m_msg{ s }
			{}
			void what() const { std::cout << m_msg << std::endl; }
			std::string m_msg{};
		};
	};

}
#endif // !MY_VECTOR_H
