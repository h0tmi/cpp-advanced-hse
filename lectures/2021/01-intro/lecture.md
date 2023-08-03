---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Intro. Move семантика, perfect forwarding, lifetime

Данила Кутенин. Лекция 1

---

# Курс

* 15 лекций и семинаров
* 3 больших ДЗ, 15 маленьких + Бонус
* Маленькие ДЗ будут выкладываться каждую неделю. От 3 задач
* $O_{\textrm{итог}} = \mathrm{Round}(0.6 \cdot O_{\textrm{большие ДЗ}} + 0.4 \cdot O_{\textrm{маленькие ДЗ}} + \textrm{ Бонус})$

---

# Курс. Дедлайны

* Маленькие задачи
  * Открываются каждую неделю после лекции в 13:00. **Сегодня тоже**
  * Дедлайн через 13 дней в пятницу в 19:00
  * Штрафы после дедлайна вычисляются по формуле $\max\left(\frac{Y}{e^{X/5}}, 0.3Y\right)$, где $Y$ &mdash; количество баллов за задачу, $X$ &mdash; количество дней после дедлайна (может быть дробным)
    * После 1 часа 99.17%, после 2 часов 98.3%, после 5 часов 95.92%, после полусуток 90.5%, после суток 81.87%, после \~6 &mdash; 30% до конца курса.
* Большие задачи
  * Модульная задача с отдельным дедлайном (обычно 2-3 недели)
  * После дедлайна 0 баллов без продления

---

# Курс. Организация

* https://t.me/cpp_advanced_hse_2021
* Чат прилинкован к каналу, закреп &mdash; источник правды в любой момент
* В чат можно кидать ссылки на свои решения, никто их не увидит кроме нас и вас
* Репозиторий https://gitlab.com/danlark/cpp-advanced-hse
* Инструкция в SETUP\.md этого репозитория
* https://cpp-hse.org/ сайт с дедлайнами и вашими задачами
* После лекции мы выдадим код регистрации

---

# Курс. Организация

* Любое списывание хотя бы одной задачи &mdash; заявление в УО
* Любое умышленное приведение в нестабильность системы &mdash; заявление в УО
* Любое неследование условиям решение &mdash; обнуление всей недели
  * Иногда можно написать какие-то костыли, которые не удовлетворяют текстовому условию
  * Выборочно будем проверять

---

# Курс. Задачи

* Маленькие задачи
  * Написать один-два файла, закрепить материал
  * Не разбираются на семинарах
* Большие задачи
  * Модульные на 500-2000 строк вместе с тестированием, сложные концепты, дотошность в деталях, код ревью по желанию
  * Разбираются на семинарах
* Бонусные задачи
  * Любой развлекательный материал, креативные и сложные задачи
  * Не разбираются вообще
* Тесты почти всегда открытые. Проверяются CI gitlab (не Яндекс.Контест)

---

# Курс. Цели

* После основ C++, хочется объяснить, что стоит за концептами, какое место сейчас у C++ в мире
* Дать много задач, чтобы набилась рука и не было боязно идти на стажировки
* Курс за основу взять ШАДовский с поправкой на знание первого курса C++. Перезачесть ШАДовский курс в будущем будет нельзя
* C++ всё ещё используется в тьме продуктов. 9 из 11 Google миллиардников на C++, большинство Яндекса на C++, геймдев на C++ и т.д.
* Будут рубрики "Факап на миллион", где мы обсудим баги, которые стоили много денег компаниям

---

# Лектор

* Работаю в Google Data Pipelining Efficiency a.k.a MapReduce
* Был разок в комитете по C++, много пишу каждый день на C++
* Закончил ФКН, был в ШАД, в инфраструктуре Яндекс.Поиска
* Мои основные увлечения это перформанс приложений, распределённые системы, поисковые движки, АКОС, C++, Rust, алгоритмы

---

# move семантика

lvalues, rvalues

```cpp
// lvalue is a location value (or left hand side value)
// rvalue is a right hand side value
int i = 1;   // i is an lvalue, 1 is rvalue (literal constant)
int* y = &i; // we can take addresses from lvalue as they have location
1 = i;       // 1 is an rvalue, cannot be on the left
y = &666;    // 666 is rvalue, no location
```

---

# move семантика

lvalues, rvalues

```cpp
int SetValue() {
  return 42;
}

int& SetGlobal() {
  static int j;
  return j;
}

SetValue() = 1;  // SetValue is not an lvalue, it is temporary returned from function
SetGlobal() = 5; // ok, int& is lvalue reference, it has location

// lvalue reference
int& a_ref = a;
++a_ref;
```

---

# move семантика

```cpp
// On the right side we have a temporary thing, an rvalue that needs to be
// stored somewhere in an lvalue.
// On the left side we have the reference (an lvalue) that should point to an
// existing object. But being 10 a numeric constant, i.e. without a specific
// memory address, i.e. an rvalue, the expression clashes with the very spirit
// of the reference.
int& b_ref = 10; // not ok
```

---

# move семантика

```cpp
void foo(int& x)
{
}

foo(42);  // Nope!
// This works instead:
// int x = 42;
// foo(x);
// Because int& can modify things! 42 is not modifiable by itself
```

---

# move семантика

```cpp
void foo_const(const int& x)
{
}
foo_const(10); // Yes!
```

---

# move семантика

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
```

---

# move семантика

Pre C++11

```cpp
void move_string(std::string& s, std::string& f) {
  s.swap(f);
  f.clear();
}
```

---

# move семантика

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

# move семантика

У C++-11 появились rvalue references для этого.

---

# move семантика. Зачем?

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

# move семантика. Зачем?

```cpp
Holder h1(10000); // regular constructor
Holder h2 = h1;   // copy constructor
Holder h3(h1);    // copy constructor (alternate syntax)
```

---

# move семантика

Копирующий конструктор

```cpp
Holder(const Holder& other) {
  data_ = new int[other.size_];  // (1)
  std::copy(other.data_, other.data_ + other.size_, data_);  // (2)
  size_ = other.size_;
}
```

---

# move семантика

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

# move семантика

Что произойдёт здесь?

```cpp
Holder CreateHolder(int size) {
  return Holder(size);
}
```

---

# move семантика

Сколько аллокаций?

```cpp
int main() {
  Holder h(10);
  h = CreateHolder(1000);
  return 0;
}
```

Ответ: минимум 3

---

# move семантика

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

# move семантика

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

# move семантика

Посвапать. Так тоже можно и деструктор сделает своё дело, но правило, что лучше не надо. Объекта после не существует.

```cpp
Holder& operator=(Holder&& other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  return *this;
}
```

---

# move семантика

Можно превращать lvalue в rvalues с помощью std::move.

```cpp
int main() {
  Holder h1(1000);           // h1 is an lvalue
  Holder h2(std::move(h1));  // move-constructor invoked (because of rvalue in input)
}
```

---

# move семантика

Иногда компилятор по умолчанию генерирует. Он генерирует, если нет никакого копируещего метода, или move метода, или деструктора.

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

# move семантика

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

# move семантика

Хорошее правило, либо вы указываете все операторы (rule of 5), либо ничего (rule of 0).

1. Копирущий конструктор
1. Копирущий оператор присваивания
1. Перемещающий конструктор
1. Перемещающий оператор присваивания
1. Деструктор

Там, где можно, ставьте `= default;`, `= delete`. Подавляющее большинство случаев.

---

# move семантика

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

# move семантика

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

# move семантика

![image](https://cdn.nextptr.com/images/uimages/B2Al-gbVpUhaENYKt8Nw3Upx.png)

---

# move семантика

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

# move семантика

Конструкторы могут принимать по значению, а потом делать move. Так "модно".

```cpp
class Annotation {
public:
  explicit Annotation(std::string text)
   : value(std::move(text)) {}
private:
  std::string value;
};
```

---

# move семантика

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

# Зачем?

Перенапривить аргументы в базовый класс, [abseil](https://github.com/abseil/abseil-cpp/blob/master/absl/status/statusor.h#L354).

```cpp
template <typename U>
StatusOr(U&& v) : Base(std::forward<U>(v)) {}
```

---

# Зачем?

Работать с функциональщиной ([пример из реального мира](https://github.com/abseil/abseil-cpp/blob/1ae9b71c474628d60eb251a3f62967fe64151bb2/absl/container/internal/raw_hash_set_allocator_test.cc)).

Аккуратненько сохранили все аргументы так, как задумывал вызывающий:

```cpp
template <class F>
static auto apply(F&& f, int32_t v) -> decltype(std::forward<F>(f)(v, v)) {
  return std::forward<F>(f)(v, v);
}
```

---

# Шаблоны и rvalue

Без шаблона не скомпилируется, хотя тип вывести можно. Почему? На семинарах и по пути курса

```cpp
template<class T>
void Fwd(T&& v) { Call(std::forward(v)); }
```

---

# Применения

* На `std::unique_ptr` можно только делать move, это гарантирует никаких копий и double free, один объект, один его разрущает. RAII (Resource Acquisition Is Initialization) идиома.
* `std::string, std::vector` можно тоже перемещать между друг другом, то же RAII
* На семинарах больше
* Меньше ошибок с ресурсами
* Меньше выстрелов в ногу как управлять памятью
* Полное избавление от new/delete в коде и работе с сырыми указателями.

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

Это гарантирует ещё больше, что вообще нет никогда никаких двух различных регионов памяти. По архитектуре не бывает double free. И вы не можете два объекта мультипоточными сделать на этапе компиляции.

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

У Rust

* Все типы movable (у C++ есть не movable типы, а также до C++11 код)
* Любое присваивание, вызов функции это move (у C++ сложная логика ссылок)
* Нельзя пользоваться объектом после move (в C++ иногда можно, иногда это UB, иногда это unspecified)
* Не вызываются деструкторы у объектов после move (у C++ вызываются, поэтому стоит деинициализировать)

---

# vs Go/Java/Python

* Все присваивания не мувают объекты
* Перед каждым объектом есть reference counter
* Когда он ноль, язык может удалить объект
* Это называется Garbage Collection
* C++ думал сделать gargabe collection, но из-за деструкторов и Бьярне оно не получило распространения.