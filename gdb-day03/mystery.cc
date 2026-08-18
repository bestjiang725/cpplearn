#include <iostream>
#include <vector>

int main() {
    std::vector<int> v = {3, 1, 4, 1, 5};

    int max = 0;
    int i = 0;
    while (i < (int)v.size()) {
        if (v[i] > max)
            max = v[i];
        // 这里少了点什么?
    }

    std::cout << "max = " << max << std::endl;
    return 0;
}
