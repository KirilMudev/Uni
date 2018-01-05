#pragma once
#include "Node.h"
#include "Iterator.h"
#include "Tab.h"

template <typename T>
class DLList
{
public:
	DLList();
	DLList(DLList<T> const& other);
	DLList& operator=(DLList<T> const& other);
	~DLList();

	bool isEmpty() const;
	void append(T const& data);
	bool removeElement(int index);
	bool insertAfter(int index, T const& element);
	bool insertBefore(int index, T const& element);
	void removeFirst();
	bool addFirst(T const& element);
	T& getAt(int index) const;
	T& getFirst() const;
	T& getLast() const;
	int getSize() const;
	Iterator<T> getIterator() const;
	void splitList(Node<T> * source, Node<T> ** frontRef, Node<T> ** backRef);
	void sort(int &key);
	
private:
	Node<T> *first;
	Node<T> *last;
	int size;

	void free();
	void copy(DLList<T> const& other);
	Node<T>* findNodeAt(int index) const;
	void fixPointers();

	void mergeSort(Node<T> ** referenceToFirst, int &key);
	Node<T> * sortedMerge(Node<T> *a,  Node<T> *b, int &key);
};

template <typename T>
DLList<T>::DLList()
{
	first = last = nullptr;
	this->size = -1;
}
 
template <typename T>
DLList<T>::DLList(DLList const& other) : first(nullptr), last(nullptr)
{
	copy(other);
}

template <typename T>
DLList<T>& DLList<T>::operator=(DLList<T> const& other)
{
	if (this != &other)
	{
		free();
		copy(other);
	}

	return *this;
}

template <typename T>
DLList<T>::~DLList()
{
	free();
}

template <typename T>
bool DLList<T>::isEmpty() const
{
	return first == nullptr;
}

template <typename T>
void DLList<T>::append(T const& data)
{
	if (isEmpty())
	{
		first = last = new Node<T>(data);
		this->size++;
	}
	else
	{
		Node<T> *newNode = new Node<T>(data, nullptr, last);
		last->next = newNode;
		last = last->next;
		this->size++;
	}
}

template <typename T>
void DLList<T>::free()
{
		Node<T> *toDelete = first;
		Node<T> *temp;

		while (toDelete != nullptr)
		{
			temp = toDelete;
			toDelete = toDelete->next;
			delete temp;
		}
}

template <typename T>
void DLList<T>::copy(DLList<T> const& other)
{
		if (other.first == nullptr)
		{
			return;
		}

		Node<T> *toCopy = other.first;
		Node<T> *newNode = new Node<T>(toCopy->data);
		Node<T> *lastCopied = newNode;

		first = newNode;

		while(toCopy->next != nullptr)
		{
			toCopy = toCopy->next;
			newNode = new Node<T>(toCopy->data, nullptr, lastCopied);
			lastCopied->next = newNode;
			lastCopied = newNode;
		}

		last = newNode;
		this->size = other.size;
}

template <typename T>
bool DLList<T>::removeElement(int index)
{
	Node<T> *itr = findNodeAt(index);

	if(itr)
	{
		if(itr->next == nullptr && itr->previous != nullptr)
		{
			last = itr->previous;
			itr->previous->next = nullptr;
			delete itr;
			size--;
			return true;
		}
		else if (itr->next != nullptr && itr->previous == nullptr)
		{
			first = itr->next;
			itr->next->previous = nullptr;
			delete itr;
			size--;
			return true;
		}
		else if (itr->next == nullptr && itr->previous == nullptr)
		{
			delete itr;
			first = last = nullptr;
			size--;
			return true;
		}
		else
		{
			itr->previous->next = itr->next;
			itr->next->previous = itr->previous;
			delete itr;
			size--;
			return true;
		}
	}

	return false;
}

template <typename T>
Node<T>* DLList<T>::findNodeAt(int index) const
{
	if(index < 0)
	{
		return nullptr;
	}
	else
	{
		Node<T> *itr = this->first;

		for(int i = 0; i < index; i++)
		{
			if(itr == nullptr)
			{
				return nullptr;
			}
			
			itr = itr->next;
		}

		return itr;
	}
}

template <typename T>
bool DLList<T>::insertAfter(int index, T const& element)
{
	Node<T> *nodeAt = findNodeAt(index);

	if(nodeAt) //node exists
	{
		Node<T> *newNode = new Node<T>(element, nodeAt->next, nodeAt);

		//if(!newNode) return false;

		nodeAt->next = newNode;
		nodeAt->next->previous = newNode;
		
		if(nodeAt == this->last) //if last element
		{
			this->last = newNode;
			this->size++;
			return true;
		}

		this->size++;
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
bool DLList<T>::insertBefore(int index, T const& element)
{
	if(index < 0)
	{
		return false;
	}
	else if(index == 0)
	{
		return addFirst(element);
	}
	else
	{
		return insertAfter(index-1);
	}
}

template <typename T>
void DLList<T>::removeFirst()
{
	if(!this->isEmpty())
	{
		//first != null

		Node<T> *oldFirst = this->first;

		this->first = this->first->next;

		if(this->first == nullptr)
		{
			this->last = nullptr;
		}

		delete oldFirst;
		this->size--;
	}
}

template <typename T>
bool DLList<T>::addFirst(T const& element)
{
	Node<T> *newNode = new Node<T>(element, this->first);

	if(newNode)
	{
		if(this->isEmpty())
		{
			this->last = newNode;
		}

		if(first->next)
		{
			this->first->next->previous = newNode;
		}
		
		this->first = newNode;
		this->size++;
	}

	return newNode != nullptr;
}

template <typename T>
T& DLList<T>::getAt(int index) const
{
	Node<T> *tmp = findNodeAt(index);
	return tmp->data;
}

template <typename T>
T& DLList<T>::getFirst() const
{
	return this->first->data;
}

template <typename T>
T& DLList<T>::getLast() const
{
	return this->last->data;
}

template <typename T>
int DLList<T>::getSize() const
{
	return this->size;
}

template <typename T>
Iterator<T> DLList<T>::getIterator() const
{
	return Iterator<T>(this->first);
}

template <typename T>
void DLList<T>::sort(int &key)
{
	this->mergeSort(&first, key);
	this->fixPointers();
}

template <typename T>
void DLList<T>::mergeSort(Node<T> ** referenceToFirst, int &key)
{
	Node<T> *head = *referenceToFirst;
    Node<T> *pFirstHalf; 
    Node<T> *pSecondHalf;

	//recursion bottom (1 or less elements)
    if ((head == nullptr) || (head->next == nullptr))
    {
		return;
    }
 
    splitList(head, &pFirstHalf, &pSecondHalf); //split list, beginning from current point
 
	//recursion on both lists
    mergeSort(&pFirstHalf, key);
    mergeSort(&pSecondHalf, key);
 
	//first becomes the merged sorted lists
	*referenceToFirst = sortedMerge(pFirstHalf, pSecondHalf, key);
}

template <typename T>
Node<T> * DLList<T>::sortedMerge(Node<T> *leftSide,  Node<T> *rightSide, int &key)
{
	Node<T> *result = nullptr; //will point to the first element of sorted list
 
	//bottom, if halves are empty
    if (leftSide == nullptr)
	{
		return rightSide;
	}   
    else if (rightSide == nullptr)
	{
		return leftSide;
	}  

	//comparison
	if (key == 1)
	{
		if (leftSide->data.getUrl() <= rightSide->data.getUrl())
		{
			if (leftSide->data.getUrl() == rightSide->data.getUrl())
			{
				if (leftSide->data.getTimestamp() <= rightSide->data.getTimestamp())
				{
					result = leftSide;
					result->next = sortedMerge(leftSide->next, rightSide, key);
				}
				else
				{
					result = rightSide;
					result->next = sortedMerge(leftSide, rightSide->next, key);
				}
			}
			else
			{
				result = leftSide;
				result->next = sortedMerge(leftSide->next, rightSide, key);
			}
		}
		else
		{
			result = rightSide;
			result->next = sortedMerge(leftSide, rightSide->next, key);
		}
	}

	if (key == 2)
	{
		if (leftSide->data.getTimestamp() <= rightSide->data.getTimestamp())
		{
			result = leftSide;
			result->next = sortedMerge(leftSide->next, rightSide, key);
		}
		else
		{
			result = rightSide;
			result->next = sortedMerge(leftSide, rightSide->next, key);
		}
	}
   
	return result;
}

template <typename T>
void DLList<T>::splitList(Node<T> * source, Node<T> ** frontRef, Node<T> ** backRef)
{
	Node<T> * fast;
	Node<T> * slow;

	//if less than two elements
	if (source == nullptr || source->next == nullptr)
	{
		*frontRef = source;
		*backRef = nullptr;
    }
    else
    {
		slow = source;
		fast = source->next;

		//two steps for fast, one step for slow
		while (fast != nullptr)
		{
			fast = fast->next;
			if (fast != nullptr)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}
 
		//slow is at the middle right now (if odd number, front list has +1 elements)
		*frontRef = source;
		*backRef = slow->next;
		slow->next = nullptr;
    }
}

template <typename T>
void DLList<T>::fixPointers()
{
	Node<T> * tmp = first;
	first->previous = nullptr;
	while (tmp != nullptr && tmp->next != nullptr)
	{
		tmp->next->previous = tmp;
		if (tmp->next->next == nullptr)
		{
			tmp->next->previous = tmp;
		}
		tmp = tmp->next;
	}
}