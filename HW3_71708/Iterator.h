#pragma once
#include "Node.h"

template <class T>
class Iterator
{
public:
	Iterator(Node<T> *_start);

	bool moveToNext();
	bool isAtEnd() const;
	T const& getCurrent() const;
	void setCurrent(T const &value);
	void restart();
	bool removeCurrent();

private:
	Node<T> *start;
	Node<T> *current;
};

template <typename T>
Iterator<T>::Iterator(Node<T> *_start)
{
	this->start = this->current = _start;
}

template <typename T>
bool Iterator<T>::moveToNext()
{
	if(this->current)
	{
		this->current = this->current->next;
		return true;
	}

	return false;
}

template <typename T>
bool Iterator<T>::isAtEnd() const
{
	//edit this. final element == start -> means we've reached end
	return this->current->next == nullptr;
}

template <typename T>
void Iterator<T>::setCurrent(T const& value)
{
	//assert( ! this->isAtEnd() );
	this->current->data = value;
}

template <typename T>
T const& Iterator<T>::getCurrent() const
{
	//assert ///only if !isAtEnd
	return this->current->data;
}

template <typename T>
void Iterator<T>::restart()
{
	this->current = this->start;
}

template <typename T>
bool Iterator<T>::removeCurrent()
{
	if(this->current)
	{
		if(current->next == nullptr && current->previous != nullptr)
		{
			Node<T> *tmp = current->previous;
			current->previous->next = nullptr;
			delete current; 
			current = tmp;
			return true;
		}
		else if (current->next != nullptr && current->previous == nullptr)
		{
			Node<T> *tmp = current->next;
			current->next->previous = nullptr;
			delete current;
			current = tmp;
			return true;
		}
		else if (current->next == nullptr && current->previous == nullptr)
		{
			delete current;
			start = current = nullptr; //hm
			return true;
		}
		else
		{
			current->previous->next = current->next;
			current->next->previous = current->previous;
			delete current;
			return true;
		}
	}

	return false;
}