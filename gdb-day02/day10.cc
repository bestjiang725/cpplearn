#include <iostream>

struct Victim {
    int arr[4];
    int secret;
};

int main() {
    Victim v = {{10, 20, 30, 40}, 42};

    for (int i = 0; i < 5; ++i)   // BUG: i < 5, arr[4] 越界
        v.arr[i] = i * 100;

    std::cout << "secret = " << v.secret << std::endl;
    return 0;
}
