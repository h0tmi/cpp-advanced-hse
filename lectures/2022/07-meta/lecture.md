---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Метапрограмирование

Сергей Скворцов. Лекция 7

---

# Метапрограмирование
Метапрограмма порождает другую программу как результат своей работы
* Макросы
* Хитрости с шаблонами
* constexpr-выражения (since C++11)
* Метаклассы (С++26 / ...)

---

# Макросы
- Простая текстовая замена
```cpp
#define MAX_SIZE 100

int array[MAX_SIZE];
```

- Функциональные макросы
```cpp
#define sqr(x) ((x) * (x))

sqr(2 + 3) -> ((2 + 3) * (2 + 3))
sqr(heavy_func()) -> ((heavy_func) * (heavy_func()))
```

---

# Макросы
- Условия на этапе компиляции
```cpp
#if defined(__unix__)
#include <unistd.h>
#elif defined(_WIN32)
#include <Windows.h>
#else
#error "Unsupported platform"
#endif
```

---

# Макросы
- Проверки
```cpp
#include <cassert>

assert(x > 5);
```

- Как устроен assert?
```cpp
#ifdef NDEBUG
#define	assert(e)	((void)0)
#else
#define assert(e)  \
    ((void) ((e) ? ((void)0) : __assert (#e, __ASSERT_FILE_NAME, __LINE__)))
#define __assert(e, file, line) \
    ((void)printf ("%s:%d: failed assertion `%s'\n", file, line, e), abort())
#endif
```

---

# Макросы
- Проверки
```cpp
Y_ENSURE(x > 5); // throws on error
Y_ENSURE(x > 5, "Lol kek");
Y_VERIFY(x > 5); // kills your program
Y_VERIFY(x > 5, "Error message");
```

https://github.com/ydb-platform/ydb/blob/main/util/generic/yexception.h#L228
```cpp
#define Y_ENSURE(...) Y_PASS_VA_ARGS(Y_MACRO_IMPL_DISPATCHER_2(__VA_ARGS__, Y_ENSURE_IMPL_2, Y_ENSURE_IMPL_1)(__VA_ARGS__))
```

---

# Макросы
- Тестирование
```cpp
#define ASSERT_EQ(actual, expected) \
    do { \
        auto actual_copy = actual; \
        auto expected_copy = expected; \
        if (!(actual_copy == expected_copy)) { \
            std::cerr << __FILE__ << ':' << __LINE__ << ": Assertion failed\n"; \
            std::cerr << "\texpected: " << expected_copy << " (= " << #expected << ")\n"; \
            std::cerr << "\tactual: " << actual_copy << " (= " << #actual << ")\n"; \
            throw AssertionError{}; \
        } \
    } while (false)
```

---

# Макросы
+ Какой-то код для нескольких разных похожих сущностей
```cpp
#define FOR_EACH_ENUM_VALUE(XX) \
    XX(First) \
    XX(Second) \
    XX(Third) \

std::string ToString(SomeEnum x) {
#define XX(value) \
    case value: \
        return #value;

    switch (x) {
    FOR_EACH_ENUM_VALUE(XX)
    }

#undef XX
}
```

---

# Макросы
```cpp
#define Y_EMPTY_REF_QUALIFIER
#define Y_FOR_EACH_REF_QUALIFIERS_COMBINATION(XX) \
    XX(Y_EMPTY_REF_QUALIFIER)                     \
    XX(&)                                         \
    XX(&&)                                        \
    XX(const)                                     \
    XX(const&)                                    \
    XX(const&&)

#define Y_DECLARE_REMOVE_CLASS_IMPL(qualifiers)             \
    template <typename C, typename R, typename... Args>     \
    struct TRemoveClassImpl<R (C::*)(Args...) qualifiers> { \
        typedef R TSignature(Args...);                      \
    };

    Y_FOR_EACH_REF_QUALIFIERS_COMBINATION(Y_DECLARE_REMOVE_CLASS_IMPL)
#undef Y_DECLARE_REMOVE_CLASS_IMPL

// https://github.com/ydb-platform/ydb/blob/main/util/generic/function.h#L14-L30
```

---

# Макросы
+ Перегрузки по числу аргументов:
```cpp
#define MACRO_IMPL_DISPATCHER_2(_0, _1, IMPL, ...) IMPL

#define ENSURE1(cond) if (!cond) { throw std::runtime_error{"Condition " #cond " failed"}; }
#define ENSURE2(cond, msg) if (!cond) { throw std::runtime_error{std::string{"Condition " #cond " failed: "} + msg}; }

#define ENSURE(...) MACRO_IMPL_DISPATCHER_2(__VA_ARGS__, ENSURE2, ENSURE1)(__VA_ARGS__)
```

---

# Макросы
На самом деле, это достаточно мощный язык, можно делать перегрузки, писать очень много странных вещей.
```cpp
Y_MATCH(res) {
    Y_CASE(const TResponse& res) {
        ...
    },
    Y_CASE(const TError& err) {
        ...
    },
};
```

https://github.com/Hirrolot/metalang99
https://github.com/Hirrolot/datatype99

---

# Шаблоны
Рекурсивное инстанциировние
```cpp
template <int N>
struct Factorial {
    static constexpr int value = Factorial<N - 1>::value * N;
};

template <>
struct Factorial<0> {
    static constexpr int value = 1;
};

std::cout << Factorial<8>::value << endl; // mov esi, 40320
```

---

# Шаблоны
If / else
```cpp
template <bool cond, class T, class F>
struct If;

struct If<false, T, F> {
    static constexpr int value = F::value;
};

template <>
struct If<true, T, F> {
    static constexpr int value = T::value;
};
```

---

# Шаблоны. SFINAE
```cpp
template <class T>
class Array {
public:
    explicit Array(size_t n, const T& value = T{}) { ... }

    template <class Iterator>
    Array(Iterator begin, Iterator end) { ... }

private:
    T* data_;
};

Array<int> ar(5, 3);
```

---

# Шаблоны. SFINAE
Substitution failure is not an error. Если при попытке подставить тип в шаблон функции при инстанциировании получается невалидный заголовок функции, то ошибки компиляции не происходит, функция просто отбрасывается из overload set.

```cpp
template <
    class Iterator,
    class Dummy = std::enable_if_t<!std::is_arithmetic_v<Iterator>>
>
Array(Iterator begin, Iterator end) { ... }
```

---

# Шаблоны. SFINAE
```cpp
template <typename T> struct is_arithmetic : std::false_type {};
template <> struct is_arithmetic<int> : std::true_type {};
template <> struct is_arithmetic<double> : std::true_type {};
template <> struct is_arithmetic<char> : std::true_type {};
// etc
```

---

# Шаблоны. SFINAE
```cpp
template <bool Cond, typename T = void>
struct EnableIf;

template <typename T>
struct EnableIf<true, T> {
    using type = T;
};

template <typename T>
struct EnableIf<false, T> {
};

template <bool Cond, typename T = void>
using EnableIfT = EnableIf<Cond, T>::type;
```

---

# Шаблоны. SFINAE
- Все живет в `<type_traits>`
- `is_class, is_enum, is_function, is_rvalue_reference, ...`
- `is_abstract, is_trivially_destructible, is_trivially_copyable, is_nothrow_move_constructible, ...`
- `remove_const, add_lvalue_reference, add_rvalue_reference, decay, ...`

---

# Шаблоны. SFINAE
```cpp
template <class T, class AlwaysVoid>
struct HasKekImpl : std::false_type {};

template <typename T>
struct HasKekImpl<T, std::void_t<decltype(std::declval<T>().kek())>> : std::true_type {}

template <typename T>
using HasKek = HasKekImpl<T, void>;

template <typename T, typename = std::enable_if_t<HasKek<T>::value>>
void WithKek();

template <typename T, typename = !std::enable_if_t<HasKek<T>::value>>
void WithKek();
```

---

# Шаблоны. SFINAE
Куда совать `enable_if_t`?
* В шаблонный аргумент.
    ```cpp
    template <typename T, typename = std::enable_if_t<HasKek<T>::value>>
    void WithKek();

    // NTTP
    template <typename T, std::enable_if_t<HasKek<T>::value>* = nullptr>
    void WithKek();
    ```

---

# Шаблоны. SFINAE
Куда совать `enable_if_t`?
- В возвращаемое значение.
    ```cpp
    template <typename T>
    std::enable_if_t<HasKek<T>::value, T> WithKek();
    ```
* В аргументы функции.
    ```cpp
    template <typename T>
    T WithKek(std::enable_if_t<HasKek<T>::value>* = nullptr);
    ```

---

# Шаблоны. SFINAE
SFINAE сложно работает на методах шаблона класса:
```cpp
template <typename T>
class UniquePtr {
public:
    template <typename = std::enable_if_t<!std::is_same_v<T, void>>>
    T& Unref(); // CE
};

template <typename T>
class UniquePtr {
public:
    template <typename U = T, typename = std::enable_if_t<!std::is_same_v<U, void>>>
    U& Unref(); // OK
};
```

---

# Constexpr. C++11
Можно писать функции, вычисляемые на этапе компиляции.
```cpp
constexpr int Fact(int n) {
    return n == 0 ? 1 : n * Fact(n - 1);
}

// Compile time
static_assert(Fact(8) == 40320);
std::array<int, Fact(5)> arr;

// Runtime
int n;
std::cin >> n;
std::cout << Fact(n);
```

---

# Constexpr. C++11
Ограничения:
+ Тело функции состоит из одного return
+ В нем можно делать довольно ограниченное множество операций: операции над константами и аргументами, вызовы constexpr функций, некоторые операторы (sizeof, throw (!))

---

# Constexpr. C++11
```cpp
constexpr void CheckIfZeroOrLess(int n) {
    return (void)(n <= 0 ? 1 : throw 5);
}

// Ill-formed, no diagnostics required
constexpr void FailUnconditionally() {
    throw 5;
}
```

---

# Constexpr. C++11
Constexpr переменные инициализируются гарантированно на этапе компиляции.

```cpp
constexpr int Double(int a) {
    return a + a;
}

int GetRuntimeValue();

constexpr int Two = Double(1);
constexpr int Kek = Double(GetRuntimeValue());
int Nekek = Double(GetRuntimeValue());
```

---

# Constexpr. C++11
+ Можно использовать в классах:
```cpp
class ConstString {
    const char* p;
    size_t size;

public;
    template <size_t N>
    constexpr ConstString(const char (&a)[N]): p(a), sz(N - 1) {}

    constexpr char operator[](size_t n) const {
        return n < sz ? p[n] : throw std::out_of_range{""};
    }

    constexpr size_t size() const { return sz; }
};

constexpr ConstString a("hello");
```

+ constexpr-методы по-умолчанию const

---

# Constexpr. C++14
+ В функции запрещены goto, try, объявление типов. Все остальное примерно можно.
```cpp
constexpr int Fact(int n) {
    int res = 1;
    for (int i = 1; i <= n; ++i) {
        res *= i;
    }
    return res;
}
```
+ constexpr у метода не означает, что метод const.
+ Много constexpr в std.

---

# Constexpr. C++17
+ Еще больше std
+ Можно создавать лямбды в constexpr
+ Constexpr if
+ Constexpr everywhere

---

# Constexpr. C++17
```cpp
constexpr auto Add(int lhs) {
    return [=](int rhs) {
        return lhs + rhs;
    };
}

int main() {
    constexpr auto inc = Add(1);
    static_assert(inc(5), 6);
    static_assert(inc(7), 8);
}
```

---

# Constexpr. C++17
```cpp
template <typename Iterator>
void FastSort(Iterator begin, Iterator end) {
    using T = typename std::iterator_traits<Iterator>::value_type;
    if constexpr (std::is_integral_v<T>) {
        RadixSort(begin, end);
    } else {
        QuickSort(begin, end);
    }
}
```

---

# Constexpr. C++20
+ Compile-time аллокаторы. `std::vector`, `std::string`.
+ `consteval`.

---

# Constexpr. C++20. cNTTP
Поддержаны сложные NTTP с сильными ограничениями:
```cpp
#include <array>
#include <string_view>

template <size_t N>
struct FixedString {
    std::array<char, N> buf;

    constexpr FixedString(const char (&str)[N]) {
        for (size_t i = 0; i < N; ++i) {
            buf[i] = str[i];
        }
    }

    std::string_view constexpr AsView() const {
        return std::string_view{buf.data(), buf.size()};
    }
};

template <size_t N>
FixedString(const char(&str)[N]) -> FixedString<N>;
```

---

# Constexpr. C++20. cNTTP

```cpp
#include <iostream>

template <FixedString str>
void Foo() {
    std::cout << str.AsView();
}

int main() {
    Foo<"abacaba">();
}
```
