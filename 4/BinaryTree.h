#pragma once
#include "Entry.h"

template <typename T>
struct Node
{
	T data;
	Node* left;
	Node* right;

	Node(T _data, Node* _left = nullptr, Node* _right = nullptr) : data(_data), left(_left), right(_right) {} 
};

template <typename T>
class BinaryTree
{
public:
	BinaryTree() : root(nullptr) {}
	BinaryTree(BinaryTree const& other) : root(copy(other.root)) {}
	BinaryTree(DynamicArray<Entry> const& dArr);
	BinaryTree& operator=(BinaryTree const& other);
	~BinaryTree();
	
	void add(T const& _data);
	bool search(T const& data);
	bool remove(T const& data);
	int removeAll(int key);
	
private: 
	Node<T> * root;

	//private methods
	void free(Node<T> *& _root);
	Node<T>* copy(Node<T> * otherRoot);

	//helpers
	Node<T> *& searchHelper(T const& _data, Node<T> *& r);
	void addHelper(T const& _data, Node<T> *& r);
	Node<T> *& removeRecurring(int key, Node<T> *& r, int &count);
	int removeAllHelper(int key, Node<T> *& r);
	void removeHelper(Node<T> *& r);
	void initializeBinaryTree(DynamicArray<Entry> const& dArr);
	void initializeBinaryTreeHelper(DynamicArray<Entry> const& dArr, int beg, int end, Node<T> *& r);
};

//PUBLIC
template <typename T>
BinaryTree<T>::BinaryTree(DynamicArray<Entry> const& dArr)
{
	if(!dArr.isEmpty())
	{
		dArr.sort();
		this->initializeBinaryTree(dArr);
	}
	else
	{
		this->root = nullptr;		
	}
}

template <typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree const& other)
{
	if(this != &other)
	{
		this->free(root);
		this->root = copy(other.root);
	}

	return *this;
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
	this->free(this->root);
}

template <typename T>
void BinaryTree<T>::add(T const& data)
{
	addHelper(data, this->root);
}

template <typename T>
bool BinaryTree<T>::search(T const& data)
{
	return searchHelper(data, this->root) != nullptr;
}

template <typename T>
int BinaryTree<T>::removeAll(int key)
{
	return removeAllHelper(key, this->root);
}

template <typename T>
bool BinaryTree<T>::remove(T const& data)
{
	Node<T> *& searchedFor = searchHelper(data, this->root);

	if(searchedFor)
	{
		removeHelper(searchedFor);	
		return true;
	}
	else
	{
		return false;
	}	
}

//PRIVATE

template <typename T>
void BinaryTree<T>::free(Node<T> *& _root)
{
	if(!_root)
		return;

	free(_root->left);
	free(_root->right);
	delete _root;
}

template <typename T>
Node<T>* BinaryTree<T>::copy(Node<T> * otherRoot)
{
	if(!otherRoot)
		return nullptr;

	return new Node<T>(otherRoot->data, copy(otherRoot->left), copy(otherRoot->right));
}

template <typename T>
void BinaryTree<T>::initializeBinaryTree(DynamicArray<Entry> const& dArr)
{
	int arSize = dArr.getSize();
	initializeBinaryTreeHelper(dArr, 0, arSize-1, this->root);
}

template <typename T>
Node<T>*& BinaryTree<T>::searchHelper(T const& _data, Node<T> *& r)
{
	if(!r)
		return r;
	
	if(r->data > _data)
	{
		return searchHelper(_data, r->left);
	}
	else if(_data > r->data)
	{
		return searchHelper(_data, r->right);
	}
	else if(_data == r->data)
	{
		return r;	
	}
	else
	{
		return searchHelper(_data, r->right);
	}
}

template <typename T>
void BinaryTree<T>::addHelper(T const& _data, Node<T> *& r)
{
	if(!r)
	{
		r = new Node<T>(_data);
		return;
	}
			
	if(r->data > _data)
	{
		addHelper(_data, r->left);
	}
	else if(_data > r->data)
	{
		addHelper(_data, r->right);
	}
	else
	{
		Node<T> * tmp = r->right;
		r->right = new Node<T>(_data);
		r->right->right = tmp;
	}
}

//this will only work for <Entry> (?)
template <typename T>
Node<T> *& BinaryTree<T>::removeRecurring(int key, Node<T> *& r, int &count)
{
	if(!r)
		return r;

	while(r->data.getKey() == key)
	{
		Node<T> * tmp = r;
		r = r->right;
		delete tmp;
		count++;
		if(!r)
			break;
	}

	return r;
}

//only for <Entry>. maybe play with the operators in class Entry, but it doesn't seem like a good idea.
template <typename T>
int BinaryTree<T>::removeAllHelper(int key, Node<T> *& r)
{
	if(!r)
		return 0;

	if(r->data.getKey() > key)
	{
		removeAllHelper(key, r->left);		
	}
	else if(r->data.getKey() < key)
	{
		removeAllHelper(key, r->right);
	}
	else
	{
		//check for == keys (all will be stored on the right subtrees)
		int counter = 1;
		//it returns the next unique element
		Node<T> *& unq = removeRecurring(key, r->right, counter);
		r->right = unq;
		//one more left to remove 
		removeHelper(r);

		return counter;
	}
}

template <typename T>
void BinaryTree<T>::removeHelper(Node<T> *& r)
{
	//check if there is right subtree
	if(r->right)
	{
		//check if the right subtree has a left subtree
		if(!(r->right->left))
		{
			//if it doesn't -> we just assign the right subtree to the element that should be removed
			Node<T> * tmp = r->right;
			delete r;
			r = tmp;
		}
		else
		{
			//the right subtree has a left subtree
			Node<T> * tmp = r->right->left;

			//go to bottom of the left subtree
			while(tmp->left)
			{
				tmp = tmp->left;
			}

			//we've reached the bottom of the left subtree
			//if there is no right subtree
			if(!tmp->right)
			{
				//swap the data, and delete the lowest left
				r->data = tmp->data;
				delete tmp;	
			}
			else
			{
				//if there is right, lowest left = it's right subtree
				r->data = tmp->data;
				//tmp = tmp->right;
				//(messed up pointers earlier, leaving it like that)
				tmp->data = tmp->right->data;
				Node<T> * sndTmp = tmp->right;
				sndTmp = tmp->right->right;
				delete tmp->right;
				tmp->right = sndTmp;
			} 
		}
	}
	else if(r->left)
	{
		Node<T> * tmp = r->left;
		delete r;
		r = tmp;
	}
	else
	{
		//only element, no left, no right -> just delete
		delete r;
		r = nullptr;
	}
}

template <typename T>
void BinaryTree<T>::initializeBinaryTreeHelper(DynamicArray<Entry> const& dArr, int beg, int end, Node<T> *& r)
{
	if(beg > end)
	{
		return;
	}

	int mid = beg + (end-beg)/2;
	r = new Node<T>(dArr[mid]);
	initializeBinaryTreeHelper(dArr, beg, mid-1, r->left);
	initializeBinaryTreeHelper(dArr, mid+1, end, r->right);
}