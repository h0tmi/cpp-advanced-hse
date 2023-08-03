---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Ссылки. Move семантика.

Сергей Скворцов. Лекция 2

---

# Категория значения

* Каждое выражение в C++ характеризуется типом и категорией значения (value category).
* Всего есть три различных категории значений:
  1. lvalue (left hand side value)
  2. xvalue (eXpiring value)
  3. prvalue (pure rvalue)
* xvalue и prvalue похожи, для них определена категория rvalue (right hand side value).
* Интуиция: исторически назвали так, потому что lvalue может быть на левой стороне оператора присваивания, а rvalue — нет.

---

# Категория значения
* Формальные правила для определения категории значения сложные. Можно почитать [cppreference](https://en.cppreference.com/w/cpp/language/value_category)
* Посмотрим на примерах.

---

# lvalues, rvalues

```cpp
int i = 1;                      // i is an lvalue, 1 is rvalue (literal constant)
int* y = &i;                    // we can take addresses from lvalue as they have location
1 = i;                          // ERROR: 1 is an rvalue, cannot be on the left
y = &42;                        // ERROR: 42 is rvalue, no location
const char(*ptr)[5] = &"abcd";  // string literal is an lvalue (!)
std::cout << "Hello, " << name; // std::ostream& operator<<(std::ostream&, const char*);

char arr[20];
arr[10] = 'z';                  // arr[10] returns lvalue reference

int a, b;
(a, b) = 5;                     // same as "b = 5";

const char* p = ...;
*p++ = 'a';
```

---

# lvalues, rvalues

```cpp
int GetValue() {
  return 42;
}

int& GetGlobal() {
  static int j;
  return j;
}

GetValue() = 1;  // GetValue is not an lvalue, it is temporary returned from function
GetGlobal() = 5; // ok, int& is lvalue reference, it has location
```

---

# lvalues

```cpp
int a = 42;
int& a_ref = a;
++a_ref;
a_ref = 5;

// On the right side we have a temporary thing, an rvalue that needs to be
// stored somewhere in an lvalue.
// On the left side we have the reference (an lvalue) that should point to an
// existing object. But being 10 a numeric constant, i.e. without a specific
// memory address, i.e. an rvalue, the expression clashes with the very spirit
// of the reference.
int& b_ref = 10; // not ok
```

---

# lvalues

```cpp
void foo(int& x) {
}

foo(42); // Nope!
// This works instead:
// int x = 42;
// foo(x);
// Because int& can modify things! 42 is not modifiable by itself
```

---

# lvalues

```cpp
void foo_const(const int& x) {
}

foo_const(10); // Yes!
```

---

# Temporary lifetime extension

```cpp
// Why?
// Well, it is a mistake and not at the same time.

// the following...
const int& ref = 10;

// ... would translate to:
int __internal_unique_name = 10;
const int& ref = __internal_unique_name;

// Without this rule we wouldn't do the things like:
class T; // defined somewhere
T f();
void g(const T &x);

g(f());
const T& x = f(); // also works
```

---

# Move семантика

Before C++11:

```cpp
void move_string(std::string& s, std::string& f) {
  s.swap(f);
  f.clear();
}
```

---

# Move семантика

Before C++11:

```cpp
std::vector<T, Allocator>::swap(std::vector& other);
std::map<K, V, Compare, Allocator>::swap(std::map& other);
std::basic_string<CharT, Traits, Allocator>::swap(std::basic_string& other);
```

---

# Move семантика

[Example in KALDI](https://github.com/kaldi-asr/kaldi/blob/master/src/decoder/lattice-simple-decoder.cc#L510
):

```cpp
void LatticeSimpleDecoder::ProcessEmitting(DecodableInterface *decodable) {
  int32 frame = active_toks_.size() - 1; // frame is the frame-index
                                         // (zero-based) used to get likelihoods
                                         // from the decodable object.
  active_toks_.resize(active_toks_.size() + 1);
  prev_toks_.clear();
  cur_toks_.swap(prev_toks_); // Pre C++11, swaps pointers!!!!!!!!!!!

  // Processes emitting arcs for one frame.  Propagates from
  // prev_toks_ to cur_toks_.
  BaseFloat cutoff = std::numeric_limits<BaseFloat>::infinity();
  for (unordered_map<StateId, Token*>::iterator iter = prev_toks_.begin();
       iter != prev_toks_.end();
       ++iter) {
    StateId state = iter->first;
    Token *tok = iter->second;
  // ...
```

---

# Move семантика

До C++11 был `std::auto_ptr`. Владение передавалось при копировании:
```cpp
template <class T>
struct auto_ptr {
    auto_ptr(T* ptr) : ptr_{ptr} {}

    auto_ptr(auto_ptr& other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    ~auto_ptr() {
        delete ptr_;
    }

private:
    T* ptr_;
}
```

---

# Move семантика

В C++11 появились rvalue references для этого. Это самая важная фича C++11.

---

# Move семантика. Зачем?

Следить за динамическими ресурсами.

```cpp
class Holder {
  public:
    Holder(int size) {
      data_ = new int[size];
      size_ = size;
    }

    ~Holder() {
      delete[] data_;
    }
  private:
    int*   data_;
    size_t size_;
};
```

---

# Move семантика. Зачем?

```cpp
Holder h1(10000); // regular constructor
Holder h2 = h1;   // copy constructor
Holder h3(h1);    // copy constructor (alternate syntax)
```

---

# Move семантика

Копирующий конструктор

```cpp
Holder(const Holder& other) {
  data_ = new int[other.size_];  // (1)
  std::copy(other.data_, other.data_ + other.size_, data_);  // (2)
  size_ = other.size_;
}
```

---

# Move семантика

Копирующий оператор

```cpp
Holder& operator=(const Holder& other) {
  if (this == &other) return *this;  // (1)
  delete[] data_;  // (2)
  data_ = new int[other.size_];
  std::copy(other.data_, other.data_ + other.size_, data_);
  size_ = other.size_;
  return *this;  // (3)
}
```

---

# Move семантика

Что произойдёт здесь?

```cpp
Holder CreateHolder(int size) {
  return Holder(size);
}
```

---

# Move семантика

Сколько аллокаций?

```cpp
int main() {
  Holder h(10);
  h = CreateHolder(1000);
  return 0;
}
```

* Ответ: минимум 3

---

# Move семантика

Почему мы не можем просто украсть CreateHolder и ничего не присваивать?

На помощь приходят другие ссылки, rvalue ссылки.

```cpp
Holder(Holder&& other) {
  data_ = other.data_;  // (1)
  size_ = other.size_;
  other.data_ = nullptr;  // (2)
  other.size_ = 0;
}
```

---

# Move семантика

Удалить левый объект, присвоить себе метаданные правого.

```cpp
Holder& operator=(Holder&& other) {
  if (this == &other) return *this;

  delete[] data_;  // (1)

  data_ = other.data_;  // (2)
  size_ = other.size_;

  other.data_ = nullptr;  // (3)
  other.size_ = 0;

  return *this;
}
```

---

# Move семантика

Можно посвопать. Деструктор объекта, на который ссылается other, сделает своё дело, но правило, что лучше не надо.

```cpp
Holder& operator=(Holder&& other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  return *this;
}
```

---

# Move семантика

Почему плохо свопать?
```
{
    std::vector<char> buf1 = AllocateBuffer(10_GB);
    std::vector<char> buf2 = AllocateBuffer(10_GB);
    ...
    buf1 = std::move(buf2);
    DoTheRealCalculations(buf1);
    // buf2 is dropped only here
}
```

---

# Move семантика

Можно превращать lvalue в rvalues с помощью std::move.

```cpp
int main() {
  Holder h1(1000);           // h1 is an lvalue
  Holder h2(std::move(h1));  // move-constructor invoked (because of rvalue in input)
}
```

---

# Move семантика

Иногда компилятор по умолчанию генерирует конструкторы / операторы присваивания. Правила, когда комплиятор может это сделать, сложные (если нет никакого копирующего метода, или move метода, или деструктора).

```cpp
class Foo {
public:
  Foo() {} // OK, no restrictions on default ctor
private:
  std::string s_;
};
```

```cpp
class Foo {
public:
  Foo() {}  // OK, no restrictions on default ctor
  ~Foo() {} // NO, user declared
private:
  std::string s_;
};
```

---

# Move семантика

```cpp
class Foo {
public:
  Foo() {}
  Foo(Foo&) = delete; // OK
  Foo& operator=(const Foo&) { return *this; } // NO, user declared!
  ~Foo() = default; // OK
private:
  std::string s_;
};
```

---

# Move семантика

Хорошее правило: либо вы указываете все конструкторы/операторы (rule of 5), либо ничего (rule of 0).

1. Копирующий конструктор
1. Копирующий оператор присваивания
1. Перемещающий конструктор
1. Перемещающий оператор присваивания
1. Деструктор

Там, где можно, ставьте `= default`, `= delete`. Подавляющее большинство случаев.

---

# Move семантика. Temporary lifetime extension

Для rvalue-ссылок ожидаемо работает temporary lifetime extension
```cpp
// parameter is rvalue reference
void fn(X &&) { std::cout << "X &&\n"; }

int main() {
  X a;
  fn(a); // compiler error, argument is an lvalue

  fn(X()); // works, argument is an rvalues
}
```

---

# Move семантика

```cpp

template<typename T> struct remove_reference { typedef T type; };
template<typename T> struct remove_reference<T&> { typedef T type; };
template<typename T> struct remove_reference<T&&> { typedef T type; };

template <class T>
inline typename remove_reference<T>::type&&
move(T&& t) {
  using U = typename remove_reference<T>::type;
  return static_cast<U&&>(t);
}
```

---

# Move семантика

![image](https://cdn.nextptr.com/images/uimages/B2Al-gbVpUhaENYKt8Nw3Upx.png)

---

# Move семантика

Никогда не делайте move на const объекты. const от типа не может отлипнуть.

```cpp
class Annotation {
public:
  explicit Annotation(const std::string text)
    : value(std::move(text)) {} // we want to call string(string&&),
                                // text is const,
                                // std::move(text) is const std::string&&
                                // we called string(const std::string&)
                                // it is a perf issue.
private:
  std::string value;
};
```

---

# Move семантика

Конструкторы могут принимать по значению, а потом делать move. Так "модно".

```cpp
class Annotation {
public:
    explicit Annotation(std::string text)
        : value_(std::move(text)) {}
private:
    std::string value_;
};
```

---

# Move семантика

Классическая ловушка:

```cpp
class Annotation {
public:
    explicit Annotation(std::string&& text)
        : value_(text) {} // Call std::string::string(const std::string&); !
private:
    std::string value_;
};
```

* Почему?
* Выражение `text` имеет тип `std::string` и категорию значения `lvalue`. Она именована, у нее можно взять адрес. Поэтому мы должны еще раз позвать `std::move`, чтоб превратить lvalue в rvalue (xvalue, на самом деле).

---

# Move семантика

Классическая ловушка:

```cpp
class Annotation {
public:
    explicit Annotation(std::string&& text)
        : value_(std::move(text)) {}
private:
    std::string value_;
};
```

---

# Move семантика

Что делает `std::move` на примитивных типах?

```cpp
int x = 5;
int y = std::move(x); // == int y = x;

std::string_view s1 = "abc";
std::string_view s2 = std::move(s1);
```

---

# Move семантика

```cpp
void f1(std::string& s);
void f2(const std::string& s);
void f3(std::string&& s);
void f4(const std::string&& s);

std::string s("Hi"); //lvalue
const std::string cs("Hi"); //const lvalue

f1(s); // OK
f1(cs); // ERROR
f1(std::move(s)); // ERROR
f1(std::move(cs)); // ERROR

f2(s); // OK
f2(cs); // OK
f2(std::move(s)); // OK
f2(std::move(cs)); // OK

f3(s); // ERROR
f3(cs); // ERROR
f3(std::move(s)); // OK
f3(std::move(cs)); // ERROR

f4(s); // ERROR
f4(cs); // ERROR
f4(std::move(s)); // OK
f4(std::move(cs)); // OK
```

---

# Шаблоны и rvalue

Шаблонные rvalue references означают немного другое.

```cpp
template<typename T>
void foo(T&&); // forwarding reference here
// T is a template parameter for foo

template<typename T>
void bar(std::vector<T>&&); // but not here
// std::vector<T> is not a template parameter,
// only T is a template parameter for bar
```

---

# Шаблоны и rvalue

```cpp
template<typename T>
void foo(T&&); // forwarding reference here

// Beacause we want to write:
foo(42);
int x = 42;
foo(x); // Not always foo(std::move(x));
```

```
T&  + &   -> T&  // Lvalues are persistent
T&  + &&  -> T&  // Lvalues are persistent
T&& + &   -> T&  // Preserving what users want without copy
T&& + &&  -> T&& // Preserving what users want without copy
```

---

# Шаблоны и rvalue

Чтобы сохранить переданное пользователем lvalue или rvalue, есть понятие perfect forwarding.

```cpp
template<class T>
void Fwd(T&& v) { Call(std::forward<T>(v)); }
```

---

# Perfect forwarding. Зачем?

Проксировать вызов какой-то функции

```cpp
template <typename Args...>
void TimeIt(F func, Args&& ...args) {
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args));
    auto end = std::chrono::high_resolution_clock::now();
    return end - start;
}
```

---

# Perfect forwarding. Зачем?

Передать аргументы как есть в конструктор:

```cpp
template <typename Args...>
void emplace(Args&& ...args) {
    new (&storage) T{std::forward<Args>(args)...};
}
```

```cpp
std::vector<std::string> strs;
strs.push_back(std::string('a', 10)); // Create temporary std::string object, then move / copy
strs.emplace_back('a', 10); // Create std::string object inplace
```

---

# Perfect forwarding. Зачем?

Перенаправить аргументы в базовый класс: [abseil](https://github.com/abseil/abseil-cpp/blob/master/absl/status/statusor.h#L354).

```cpp
template <typename U>
StatusOr(U&& v) : Base(std::forward<U>(v)) {}
```

---

# Шаблоны и rvalue

Без шаблона не скомпилируется, хотя тип вывести можно. Почему? На семинарах и по пути курса

```cpp
template<class T>
void Fwd(T&& v) { Call(std::forward(v)); }
```

---

# Ref qualifiers

Вы уже писали перегрузки методов по константности:
```cpp
class Foo {
public:
    int& Get() {
        return value_;
    }

    const int& Get() const {
        return value_;
    }
};
```

Можно ли перегружать не только по константности, но и по rvalue / lvalue?

---

# Ref qualifiers

Можно.
```cpp
struct Foo {
    int& Get() & { return value_; }

    const int& Get() const& { return value_; }

    int&& Get() && { return std::move(value_); }

    const int&& Get() const&& { return std::move(value_); }
};
```

---

# Ref qualifiers

Как позвать?
```cpp
Foo{}.Get(); // call Get() &&

Foo foo;
foo.Get(); // call Get() &
std::as_const(foo).Get(); // call Get() const&
std::move(foo).Get(); // call Get() &&
```

---

# Ref qualifiers

Можно считать, что все методы принимают неявный первый параметр `self`.
В C++23 его можно сделать явным.
`self`, как и остальные параметры, бывает lvalue/rvalue ссылкой, а так же константным / не константным.
```cpp
struct Foo {
    int& Get() & { return value_; }

    const int& Get() const& { return value_; }

    int&& Get() && { return std::move(value_); }

    const int&& Get() const&& { return std::move(value_); }
};
```

---

# Move семантика. Применения

* На `std::unique_ptr` можно только делать move, это гарантирует никаких копий и double free: всегда один объект владеет ресурсом. RAII (Resource Acquisition Is Initialization) идиома.
* `std::string, std::vector` можно тоже перемещать между друг другом, то же RAII
* Меньше ошибок с ресурсами
* Меньше выстрелов в ногу как управлять памятью
* Полное избавление от new/delete в коде и работе с сырыми указателями.
* На семинарах больше

---

# vs Rust

В Rust любой оператор `=` это move.

```rust
struct A;

fn test() {
    let a = A;
    let b = a;
    let c = a; // compile error, a is moved
}
```

---

# vs Rust

Это гарантирует ещё больше, что вообще нет никогда никаких двух различных регионов памяти. По архитектуре не бывает double free.

```rust
struct A;

fn move_to(a: A) {
    // a is moved into here, you own it now.
}

fn test() {
    let a = A;
    move_to(a);
    let c = a; // compile error, a is moved
}
```

---

# vs Rust

В Rust

* Правильный дефолт: move by default, explicit copy (`x.clone()`). В C++ наоборот.
* Move всегда тривиальный: просто memcpy. В C++ бывает сложная логика в move ctor / assignemnt.
* Подавляющее большинство типов movable (у C++ есть не movable типы, а также до C++11 код)
* Любое присваивание, вызов функции это move (у C++ сложная логика ссылок)
* Нельзя пользоваться объектом после move (в C++ иногда можно, иногда это UB, иногда это unspecified)
* Не вызываются деструкторы у объектов после move (у C++ вызываются, поэтому стоит деинициализировать)

---

# vs Go/Java/Python

* Все присваивания не мувают объекты
* Перед каждым объектом есть reference counter
* Когда он ноль, язык может удалить объект
* Это называется garbage collection
* В C++ думали сделать gargabe collection, но оно не получило распространения. В C++11 добавили какие-то заготовки для GC, в C++20 их убрали.
