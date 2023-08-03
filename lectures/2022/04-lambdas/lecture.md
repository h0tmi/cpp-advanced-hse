---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Типы, lambda функции, сборка кода

Сергей Скворцов. Лекция 4

---

# Типы. Traits, policies

* Мы научились в трейты (метафункции над типами), но как их удобно использовать?
* Несколько вариантов:
    1. Частичные специализации
    1. SFINAE (страшная штука, поговорим после)
    1. Tag dispatching
    1. If constexpr
    1. Концепты

---

# Типы. Tag dispatching

- Красивая идиома: используем перегрузки для специализации поведения

```cpp
template <typename Iterator>
Iterator LowerBound(T value, Iterator begin, Iterator end, std::random_access_iterator_tag);

template <typename Iterator>
Iterator LowerBound(T value, Iterator begin, Iterator end, std::forward_iterator_tag);

template <typename Iterator, typename IteratorCategory>
Iterator LowerBound(T value, Iterator begin, Iterator end, IteratorCategory category);

template <T value, typename Iterator>
Iterator LowerBound(T value, Iterator begin, Iterator end) {
    return LowerBound(value, begin, end, typename std::iterator_traits<Iterator>::iterator_category{});
}
```
---

# Типы. Tag dispatching

- Красивая идиома: используем перегрузки для специализации поведения

```cpp
template <typename T, typename U>
T Pow(T value, U power, std::true_type);

template <typename T, typename U>
T Pow(T value, U power, std::false_type);

template <typename T, typename U>
T Pow(T value, U power) {
    return PowImpl(value, power, std::is_floating_point_v<U>{});
}
```

* Минусы: много кода

---

# Типы. If constexpr

- Все еще больно, но не так сильно

```cpp
template <T value, typename Iterator>
Iterator LowerBound(T value, Iterator begin, Iterator end) {
    using IteratorCategory = typename std::iterator_traits<Iterator>::iterator_category;
    if constexpr (std::is_same_v<IteratorCategory, std::random_access_iterator_tag>) {
        return LowerBoundRandomAccess(value, begin, end);
    } else if constexpr (std::is_same_v<IteratorCategory, std::forward_iterator_tag>) {
        return LowerBoundForward(value, begin, end);
    } else {
        return LowerBoundDefault(value, begin, end);
    }
}
```

---

# Типы. If constexpr

- Как уронить компиляцию в `if constexpr`?

```cpp
template <T value, typename Iterator>
Iterator LowerBound(T value, Iterator begin, Iterator end) {
    using IteratorCategory = typename std::iterator_traits<Iterator>::iterator_category;
    if constexpr (std::is_same_v<IteratorCategory, std::random_access_iterator_tag>) {
        return LowerBoundRandomAccess(value, begin, end);
    } else if constexpr (std::is_same_v<IteratorCategory, std::forward_iterator_tag>) {
        return LowerBoundForward(value, begin, end);
    } else {
        static_assert(false, "Unsupported iterator tag"); // Unconditional compile error
    }
}
```

---

# Типы. If constexpr

- На помощь приходит зависимый контекст:

```cpp
template <typename T>
static constexpr bool kAlwaysFalse = false;

template <T value, typename Iterator>
Iterator LowerBound(T value, Iterator begin, Iterator end) {
    using IteratorCategory = typename std::iterator_traits<Iterator>::iterator_category;
    if constexpr (std::is_same_v<IteratorCategory, std::random_access_iterator_tag>) {
        return LowerBoundRandomAccess(value, begin, end);
    } else if constexpr (std::is_same_v<IteratorCategory, std::forward_iterator_tag>) {
        return LowerBoundForward(value, begin, end);
    } else {
        static_assert(kAlwaysFalse<T>, "Unsupported iterator tag"); // Unconditional compile error
    }
}
```

---

# Типы. Детектор
```cpp
template <class T, class AlwaysVoid>
struct HasKekImpl {
    static constexpr bool value = false;
};
 
template <typename T>
struct HasKekImpl<T, std::void_t<decltype(std::declval<T>().kek())>> {
    static constexpr bool value = true;
};

template <typename T>
using HasKek = HasKekImpl<T, void>;
```

---

# Типы. Concepts

- Огромные ошибки, сложности шаблонов и так далее

```cpp
template <typename Key, typename Value, typename Hash = std::hash<Key>, /*...*/>
class std_map {/*...*/};

Type l = {3, -1, 10};
std_map<Type, int> m;
m[l] = 10;
//Typical compiler diagnostic without concepts:
//  250 lines of error
```

---

# Типы. Concepts

- C++20 принес новый механизм: концепты

```cpp
// 5 lines of error.
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};
template <Hashable Key, typename Value>
class std_map {/*...*/};
```

---

# Типы. Concepts

- Новое ключевое слово `requires`:

```cpp
template <typename T>
bool HasFoo = requires(const T& value) {
    value.foo();
};
```

---

# Типы. Concepts

- Можно даже засунуть в constexpr if:

```cpp
template <typename T>
void Kek(T& value) {
    if constexpr (requires { value.foo() }) {
    }
}
```

---

# Типы. Concepts

- Концепты — constexpr bool выражения.

```cpp
template <typename T>
concept AlwaysFalse = false;

template <typename Derived, typename Base>
concept IsBaseOf = std::is_base_of_v<Base, Derived>;
```

---

# Типы. Concepts

- Можно ограничивать концептами любую шаблонную сущность

```cpp
template <typename T>
requires Hashable<T>
void Kek(T& value) {
    // can use std::hash<T>
}

template <typename T>
requires Hashable<T>
class HashSet;
```

---

# Типы. Concepts

- Есть два коротких синтаксиса для ограничений:

```cpp
template <Hashable T>
class HashSet;

template <Hashable T>
void Foo(T value);
```

- И более короткий, но страшный:
```cpp
void Foo(Hashable auto value);

void Bar() {
    Hashable auto value = 123;
}
```

---

# Lambda функции

- Если вы понимаете lambda функции, вы понимаете приличную часть C++
- Вы скорее всего их уже несколько раз использовали
- Lambda функции сделаны реально хорошо. Это признают все члены стандарта

```cpp
// Puzzle. What's going on?
(+[](){}())()
```

---

# Lambda функции

```cpp
[/*capture*/]</*optional template params*/>(/*params*/) /*specifiers*/ {
  /*body*/
}
```

---

# Lambda функции

- Каждая лямбда — отдельный тип
- Хитрые правила захвата

```cpp
int foo = 0;
std::vector<int> bar;
[&foo]() {} // By reference
[foo]() {} // By value
[bar = std::move(bar)]() {} // Declare **new** bar copy to bar
[&]() {}  // Capture everything by reference (discouraged)
[=]() {}  // Capture everything by value (you must know what you do)
```

---

# Lambda функции

- Как сделать лямбду, хранящую `std::unique_ptr` в захвате?

```cpp
std::unique_ptr<int> uniq = std::make_unique<int>(42);
auto with_uniq = [uniq = std::move(uniq)] {
};
```

- Как мувнуть uniq из лямбды?

```cpp
std::unique_ptr<int> uniq = std::make_unique<int>(42);
auto with_uniq = [uniq{std::move(uniq)}]() mutable {
    std::move(uniq); // std::unique_ptr<int>&&
};
```

---

# Lambda функции

- Лямбды без списка захвата можно превратить в указатель на функцию
- Можно использовать красивый хак: унарный плюс перед лямбдой явно превращает ее в функцию

```cpp
auto /* unique type */ pure_lambda = [](int a, int b) { return a + b; };
auto /* int (*)(int, int) */ fptr = +[](int a, int b) { return a + b; };
```

* Можно ли лямбды со списком захвата превратить в указатель на функцию?

---

# Lambda функции

- Лямбды с С++20 можно использовать в unevaluated контексте

```cpp
template <typename T, typename UniqueTag = decltype([](T&& ref) {})>
struct Holder;

std::map<int, decltype([](int a, int b) {
    return b < a;
})> mapka;
```

---

# Lambda функции

- Могут быть шаблонными

```cpp
// How can it be implemented?
auto auto_lambda = [](const auto& x, const auto& y) {};

// You can use typename instead of auto.
auto templated_lambda = [bar = std::move(bar)]<typename T>(const T& x, const T& y) {
  return bar.size()
};
templated_lambda(1, 2);
```

---

# Lambda функции

- Неявно constexpr (since C++17)

```cpp
template <int T>
static constexpr int kNumPrimes = []() -> int {
  // Check all i from 2 to T if they are prime
}();
```

---

# Lambda функции

- Можно превращать в шаблонные аргументы, в том числе и классов.
- Самый простой пример: `std::sort`

```cpp
template <class Callback>
void DoWorkAndCallback(const std::vector<Data>& data, Callback&& cb);

DoWorkAndCallback(data, [&]() { /*...*/ });
```

---

# Lambda функции

- Даже шаблонные аргументы классов с помощью deduction guides (будет в дз).

```cpp
template <class T>
class Foo {
public:
  Foo(T&& t) : callback_(std::move(t)) {}

private:
  T callback_;
};
```

---

# Как передавать функторы в функции?

1. Указатель на функцию
```cpp
void foo(void (*callback)(int));
```
2. Указатель на функцию с контекстом
```cpp
void foo(void (*callback)(void*, int), void* ctx);
```
3. Шаблонный параметр (нужна ли универсальная ссылка?)
```cpp
template <typename F>
void foo(F&& callback);
```

---

# Как передавать функторы в функции?
4. Шаблонный параметр с концептом
```cpp
void foo(Fn<void(int)> auto&& callback);
```
5. `std::function`:
```cpp
void foo(const std::function<void(int)>& fn);
```
6. `nonstd::function_ref`:
```cpp
void foo(std::function_ref<void(int)> fn);
```
