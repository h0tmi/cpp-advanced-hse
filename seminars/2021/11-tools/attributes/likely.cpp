long long fact(long long n) {
    if (n > 1)
        [[likely]] return n * fact(n - 1);
    else
        [[unlikely]] return 1;
}

// more interesting: https://godbolt.org/z/W4efGWzcc