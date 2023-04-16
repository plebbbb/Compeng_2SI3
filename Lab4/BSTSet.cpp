#include "BSTSet.h"
#include "MyStack.h"
#include "MyQueue.h"
#include <list>
#include <iostream>
#include <vector>

using namespace std;

BSTSet::BSTSet()
{
	root = NULL;
}

BSTSet::BSTSet(const std::vector<int>& input)
{
    for (int i : input){
        TNode* current = root;
        if (root == NULL){
            root = new TNode(i, NULL, NULL);
            continue;
        }
        while(true){
            TNode** selected;
            if (current->element > i){
                selected = &(current->left);
            } else if (current->element < i){
                selected = &(current->right);
            } else break;

            if (*selected == NULL){
                *selected = new TNode(i, NULL, NULL);
                break;
            } else current = *selected;
        }
    }
}

BSTSet::~BSTSet()
{
    MyStack<TNode*> stack;
    TNode* cur = root;
    while (cur != NULL || stack.size != 0){
        while (cur != NULL){
            stack.push(cur);
            cur = cur->left;
        }
        cur = stack.pop();
        TNode* tmp = cur;
        cur = cur->right;
        delete tmp;
    }
}

bool BSTSet::isIn(int v)
{
    TNode* selected = root;
	while (selected != NULL){
        if (selected->element == v) return true;
        if (selected->element < v) selected = selected->right;
        else selected = selected->left;
    }
    return false; 
}

void BSTSet::add(int v)
{
	TNode** selected = &root;
	while (*selected != NULL){
        if ((*selected)->element == v) return;
        if ((*selected)->element < v) selected = &((*selected)->right);
        else selected = &((*selected)->left);
    }
    *selected = new TNode(v, NULL, NULL);
}

bool BSTSet::remove(int v)
{    
    if (root == NULL) return false;
    TNode** selected = &root;
	while (*selected != NULL){
        if ((*selected)->element == v) break;
        if ((*selected)->element < v) selected = &((*selected)->right);
        else selected = &((*selected)->left);
    }
    if ((*selected) == NULL) return false; 

    TNode* L = (*selected)->left;
    TNode* R = (*selected)->right;

    if (L && R) {
        //find rightest left node
        TNode** iter = &L;
        while ((*iter)->right != NULL) iter = &((*iter)->right);


        //find leftest node of rightest left node for grafting
        TNode** swp = iter;
        while (*swp != NULL) swp = &((*swp)->left);
        *swp = ((L == *iter) ? *swp : L); //graft old left node on end
        //cout << "SW" << (*swp)->element;

        //replace
        delete *selected;
        *selected = *iter;
        *iter = NULL;
        (*selected)->right = R;
    } else {
        delete *selected;
        *selected = ((L == NULL) ? R : L);
    }

    return true;
}

void BSTSet::Union(const BSTSet& s)
{
    MyStack<TNode*> stack;
    TNode* cur = s.root;
    while (cur != NULL || stack.size != 0){
        while (cur != NULL){
            stack.push(cur);
            cur = cur->left;
        }
        cur = stack.pop();
        add(cur->element);
        cur = cur->right;
    }
}

void BSTSet::intersection(const BSTSet& s)
{
    BSTSet tmp;
    MyStack<TNode*> stack;
    TNode* cur = s.root;
    while (cur != NULL || stack.size != 0){
        while (cur != NULL){
            stack.push(cur);
            cur = cur->left;
        }
        cur = stack.pop();
        if (isIn(cur->element)) tmp.add(cur->element);
        cur = cur->right;
    }


    cur = root;
    while (cur != NULL || stack.size != 0){
        while (cur != NULL){
            stack.push(cur);
            cur = cur->left;
        }
        cur = stack.pop();
        if (!tmp.isIn(cur->element)) remove(cur->element);
        cur = cur->right;
    }
}

void BSTSet::difference(const BSTSet& s)
{
    MyStack<TNode*> stack;
    TNode* cur = s.root;
    while (cur != NULL || stack.size != 0){
        while (cur != NULL){
            stack.push(cur);
            cur = cur->left;
        }
        cur = stack.pop();
        if (isIn(cur->element)) remove(cur->element);
        cur = cur->right;
    }

}

int BSTSet::size()
{
    int count = 0;
    MyStack<TNode*> stack;
    TNode* cur = root;
    while (cur != NULL || stack.size != 0){
        while (cur != NULL){
            stack.push(cur);
            cur = cur->left;
        }
        cur = stack.pop();
        count++;
        cur = cur->right;
    }    
    return count; //change this after completing this function
}

int BSTSet::height()
{
    printBSTSet();
    if (root == NULL) return -1;

    MyQueue<TNode*> pm;
    pm.push(root);

    TNode* fr = NULL;
    int depth = 0;

    while (pm.size != 0){
        int size = pm.size;
        cout << size << " ";
        while (size--){
            fr = pm.pop();
            if(fr->left) pm.push(fr->left);
            if(fr->right) pm.push(fr->right);
        }
        depth++;
        cout << depth << "\n";

    }
    return depth-1;
}
// create and use class MyStack
void BSTSet::printNonRec()
{
    MyStack<TNode*> stack;
    TNode* cur = root;
    while (cur != NULL || stack.size != 0){
        while (cur != NULL){
            stack.push(cur);
            cur = cur->left;
        }
        cur = stack.pop();
        std::cout << cur->element << ",";
        cur = cur->right;

    }

}

// Do not modify following provided methods

void BSTSet::printBSTSet()
{
    if (root == NULL)
        std::cout << "";
    else {
        printBSTSet(root);
    }
}

void BSTSet::printBSTSet(TNode* t)
{
    if (t != NULL) {
        printBSTSet(t->left);
        std::cout << t->element << ",";
        printBSTSet(t->right);
    }
}
