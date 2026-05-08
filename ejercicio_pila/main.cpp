#include "custom_stack.h"
#include <iostream>

int main () {
    CustomStack mipila;
    mipila.push(1);
    std::cout << "mipila() = " << mipila.top() << std::endl;
    return 0;
}
   