---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Немного про типы и lambda функции

Данила Кутенин. Лекция 3

---

# Типы

* Мы привыкли к типам в C++
* Типы созданы для избежания ошибок на масштабе
* Есть много дефолтных типов `bool`, `int`, `long`, `int32_t`, etc.

```cpp
int16_t i = 123;
uint64_t some_bigger_i = i;
auto another_integer = 0;
auto another_long_integer = 0l;
```

---

# Типы

* Далее типы можно комбинировать в структуры, больше C style, но всё ещё разрешено с C++

```cpp
struct TwoInts {
  int64_t hi;
  int64_t lo;
};
```

---

# Типы

* В которые мы можем добавлять дефолтные операции работы. Такое мы уже называем классами

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

# Типы. Шаблоны

* Вы знакомы с шаблонами, это абстракция над типами

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

* Вы можете пользоваться шаблоном в любом месте класса/функции
* И молиться, что оно скомпилируется

---

# Типы. Шаблоны

* Шаблонные аргументы и типы могут выводиться
* В функциях, а потом и в классах через конструкторы и deduction guides

```cpp
int x = -1;
x = std::max(0, x);

template <class T1, class T2>
struct pair {
  pair(const T1& f, const T2& s) : first(f), second(s) {}
  T1 first; // Use of T1
  T2 second; // Use of T2
};

pair p{0, 1};
```

---

# Типы. Шаблоны

* Иногда не выводятся

```cpp
int32_t x = 0;
x = std::max(0, x); // long compiler error.
```

* Почему?

---

# Типы. Traits, policies

* Иногда хочется понять что-то о типе

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

* list iterator

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

Вариант 1:

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

* Как запретить людям явно писать `iterator_traits<int*, false>` ?
* В реальности всё чуть сложнее, но такого варианта уже хватит:

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

# Типы. Traits policies

* Правила матчинга очень сложные. Не учите их.
* Как стоит делать и не запутаться:

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

* Для функций не работает, кстати, только для классов :)

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

* [`#include <type_traits>`](https://en.cppreference.com/w/cpp/header/type_traits)
* Есть всё, от `std::true_type`, `std::false_type` до `std::is_base_of`

Наиболее популярные

* `std::remove_cv{ref}`, `std::is_trivial`, `std::is_same`, `std::decay`
* У всех типовых трейтов есть `_t` суффикс, у булевых `_v`

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

* Переменные в шаблонах могут быть только переменными вычисленными на этапе компиляции, то есть `constexpr`

```cpp
constexpr bool kTrue = true; // OK
const bool kTrue = true; // Not OK
```

* На этапе компиляции можно много что вычислить, об этом потом. Сейчас просто понимание, зачем писать `constexpr` на таких переменных


---

# Типы. Traits, policies

* `if constexpr` уменьшил так порядочно боли, но всё ещё больно

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

* Зачем нужен `typename` перед типами?

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

* Rebind pointer

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

* Та же история, что если rebind это не функция, а переменная. Компилятор подскажет, не парьтесь

---

# Типы. Traits, policies, рубрика факап на миллион

```cpp
template<typename Key, /*...*/>
class flat_hash_set : public raw_hash_set<Key, /*value_type=*/Key> {}
template<typename Key, typename Value, /*...*/>
class flat_hash_map : public raw_hash_set<Key, /*value_type=*/std::pair<const Key, Value>> {}
class raw_hash_set::iterator {
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename raw_hash_set::value_type;
  using reference = value_type&;
  using pointer = std::remove_reference_t<reference>*;
  using difference_type = typename raw_hash_set::difference_type;
};
```

---

# Типы. Traits, policies, рубрика факап на миллион

* 2 days of debugging

```cpp
flat_hash_set<int> x;
x.insert(42);
(*x.find(42))++;
assert(x.find(43) == x.end()); // Sad :(
```

---

# Типы. Traits, policies, рубрика факап на миллион

```cpp
template<typename Key, /*...*/>
class flat_hash_set : public raw_hash_set<Key, /*value_type=*/Key> {}
template<typename Key, typename Value, /*...*/>
class flat_hash_map : public raw_hash_set<Key, /*value_type=*/std::pair<const Key, Value>> {}
class raw_hash_set::iterator {
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename raw_hash_set::value_type;
  using reference = std::conditional_t<PolicyTraits::constant_iterators::value,
                                  const value_type&, value_type&>;
  using pointer = std::remove_reference_t<reference>*;
  using difference_type = typename raw_hash_set::difference_type;
};
```

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

* Вы можете писать bool выражения, каким свойствам они удовлетворяют
* Если не компилируется, не удовлетворяет
* Если false, не удовлетворяет
* Ещё поговорим. С++20

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

* Если вы понимаете lambda функции, вы понимаете приличную часть C++
* Вы скорее всего их уже несколько раз использовали
* Lambda функции сделаны реально хорошо. Это признают все члены стандарта

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

* Lambda это отдельные типы
* lvalues, rvalues, by value
* variable initialization, конструкторы
* Plan: go to c++ insights

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

* templates, deduction guides
* Plan: go to c++ insights

```cpp
// How can it be implemented?
auto auto_lambda = [](const auto& x, const auto& y) {};

// You can use concept instead of typename.
auto l = [bar = std::move(bar)]<typename T>(const T& x, const T& y) {
  return bar.size()
};
l<int>(1, 2);
```

---

# Lambda функции

* constexpr, trailing return types
* Это кодген
* Plan: go to c++ insights

```cpp
template <int T>
static constexpr int kNumPrimes = []() -> int {
  // Check all i from 2 to T if they are prime
}();
```

---

# Lambda функции

* Это можно превращать в шаблонные аргументы, в том числе и классов.
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
private:
  T callback_;
};
// You will learn how to write yourself.
```

