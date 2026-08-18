#include <iostream>
#include <vector>

int fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int sum_vec(const std::vector<int>& v) {
    int total = 0;
    for (int x : v) total += x;
    return total;
}

int main() {
    std::vector<int> v;
    for (int i = 1; i <= 5; ++i)
        v.push_back(i * i);
    std::cout << "fib(5) = " << fib(5) << std::endl;
    std::cout << "sum = " << sum_vec(v) << std::endl;
    return 0;
}
