#pragma once

template <typename T>
struct Node
{
	Node<T> *next, *previous;
	T data;

	Node(T _data, Node<T> *_next = nullptr, Node<T> *_previous = nullptr)
		: data(_data), next(_next), previous(_previous) {}
};