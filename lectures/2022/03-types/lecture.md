---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Типы, шаблоны, lambda функции

Сергей Скворцов. Лекция 3

---

# Типы

- Ограничивают возможные операции над объектом
* Придают понятную семантику объекту
* Есть много встроенных типов: `bool`, `int`, `long`, `int32_t`, etc.
    ```cpp
    int16_t i = 123;
    uint64_t some_bigger_i = i;
    auto another_integer = 0;
    auto another_long_integer = 0l;
    ```

---

# Типы

Из типов можно собирать другие типы, комбинируя их

```cpp
struct TwoInts {
  int64_t hi;
  int64_t lo;
};
```

---

# Типы

- Можно добавлять методы.
- Такой тип несет какой-то инвариант: методы его сохраняют.

```cpp
class TwoInts {
public:
  TwoInts() : hi_(0), lo_(0) {}
  int64_t GetHigh() const { return hi_; }
  int64_t GetLow() const { return lo_; }
private:
  int64_t hi_;
  int64_t lo_;
};
```

---

# Типы

- Любой тип имеет ненулевой размер в байтах (char)
* Даже пустой:
    ```cpp
    struct Empty {};
    static_assert(sizeof(Empty) == 1);
    ```
* Потому что в C++ все объекты одного типа имеют разные адреса, нет zero sized types (привет, Rust)
* В стандарте есть лазейка, позволяющая хранить пустые типы без лишней памяти

---

# Типы. EBO

- Лазейка называется empty base optimization
* Компилятор может не добавлять пустые базы в layout типа, если это ничему не противоречит
    ```cpp
    struct IntWithEmpty : Empty {
        int value;
    };
    static_assert(sizeof(IntWithEmpty) == sizeof(int)); // Not guaranteed
    ```

---

# Типы. EBO

* При этом все равно нельзя иметь объекты одного типа с одинаковыми адресами:
    ```cpp
    struct TwoEmpty : Empty {
        Empty anotherEmpty;
    };
    static_assert(sizeof(TwoEmpty) == 2);

    struct Proxy1 : Empty {};
    struct Proxy2 : Empty {};
    struct TwoEmptyBases : Proxy1, Proxy2 {};
    static_assert(sizeof(TwoEmptyBases) == 2);
    ```

---

# Шаблоны

- Вы знакомы с шаблонами, это параметризованная типами / значениями сущность (класс, функция, переменная)

```cpp
template <class T1, class T2>
struct pair {
    T1 first; // Use of T1
    T2 second; // Use of T2
};

int x = 0;
pair<int, int> p(x, x);
pair<int, int&> p(x, x); // Because why not.

x = std::max<int32_t>(0, x);
```

---

# Шаблоны
- Бывают шаблоны переменных: редко используемая фича, но иногда полезно
```cpp
template <int A, int B>
constexpr int Sum = A + B;
```

---

# Шаблоны. NTTP

- Бывают значения в качестве шаблонных параметров (non-type template parameter):

```cpp
template <typename T, size_t Count>
class Array {
    T buf[Count];
};
```

---

# Шаблоны. NTTP

- С C++20 сильно расширили возможные типы NTTP:

```cpp
template <util::fixed_string regex>
class StaticRegularExpression;

StaticRegularExpression<"([0-9a-f]{8}-){3}([0-9a-f]{8})"> regex;
```

* `util::fixed_string` нужно реализовывать самим

---

# Шаблоны

- Шаблонные аргументы и типы могут выводиться

```cpp
template <typename T>
T inc(T a) { return a + 1; }

inc(5); // call inc<int>

int x = -1;
x = std::max(0, x); // call std::max<int>();

template <int I>
int foo(std::integral_constant<int, I> _unused) {
    return I * I;
}

void bar() {
    Foo(std::integral_constant<int, 5>{}); // Call Foo<5>
}
```

---

# Шаблоны

- Иногда не выводятся
    ```cpp
    uint32_t x = 0;
    x = std::max(0, x); // long compiler error.
    ```

* Почему?

* ```
    kek.cpp:5:9: error: no matching function for call to 'max'
        x = std::max(0, x); // long compiler error.
        ^~~~~~~~
    .../max.h:40:1: note: candidate template ignored:
        deduced conflicting types for parameter '_Tp' ('int' vs. 'uint32_t' (aka 'unsigned int'))
        max(const _Tp& __a, const _Tp& __b)
    ```

---

# Шаблоны

- В классах через конструкторы и deduction guides (since C++17)

```cpp
template <class T1, class T2>
struct pair {
  pair(const T1& f, const T2& s) : first(f), second(s) {}
  T1 first; // Use of T1
  T2 second; // Use of T2
};

pair p{0, 1}; // CE before C++17, pair<int, int> after C++17
```

---

# Шаблоны. CTAD

- Можно написать явный class template deduction guide, он сильнее:

```cpp
template <class T1, class T2>
struct pair {
  pair(const T1& f, const T2& s) : first(f), second(s) {}
  T1 first;
  T2 second;
};

template <typename T1, typename T2>
pair(const T1&, const T2&) -> pair<cosnt T1&, const T2&>;

pair p{0, 1}; // pair<const int&, const int&>
```

---

# Variadic templates

- Шаблоны бывают с переменным числом аргументов

```cpp
template <typename F, typename ...Args>
void ProxyCall(F&& func, Args&&... args) {
    func(std::forward<Args>(args)...);
}

template <typename ...Ts>
class tuple;

template <typename ...Ts>
class variant;
```

---

# Variadic templates

- С такими шаблонами тяжело работать

```cpp
template <typename ...Ts>
struct Tuple;

template <>
struct Tuple<> {};

template <typename T, size_t Index>
struct TupleElement {
    T value;
};

template <typename T, typename ...Rest>
struct Tuple<T, Rest...>
    : TupleElement<T, sizeof...(Rest)>
    , Tuple<Rest...>
{};
```

---

# Variadic templates. Fold expressions

- С такими шаблонами тяжело работать

```cpp
int Sum() {
    return 0;
}

template <typename T, typename ...Rest>
int Sum(T first, Rest... tail) {
    return first + Sum(tail...);
}

int ka = Sum(1, 10, 4, 4);
```

---

# Variadic templates. Fold expressions

- До C++17 были смешные хаки: у parameter pack было мало использований, но можно было инициализировать массив

```cpp
template <typename ...Ts>
int Sum(Ts... values) {
    int sum = 0;
    int arr[]{(sum += values)...};
    return sum;
}

int ka = Sum(1, 10, 4, 4);
```

---

# Variadic templates. Fold expressions

- В C++17 добавили _fold expressions_

```cpp
template <typename ...Ts>
int Sum(Ts... values) {
    return (0 + ... + values);
}

int ka = Sum(1, 9, 9, 0);
```

* Скобки обязательны, это часть синтаксиса. `return 0 + ... + values;` не работает.

* Есть разные виды fold expressions, бывают правые / левые, подробнее смотрите на cppreference.

---

# Типы. Traits, policies

- Иногда хочется понять что-то о типе

```cpp
template<class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value)
{
    ForwardIt it;
    // What is this?
    typename std::iterator_traits<ForwardIt>::difference_type count, step;
    count = std::distance(first, last);
    while (count > 0) {
      // ...
    }
    return first;
}
```

---

# Типы. Traits, policies

```cpp
typename std::iterator_traits<ForwardIt>::difference_type count;
//^^^^^  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//later  `iterator_traits has difference_type for the iterator`
```

---

# Типы. Traits, policies

```cpp
struct iterator_traits<T*> {
  using difference_type = ptrdiff_t;
  // std::remove_cv removes `const` and `volatile` from T
  using value_type = typename std::remove_cv<T>::type;
  using pointer = T*;
  using reference = T&;
  using iterator_category = random_access_iterator_tag;
};
```

```cpp
std::is_same_v<std::iterator_traits<const int*>::pointer, const int*>;
std::is_same_v<std::iterator_traits<int*>::reference, int&>;
```

---

# Типы. Traits, policies

- list iterator

```cpp
template <class T, class VoidPtr>
class list_iterator {
  using iterator_category = bidirectional_iterator_tag;
  using value_type = T;
  using reference = value_type&;
  // Later
  using pointer = typename rebind_pointer<VoidPtr, value_type>::type;
  // Another traits
  using difference_type = typename pointer_traits<pointer>::difference_type;
  // ...
};
```

---

# Типы. Traits, policies

* Как iterator_traits работают вообще?
* Мы же не хотим специализировать каждый указатель
* А для контейнеров возьмём что-то с их имплементацией
* Well, для этого существуют partial specializations

---

# Типы. Traits, policies

```cpp
template <class T, bool is_internal = std::is_pointer_v<T>>
struct iterator_traits;

template <class T>
struct iterator_traits<T, true> {
  // Use pointer implementation
};

// Just delegate.
template <class T>
struct iterator_traits<T, false> {
  using iterator_category = typename T::iterator_category;
  using value_type = typename T::value_type;
  using reference = typename T::reference;
  using pointer = typename T::pointer;
  using difference_type = typename T::difference_type;
};
```

---

# Типы. Traits, policies

```cpp
// Chooses iterator_traits<int*, true>
iterator_traits<int*> x1;
// Chooses iterator_traits<$ITERATOR_FROM_LIST, false>
iterator_traits<decltype(std::list<int>().begin())> x2;
// Prefferable to the above.
// Chooses iterator_traits<$ITERATOR_FROM_LIST, false>
iterator_traits<typename std::list<int>::iterator> x3;
```

---

# Типы. Traits, policies

- Как запретить людям явно писать `iterator_traits<int*, false>` ?
- В реальности всё чуть сложнее, но такого варианта уже хватит:

Вариант 2:

```cpp
// By default it is a delegation.
template <class T>
struct iterator_traits {
  using iterator_category = typename T::iterator_category;
  using value_type = typename T::value_type;
  using reference = typename T::reference;
  using pointer = typename T::pointer;
  using difference_type = typename T::difference_type;
};
```

---

# Типы. Traits, policies

```cpp
// For pointers use pointer implementation
template <class T>
struct iterator_traits<T*> {
  using difference_type = ptrdiff_t;
  using value_type = typename std::remove_cv<T>::type;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::random_access_iterator_tag;
};
```

---

# Частичные специализации

- Правила матчинга очень сложные. Не учите их.
- Как стоит делать и не запутаться:

```cpp
// Default implementation.
template <class T1, /* Your types ... */, bool x1, /* Your possibly empty number of flags */>
class spec {}; // {} is optional. You can just declare it.

template </*Fewer or same number of types*/>
class spec</*Your type*/, /*Your flags*/> {

};

template </*Fewer or same number of types*/>
class spec</*Your another type*/, /*Your flags*/> {

};
// ...
```

---

# Частичные специализации

- С ними можно выводить много хитрых штук

```cpp
template <typename Signature>
struct FunctionTraits;

template <typename Ret, typename ...Args>
struct FunctionTraits<Ret(Args...)> {
    using ReturnType = Ret;
    using Args = TypeList<Args...>;
};
```

---

# Типы. Traits policies

```cpp
template <class T> struct is_pointer {
  static constexpr const bool value = false;
};
template <class T> struct is_pointer<T*> {
  static constexpr const bool value = true;
};

template <class T, class U>
struct is_same {
  static constexpr const bool value = false;
};
template <class T>
struct is_same<T, T> {
  static constexpr const bool value = true;
};
```

---

# Типы. Traits policies

- Для функций не работает, кстати, только для классов :)

```cpp
template <class T> struct remove_const {
  using type = T;
};
template <class T> struct remove_const<const T> {
  using type = T;
};
```

---

# Типы. Traits, policies

- [`#include <type_traits>`](https://en.cppreference.com/w/cpp/header/type_traits)
- Есть всё, от `std::true_type`, `std::false_type` до `std::is_base_of`

Наиболее популярные

- `std::remove_cv{ref}`, `std::is_trivial`, `std::is_same`, `std::decay`
- У всех типовых трейтов есть `_t` суффикс, у булевых `_v`

```cpp
// Equivalent
std::decay_t<T> x;
typename std::decay<T>::type x;

// Equivalent
std::is_same_v<T1, T2>
std::is_same<T1, T2>::value
```

---

# Типы. Traits, policies

- Переменные в шаблонах могут быть только переменными вычисленными на этапе компиляции, то есть `constexpr`

```cpp
constexpr bool kTrue = true; // OK
const bool kTrue = true; // Not OK
```

- На этапе компиляции можно много что вычислить, об этом потом. Сейчас просто понимание, зачем писать `constexpr` на таких переменных

---

# Типы. Traits, policies

- `if constexpr` уменьшил так порядочно боли, но всё ещё больно

```cpp
template <class T>
void Foo(T integral) {
  // If true, everything else does not compile.
  if constexpr (std::is_same_v<T, int>) {
    //
  } else constexpr (std::is_same_v<T, float>) {
    //
  } else {
    //
  }
}
```

---

# Типы. Traits, policies

- Зачем нужен `typename` перед типами?

```cpp
template <class T>
struct Foo;
template <>
struct Foo<float> { static constexpr int cv = 0; }
template <>
struct Foo<int> { using cv = int; }
// Multiplication of Foo<T>::cv and b or the definition of pointer?
// General rule:
//   First try to understand the syntax of the templated code, then semantics
Foo<T>::cv * b;
// Type, no questions. By default assumed to be a variable.
typename Foo<T>::cv * b;
```

---

# Типы. Traits, policies

- Rebind pointer

```cpp
// Predefine a memory block
struct block;

// Define a pointer to a memory block from the kind of pointer Ptr s
// If Ptr is any kind of T*, block_ptr_t is block*
// If Ptr is smart_ptr<T>, block_ptr_t is smart_ptr<block>
using block_ptr_t = typename std::pointer_traits<Ptr>::template rebind<block>;
//                                                     ^^^^^^^^
//                                                     What the hell is that?
```

* Та же история: что, если rebind это не функция, а переменная? Компилятор подскажет, не парьтесь

---

# Типы. Concepts

* Огромные ошибки, сложности шаблонов и тд.

```cpp
template <typename Key, typename Value, typename Hash = std::hash<Key>, /*...*/>
class std_map {/*...*/};

Type l = {3,-1,10};
std_map<Type, int> m;
m[l] = 10;
//Typical compiler diagnostic without concepts:
//  250 lines of error
```

---

# Типы. Concepts

- Вы можете писать bool выражения, каким свойствам они удовлетворяют
- Если не компилируется, не удовлетворяет
- Если false, не удовлетворяет
- Ещё поговорим. С++20

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
auto with_uniq = [uniq{std::move(uniq)}] {
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
auto l = [bar = std::move(bar)]<typename T>(const T& x, const T& y) {
  return bar.size()
};
l<int>(1, 2);
```

---

# Lambda функции

* Неявно constexpr (since C++17)

```cpp
template <int T>
static constexpr int kNumPrimes = []() -> int {
  // Check all i from 2 to T if they are prime
}();
```

---

# Lambda функции

* Можно превращать в шаблонные аргументы, в том числе и классов.
* Самый простой пример: `std::sort`

```cpp
template <class Callback>
void DoWorkAndCallback(const std::vector<Data>& data, Callback&& cb);

DoWorkAndCallback(data, [&]() { /*...*/ });
```

---

# Lambda функции

* Даже шаблонные аргументы классов с помощью deduction guides (будет в дз).

```cpp
template <class T>
class Foo {
public:
  Foo(T&& t) : callback_(std::move(t)) {}

  void commit() {
      callback_();
  }

private:
  T callback_;
};

Foo foo{[] {
    return 42;
}};
// ...
foo.commit();
```
