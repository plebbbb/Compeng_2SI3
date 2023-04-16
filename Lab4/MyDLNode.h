#ifndef MYDLN_H
#define MYDLN_H
#include <cstddef>

template <class T>
class MyDLNode{
    private:

    public:
        T value;
        MyDLNode* next = NULL;
        MyDLNode* prev = NULL;
        MyDLNode(){
            value = NULL;
            next = NULL;
            prev = NULL;
        };
        MyDLNode(T in){
            value = in;
            next = NULL;
            prev = NULL;
        };
};

#endif