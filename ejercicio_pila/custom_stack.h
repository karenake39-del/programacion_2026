#ifndef CUSTOM_STACK_H
#define CUSTOM_STACK_H

#include <vector>

class CustomStack {
private:
    std::vector<int> datos;


public:
    CustomStack();
    ~CustomStack();


    bool empty();

    int size();

    int top ();

    void push(int valor);

    void pop();

};

#endif // CUSTOM_STACK.H