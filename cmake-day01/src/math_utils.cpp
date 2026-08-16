#include "mylib/math_utils.h"

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

long long fib(int n) {
    long long a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        long long t = a + b;
        a = b;
        b = t;
    }
    return a;
}
