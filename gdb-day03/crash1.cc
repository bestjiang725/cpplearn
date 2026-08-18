#include <iostream>

void level3() {
    int* p = nullptr;
    *p = 42;
}

void level2() { level3(); }
void level1() { level2(); }

int main() {
    std::cout << "before crash" << std::endl;
    level1();
    std::cout << "after crash" << std::endl;
    return 0;
}
