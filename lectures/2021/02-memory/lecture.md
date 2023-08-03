---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Память

Данила Кутенин. Лекция 2

---

# Память

* В C++ есть несколько видов памяти
* Их разделять поможет лучше вам понимать как объекты манипулируются
* Например, как работает move семантика
* Память -- самая главная абстракция C++
* Когда вы пишете код, вы постоянно должны понимать откуда и как память идёт

---

# Виды памяти

* Полученные через `operator new` (куча, heap), например, `std::vector`, `std::string`, etc.
* Память на "стеке" (stack) `int x[10]`, или `std::array<int, 10>`, или просто `int x;`.
* Глобальная память, например, глобальные или статические объекты
* Битовые поля структур (хотя это не совсем корректно)
* Thread-local storage (мимо нашей лекции сегодня)

---

# Виды памяти. `operator new`

Это самый универсальный способ получить объект

```cpp
std::unique_ptr<int> ptr(new int());

// Calls new int[100]();
std::vector<int> v(100);

std::unordered_map<int, int> x;
// calls new std::pair<const int, int>(1, 2);
x[1] = 2;
```

---

# Виды памяти. `operator new`

* За каждым new должен идти один корректный delete
* `std::vector`, `std::unique_ptr`, другие контейнеры это делают в деструкторе
* Нет `new`, нет `delete`

```cpp
int* x = new int();
delete x;

int* arr = new int[100];
delete[] arr;
// delete arr; // ERROR!
```

---

# Виды памяти. `operator new`. Эпоха динозавров

* В С это похоже на malloc
* Отличия:
  * new может кинуть исключение при недостатке памяти, malloc вернёт nullptr
  * new имеет различные спецификаторы как конструкторы
  * new типизированный
  * То, что malloc это функция, а new -- оператор и поэтому быстрее -- ужасные коментарии плохих гайдов по C++

---

# Виды памяти. `operator new`. Эпоха динозавров

[DOOM-3](https://github.com/id-Software/DOOM-3-BFG/blob/1caba1979589971b5ed44e315d9ead30b278d8b4/neo/framework/Session.cpp)

```cpp
void idSessionLocal::Init() {

  common->Printf( "-------- Initializing Session --------\n" );

  InitSteam();
  ConstructSteamObjects();

  signInManager = new idSignInManagerWin();
  achievementSystem = new idAchievementSystemWin();
  achievementSystem->Init();
  Initialize();

  common->Printf( "session initialized\n" );
  common->Printf( "--------------------------------------\n" );
}
```

---

# Виды памяти. `operator new`. Эпоха динозавров

```cpp
void idSessionLocal::Shutdown() {

  delete signInManager;

  if ( achievementSystem != NULL ) {
    achievementSystem->Shutdown();
    delete achievementSystem;
  }

  DestroySteamObjects();
}
```

---

# Виды памяти. `operator new`

```cpp
int * x = new int[100];
if (!DoSomework(a, b, c, d)) {
  return; // WRONG!
}
delete[] x;
```

```cpp
void f() {
  int* p = new int(7);
  g();      // may throw
  delete p; // okay if no exception
} // memory leak if g() throws
```

---

# Виды памяти. `operator new`

* Лучшая фича C++ -- закрывающаяся скобка

```cpp
} // Destructors are called, new better be paired with delete
```

---

# Виды памяти. `operator new`

* Не пользуйтесь `operator new`, никогда.
* Используйте `std::unique_ptr` и `std::make_unique`, контейнеры
* Если есть `new`, это плохой тон, так плохо писать код и скорее всего будут ошибки
  * Проверяйте свой код Address Sanitizerом

---

# Виды памяти. `operator new`

Исключения

* Placement new, см. семинары
* Но лучше и это не использовать

---

# Виды памяти. `operator new`

Вы можете переопределять `operator new` локально и глобально.

```cpp
void* operator new(std::size_t sz) {/* ... */}
void operator delete(void* ptr) noexcept {/* ... */}
```

---

# Виды памяти. `operator new`

```cpp
struct X {
  static void* operator new(std::size_t sz) {
    std::cout << "custom new for size " << sz << '\n';
    return ::operator new(sz);
  }
  static void* operator new[](std::size_t sz) {
    std::cout << "custom new[] for size " << sz << '\n';
    return ::operator new(sz);
  }
};
int main() {
  X* p1 = new X;
  delete p1;
  X* p2 = new X[10];
  delete[] p2;
}
```

---

# Виды памяти. `operator new`

Операторы можно ещё с аргументами делать.

```cpp
struct X {
  X() { throw std::runtime_error(""); }
  static void* operator new(std::size_t sz, bool b) {
    std::cout << "custom placement new called, b = " << b << '\n';
    return ::operator new(sz);
  }
  static void operator delete(void* ptr, bool b) {
    std::cout << "custom placement delete called, b = " << b << '\n';
    ::operator delete(ptr);
  }
};
int main() {
  try {
    X* p1 = new (true) X;
  } catch(std::exception&) { }
}
```

---

# Виды памяти. Аллокаторы

* Большинство контейнеров имеют допольнительный шаблонный аргумент.

```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class vector;
```

* Дефолтный `std::allocator` это обёртка над `new`.

---

# Виды памяти. `std::allocator<T>`

```cpp
T* allocate(size_t n) {
  if (n > allocator_traits<allocator>::max_size(*this))
      throw std::length_error("allocator<T>::allocate(size_t n)"
                              " 'n' exceeds maximum supported size");
  return static_cast<T*>(::operator new(n * sizeof(T)));
}

void deallocate(T* p, size_t n) noexcept {
  ::operator delete(p);
}
```

---

# Виды памяти. Аллокаторы

* Если вы видите шаблонный аргумент аллокатора, под капотом new и этот объект хранит в себе динамическую память
* Если нет, значит деструкторы не удаляют память (пример, `std::array`)
* В С++17 добавили `polymorphic_allocator`, будет на семинарах

---

# Виды памяти. Аллокаторы

* Как они хранятся?

```cpp
template <class T, class A = std::allocator<T>>
class vector {
private:
  T* start_;
  size_t size_;
  size_t capacity_;
  A allocator_; // Extra one byte in every single container???????????????????
};
```

* Ответ на семинаре :)

---

# Виды памяти. Аллокаторы

* Под капотом аллокаторы сложны, это целая наука
* Paging, cache locality, thread local stack caches, arenas. Это всё про АКОС
* Но стоит знать, что new не всегда возвращает память из кучи, просто `heap` vs `stack` хорошее разделение
* Аллокаторы не всегда дешёвые, много мелких аллокаций хорошо могут так замедлить вашу систему

---

# Виды памяти. Правила

* Если нужны аллокации, аллоцируйте как можно больше одним куском памяти
* Не переусердствуйте, но и не давайте программе аллоцировать/деаллоцировать очень много (сотни тысяч в секунду)
* Непрерывные куски памяти всегда быстрее работают
  * Так устроена память, кэши в процессорах

---

# Виды памяти. Кейс

```cpp
std::vector<int> x;
for (size_t i = 0; i < n; ++i) {
  x.push_back(f(g(i)));
}
```

---

# Виды памяти. Кейс

```cpp
std::vector<int> x;
x.reserve(n); // Important, removes log(n) allocations
for (size_t i = 0; i < n; ++i) {
  x.push_back(f(g(i)));
}
```

---

# Виды памяти. Stack

* Большинство объектов, которые вы создаёте, лежат на "стеке"

```cpp
int x;
int array[10];
std::array<int, 10> std_array;
std::string string_array[10];

int* y = &x; // location of x, alternatively, you can use std::addressof(x)
// std::addressof(x) is universal for types which do overload `operator&`
```

* Стек это "преаллоцированная" память, часто ограниченная несколькими килобайтами -- парой мегабайт

---

# Виды памяти. Stack

* Из-за своей ограниченности легко "проломать" стек неконстантными аргументами.

```cpp
void foo(size_t n) {
  int array[n]; // Error prone, should not be used but allowed.
  // std::array<int, n> std_array; // does not compile, n is not constant.
  // ...
}
```

---

# Виды памяти. Stack. Правила

* Стек это "обычная" инициализация переменной/C-style массивов
* Все аргументы массивов при аллокации на стеке должны быть константными
* Все переменные можно адресовать оператором `&` или `std::addressof`
* Минимальная адресная единица -- 1 байт, все типы имеют хотя бы 1 байт. В C есть пустые типы и они вызывали проблемы
* Померить сколько займёт на стеке можно через `sizeof`.
* Большинство современных систем имеют указатели в 8 байт (подробнее на АКОС)

```cpp
sizeof(int)
sizeof(int[10])
sizeof(std::vector<int>)
```

---

# Виды памяти. Указатели

* Указатели обычно указывают на память :shrug:
* Указатель под капотом это число с указанием региона памяти

```cpp
int x = 0;
int* y = &x;
*y = 5;
assert(x == 5);
int another = 42;
int** z = &y;
**z = 420;
*z = &another;
**z = 69;
assert(x == 420);
assert(another == 69);
```

---

# Виды памяти. Указатели

* Чтобы прочитать по этому региону, есть операция разыменовывания с помощью `*`
* Для простоты можно считать, что указатели можно получить с помощью адресации стека или аллокации heap

---

# Виды памяти. Указатели

* Указатели стека не должны быть никак удалены
* Указатели кучи должны быть удалены
* Указатели бывают нулевыми

```cpp
std::unique_ptr<int> ptr; // nullptr under the hood.
std::unique_ptr<int> ptr(nullptr); // equivalent.
int* raw_ptr = nullptr;
```

* Разыменовывание нулевого указателя это всегда ошибка

```cpp
// Good taste.
if (ptr) {
  *ptr = 10;
}
```

---

# Виды памяти. Указатели

```cpp
int y = 10;
const int* x = &y; // Adding const is ok
*x = 42; // compiler error
```

```cpp
int y = 10;
int* const x = &y; // Adding const is ok
*x = 42; // OK, x is constant
x = &y; // ERROR!
```

---

# Виды памяти. Указатели

```cpp
T* ptr;              // Just pointer
const T* ptr;        // const pointer, data cannot be changed
T* const ptr;        // const pointer variable, pointer cannot be changed
T const * const ptr; // Both, east const notation
const T * const ptr; // Both, west const notation, equivalent
```

* Правило, берите звёздочку, разделяйте по ней, всё что слева -- про данные, всё, что справа про саму переменную
* Общее правило -- const относится к первому типу слева, а если его нет, то справа
* Иногда это не просто

```cpp
const T* const ** const * ptr = nullptr;
```

---

# Виды памяти. Указатели

* Зачем вообще нужны указатели?
* Чтобы передавать управление другим сущностям
* Некоторые стайлгайды (Google, Yandex, Bloomberg) предпочитают такой код.

```cpp
int value = 0;
ComputeSomething(a, b, c, &value); // a, b, c -- input, value -- output
```

---

# Виды памяти. Указатели

```cpp
// Same but with a reference
void ComputeSomething(int a, int b, int c, int& value) {
}

ComputeSomething(a, b, c, value); // What is output?
```

---

# Виды памяти. Ссылки vs Указатели

* Ссылки под капотом это указатели
* Ссылка не может быть нулевой, указатель может
* Ссылки умеют лучше работать с rvalue ссылками, например
* Нельзя брать ссылки на ссылки
* В целом отличий больше нет
* Всё больше стайлгайдов говорят, что указатели никогда ничего не хранят (то есть они не из кучи или кто-то другой за них ответственный, то есть не надо на них звать `delete`). Подразумевайте это по умолчанию, иначе ошибётесь и запутайтесь
* Стайлгайды не сходятся в вопросе ссылок против указателей. Я за ссылки, но практика показывает с ссылками чуть легче ошибиться

---

# Виды памяти. Умные указатели

* `std::unique_ptr` хранит в себе указатель и разрушает его корректно, не боится исключений
* `std::shared_ptr` разделяет хранение и удалет только тогда, когда нет. Для этого есть counter
* Больше на семинарах и в первом дз

---

# Виды памяти. View объектов

* `std::string` хранит в себе память, но передавать всегда `const std::string&` это надо положить указатель на стек и передать
* `std::string_view` это view объекта, он не ответственный за память, хранит два указателя на начало и конец
* $O(1)$ все операции типа подстрок, удаление начала и так далее, но нельзя модифицировать

```cpp
// Why do we need strings? string_view is enough in most cases.
std::vector<std::string_view> v;
{
  std::string to_split = SomeData();
  v = std::views::split(to_split, ',');
  // v is ok
}
// use of v is not ok, to_split is destroyed
```

---

# Виды памяти. View объектов

* Могут быть compile time константами

```cpp
constexpr std::string_view kFormatString = "Welcome to %s!\n";
```

---

# Виды памяти. View объектов

* Надо доказывать, что переживают вызовы

```cpp
int main() {
  std::string s = "Hellooooooooooooooo ";
  std::string_view sv = s + "World\n"; // Temporary and assigns to it.
  std::cout << sv; // Undefined behavior
}
```

---

# Виды памяти. View объектов

* View -- легкие объекты, обязательно должны переживать своих создателей
* Передавать по значению (на семинарах расскажут детали и сложности)
* Удобно манипулировать и не ошибиться сюрпризам линейной сложности

---

# Виды памяти. Аналогичные примеры

* `std::function` -> `absl::function_ref` (стандарт всё ещё принимает)
* `std::vector`/любой continuous контейнер -> [`std::span`](https://en.cppreference.com/w/cpp/container/span)

---

# Виды памяти. Глобальная память

* Инициализируется до `main` дефолтными значениями
* В неопределённом порядке создаются и разрушаются

```cpp
int x;
int main() {}
```

---

# Виды памяти. Глобальная память

* `static` переменные инициализируются один раз и всегда могут быть использованы
* Такие переменные называют Singleton, они thread safe, об этом ещё поговорим.
* Тоже глобальная память, чуть лучше, можно использовать в .h файлах
* `std::cin`, `std::cout` это глобальные переменные
* Все строки в `""` -- глобальные переменные, то есть регионы памяти

```cpp
// inline is for defined functions in .h files.
inline int& GetVersion() {
  static int v = 0;
  return v;
}
inline void IncrementVersion() {
  GetVersion()++;
}
```

---

# Виды памяти. Глобальная память

* Огромный факап в Google на 2 часа с большими потерями денег:

```cpp
#include "absl/flags/flag.h"

// Global variable
int flag1_value;
int GetFlag1Value() { return flag1_value; }

// Then this static int
static int flag1_dynamic = GetFlag1Value();

// Argument flag declaration. "./binary --flag1=123"
ABSL_FLAG(int, flag1, 1, "flag1").OnUpdate([]() {
  flag1_value = absl::GetFlag(FLAGS_flag1);
});
```

---

# Виды памяти. Глобальная память

* Огромный факап в Google на 2 часа с большими потерями денег:

```cpp
// Same but flag and static variables are changed.
int flag2_value;

int GetFlag2Value() { return flag2_value; }

ABSL_FLAG(int, flag2, 1, "flag2").OnUpdate([]() {
  flag2_value = absl::GetFlag(FLAGS_flag2);
});

static int flag2_dynamic = GetFlag2Value();
```

---

# Виды памяти. Глобальная память

```cpp
// Fails
TEST(FlagTest, Flag1) {
  EXPECT_THAT(flag1_dynamic, testing::Eq(1));
}

// Passes
TEST(FlagTest, Flag2) {
  EXPECT_THAT(flag2_dynamic, testing::Eq(1));
}
```

---

# Виды памяти. Глобальная память

* Используется в основном только для каких-то супер унифицированных на всю программу вещей
* Например, ввод, вывод, метрики, конфиги (примеры библиотек, [glog](https://github.com/google/glog), [absl::Flag](https://abseil.io/docs/cpp/guides/flags))
* Старайтесь избегать, порядок создания и разрушения не гарантирован
* Разрушение можно починить
* Многие инициализации тоже с помощью `constinit`, больше на семинаре

```cpp
T& GetObj() {
  static T* x = new T(); // Not a leak! Static storage duration is once per program.
  return *x;
}
```

---

# Виды памяти. Глобальная память

* Глобальные переменные хорошо делать только константными, например, `constexpr`, так они инициализируются на этапе компиляции, поэтому гарантии есть
* То есть в какой-то степени создание тоже можно починить у константных объектов

```cpp
static constexpr int kConstant = 42;
```

* Больше на семинарах

---

# Виды памяти. Битовые поля

* Структуры могут у числовых полей забирать биты
* Эти поля невозможно адресовать
* Используется в самых страшных оптимизациях для упаковки структур

```cpp
struct S {
  // three-bit unsigned field,
  // allowed values are 0...7
  unsigned int b : 3;
  // Values are -4..3
  int c : 3;
};
int main() {
  S s = {6, 3};
  // unsigned int* x = &s.b; // compiler error
}
```
