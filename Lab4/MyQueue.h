#ifndef MYQUEUE_H_
#define MYQUEUE_H_
#include "MyDLNode.h"
#include <cstddef>

template <class T>
class MyQueue{
    private:
        MyDLNode<T>* front = NULL;
        MyDLNode<T>* bottom = NULL;
    public:
        int size = 0;

        MyQueue(){}
        ~MyQueue(){
            while (front != NULL){
                MyDLNode<T>* tmp = front;
                front = front->next;
                delete tmp;
            }
        }
        void push(T in){
            MyDLNode<T>* temp = new MyDLNode<T>(in);
            temp->next = NULL;
            if (top == NULL){
                front = bottom = temp;
            } else {
                bottom->next = temp;
                bottom = temp;
            }
            size++;
        }
        T pop(){
            if (front == NULL){
                return NULL;
            } 
            T output = front->value;

            if(front == bottom){
                front = bottom = NULL;
            } else {
                front = front->next;
            }
            size--;
            return output;
        }
};

#endif
