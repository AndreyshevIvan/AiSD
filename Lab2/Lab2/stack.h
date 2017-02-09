#pragma once

#ifndef STACK_H
#define STACK_H

#include <cassert> 
#include <iostream>

#include <string>
#include <iomanip> 

template <typename T>
class Stack
{
private:
	T *stackPtr;                   
	const int size;                   
	int top;                         
public:
	Stack(int = 10);  
	~Stack();                        

	inline void push(const T &);    
	inline T pop();                   
	inline void printStack();         
	inline const T &Peek(int) const;  
	inline int getStackSize() const;  
	inline T *getPtr() const;         
	inline int getTop() const;       
};

template <typename T>
Stack<T>::Stack(int maxSize) :
	size(maxSize)
{
	stackPtr = new T[size];
	top = 0;
}

template <typename T>
Stack<T>::~Stack()
{
	delete[] stackPtr;	
}

template <typename T>
inline void Stack<T>::push(const T &value)
{
	assert(top < size);

	stackPtr[top++] = value;
}

template <typename T>
inline T Stack<T>::pop()
{
	assert(top > 0);

	return stackPtr[--top];
}

template <class T>
inline const T &Stack<T>::Peek(int nom) const
{
	//
	assert(nom <= top);

	return stackPtr[top - nom];
}

template <typename T>
inline void Stack<T>::printStack()
{
	for (int ix = top - 1; ix >= 0; ix--)
		std::cout << "|" << std::setw(4) << stackPtr[ix] << std::endl;
}

template <typename T>
inline int Stack<T>::getStackSize() const
{
	return size;
}

template <typename T>
inline T *Stack<T>::getPtr() const
{
	return stackPtr;
}

template <typename T>
inline int Stack<T>::getTop() const
{
	return top;
}

#endif