#pragma once
#include "assert.h"

const int RESIZE_COEF = 2;

template <typename T>
class DynamicArray
{
public:
	DynamicArray();
	DynamicArray(const DynamicArray& other);
	DynamicArray& operator=(const DynamicArray& other);
	~DynamicArray();
	
	void add(const T& newElement);
	bool isEmpty() const;
	void sort() const;
	T operator[](int idx) const;
	int getSize() const;
	

private:
	T * data;
	int size;
	int capacity;

	void free();
	void copy(const DynamicArray& other);
	void resize();
	T at(int idx) const;
};

template <typename T>
DynamicArray<T>::DynamicArray() 
{
	this->size = 0;
	this->capacity = 8;
	this->data = new T[capacity];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& other)
{
	this->copy(other);
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other)
{
	if(this != &other)
	{
		this->free();
		this->copy(other);
	}

	return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
	this->free();
}

template <typename T>
void DynamicArray<T>::add(const T& newElement)
{
	if(size == capacity-1)
	{
		this->resize();
	}

	this->data[size] = newElement;
	size++;
}

template <typename T>
bool DynamicArray<T>::isEmpty() const
{
	return size == 0;
}

template <typename T>
void DynamicArray<T>::sort() const
{
	//selection sort
	for(int i = 0; i < this->size; i++)
	{
		int minId = i;
		T minVal = data[minId];

		for(int j = i+1; j < this->size; j++)
		{
			if(minVal > data[j])
			{
				minVal = data[j];
				minId = j;
			}
		}

		if(minId != i)
		{
			T temp = data[i];
			data[i] = minVal;
			data[minId] = temp;
		}
	}
}

template <typename T>
T DynamicArray<T>::operator[](int idx) const
{
	return at(idx);
}

template <typename T>
int DynamicArray<T>::getSize() const
{
	return this->size;
}

//PRIVATE 
template <typename T>
void DynamicArray<T>::free()
{
	delete[] this->data;
}

template <typename T>
void DynamicArray<T>::copy(const DynamicArray& other)
{
	this->size = other.size;
	this->capacity = other.capacity;
	this->data = new Entry[capacity];

	for(int i = 0; i < size; i++)
	{
		this->data[i] = other.data[i];
	}
}

template <typename T>
void DynamicArray<T>::resize()
{
	this->capacity *= RESIZE_COEF;

	T * temp = new T[capacity];

	for(int i = 0; i < this->size; i++)
	{
		temp[i] = this->data[i];
	}

	delete[] data;
	data = temp;
}

template <typename T>
T DynamicArray<T>::at(int idx) const
{
	assert(idx >= 0 && idx < size);

	return data[idx];
}