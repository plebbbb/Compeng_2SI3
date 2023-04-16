#ifndef MYSTACK_H_
#define MYSTACK_H_
#include "MyDLNode.h"
#include <cstddef>

template <class T>
class MyStack{
    private:
        MyDLNode<T>* dummy;
        MyDLNode<T>* top;
    public:
        MyStack(){
            dummy = new MyDLNode<T>();
            top = dummy;
            size = 0;
        }
        ~MyStack(){
            while (dummy != NULL){
                MyDLNode<T>* tmp = dummy;
                dummy = dummy->next;
                delete tmp;
            }
        }
        void push(T in){
            top->next = new MyDLNode<T>(in);
            top->next->prev = top;
            top = top->next;
            size++;
        }
        T pop(){
            if (top == dummy) return NULL;
            T output = top->value;
            top = top->prev;
            delete top->next;
            top->next = NULL;
            size--;
            return output;
        }

        T get_top(){
            return top->value;
        }
        int size = 0;
};

#endif
