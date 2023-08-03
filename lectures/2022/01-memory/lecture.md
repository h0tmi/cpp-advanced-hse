---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Введение. Объекты. Память

Сергей Скворцов. Лекция 1

---

# Курс

* 15 лекций и семинаров
* 15 маленьких ДЗ, 3 больших
* Бонусные задания
* $\mathrm{Round}(0.6 \cdot O_{\textrm{большие ДЗ}} + 0.4 \cdot O_{\textrm{маленькие ДЗ}} + 0.1 \cdot O_{\textrm{тесты}} + 0.1 \cdot O_{\textrm{Бонус}})$

---

# Курс. Маленькие ДЗ

* Написать один-два файла, закрепить материал
* Каждую неделю после лекции в 15:00, **сегодня тоже**
* Дедлайн через 13 дней, в 23:59 четверга
* Штрафы после дедлайна вычисляются по формуле $\max\left(\frac{Y}{e^{X/5}}, 0.3Y\right)$, где $Y$ &mdash; количество баллов за задачу, $X$ &mdash; количество дней после дедлайна (может быть дробным)
 * После 1 часа 99.17%, после 2 часов 98.3%, после 5 часов 95.92%, после полусуток 90.5%, после суток 81.87%, после \~6 &mdash; 30% до конца курса.
* Тесты всегда открытые. Проверяются через GitLab CI.

---

# Курс. Большие ДЗ

* Модульные на 500-2000 строк вместе с тестированием, сложные концепты, дотошность в деталях, код ревью по желанию
* Отдельный дедлайн (обычно 2-3 недели)
* После дедлайна 0 баллов без продления
* **Да, у нас резист к спеллу переноса дедлайнов**
* Тесты всегда открытые. Проверяются через GitLab CI.

---

# Курс. Тесты

* Регулярные тесты в конце семинаров на усвоение материала
* 2-3 простых вопроса, ответ в свободной форме
* Обратная связь

---

# Курс. Бонус

* Любой развлекательный материал, креативные и сложные задачи
* Какая-то еще активность на усмотрение преподавателей

---

# Курс. Организация

* https://t.me/cpp_advanced_hse_2022
* Канал — главный источник правды
* Чат для обсуждения, в него можно кидать ссылки на решения
* Репозиторий https://gitlab.com/danlark/cpp-advanced-hse
* https://cpp-hse.net/ сайт с дедлайнами и вашими задачами
* После лекции мы выдадим код регистрации

---

# Курс. Организация

* Любое списывание хотя бы одной задачи &mdash; заявление в УО
* Любое умышленное приведение в нестабильность системы &mdash; заявление в УО
* Любое умышленное нарушение условий или ограничений задачи &mdash; обнуление всей недели
  * Иногда можно написать какие-то костыли, которые не удовлетворяют текстовому условию
  * Учим писать код, а не подгонять под тесты
  * Выборочно будем проверять

---

# Курс. Цели

* Как и почему применять современный C++
* Много кода, набиваем руку
* Развиваем интуицию, не зубрим стандарт
* После можно идти писать код в прод
* Основан на замечательном курсе по C++ в ШАДе, но перезачесть будет нельзя

---

# Курс. Написание кода

* Windows не поддерживается, предпочтителен Linux (виртуалка с Ubuntu)
* Проверяйте решения под санитайзерами (`-fsanitize=address`)
* Дебаггер (`gdb`)

---

# Лектор

* Тимлид в базовом поиске Яндекса
* Куча кода на C++
* СУНЦ, ФКН, ШАД

---

# Память

* Память — самая главная абстракция C++
* Когда вы пишете код, вы постоянно должны понимать откуда и как память идёт
* С большой силой приходит большая ответственность: 80% всех уязвимостей и ошибок связанны с неправильной работой с памятью. Rust, Carbon или починка C++?

---

# Объекты

* Создаются с вызовом конструктора, разрушаются после завершения деструктора
* Доступ до объекта вне времени жизни — UB
* Представление объекта — выровненный регион памяти в `sizeof(Object)` байт
* Переменная — объект или ссылка на него

---

# Память

* Множество последовательных регионов байтов
* Байт — `char`, `sizeof(char) == 1`, `CHAR_BITS = ?`
* Объект прибит к последовательному участку памяти
* Копия, перемещение — другой объект
* Несколько видов памяти

---

# Ручное управление жизнью объекта
* Не повторять дома:
    ```cpp
    char* buf = ...;
    new (buf) std::string("Hello, world!");
    ...
    reinterpret_cast<std::string*>(buf)->~string();
    ```
* Ну уж точно не повторять дома:
    ```cpp
    std::string kek("Hello, world!");
    (&kek)->~string();
    new (&kek) std::string("Muhaha");
    ```

---

# Жизнь объекта
* Сырые байтики + время жизни
* Разные объекты могут переиспользовать одну память, но иметь разное время жизни
    ```cpp
    std::vector<int> vec;
    vec.push_back(0); // vec[0] — object #1
    vec.pop_back();
    vec.push_back(1); // vec[0] — object #2
    ```

---

# Виды памяти

* Automatic storage duration (жаргон "на стеке")
* Static storage duration (жаргон "глобальный")
* Dynamic storage duration (жаргон "в куче")
* Thread-local storage duration (C++11, после многопоточки)

---

# Динамическая память

* Выделяется и освобождается руками
* Объекты живут столько, сколько напишет программист
* Сырые `operator new`, `malloc`
* Типизированный `new` expression (`new int`, `new char[32]`)
* Обертки вроде `std::vector`, `std::string`

---

# Динамическая память

```cpp
int* kek = new int{42};
delete kek;

int* array = new int[100];
delete array;

std::unique_ptr<int> ptr(new int{19});

// Calls operator new(sizeof(int) * 100);
std::vector<int> v(100);

std::unordered_map<int, int> x;
// calls new std::pair<const int, int>(1, 2);
x[1] = 2;
```

---

# Динамическая память
```cpp
int* x = new int();
delete x;

int* arr = new int[100];
delete[] arr;
// delete arr; // Undefined behaviour!

new int(); // Leak, but not UB. Memory leaks are safe
```

* За каждым `new` должен идти ровно один корректный `delete`
* `std::vector`, `std::unique_ptr`, другие контейнеры это делают в деструкторе
* Нет `new`, нет `delete`

---

# Динамическая память. Эпоха динозавров

* В С это `malloc`
* Отличия от new expression:
  * `new` может кинуть исключение при недостатке памяти (зависит от системы, не обязательно), `malloc` вернёт `nullptr`
  * `new` создает объекты, `malloc` не создает (почти, C++20 все испортил)
  * `new` типизированный
  * То, что `malloc` это функция, а `new` — оператор и поэтому быстрее — ужасные комментарии плохих гайдов по C++
* Отличия от operator new:
  * `operator new` кидает исключение (опционально)
  * Больше [настроек](https://en.cppreference.com/w/cpp/memory/new/operator_new)

---

# Динамическая память. `operator new`
* Можно перегружать глобально и на уровне одного объекта
* Редко используемая фича, специфичные проблемы

---

# Ручное управление динамической памятью
https://github.com/torvalds/linux/blob/master/mm/mmap.c#L1814-L1860
```c
void* buf1 = malloc(1024);
// ...
if (error)
    goto err1;
// ...
void* buf2 = malloc(1024);
// ...
if (error)
    goto err2;
// ...
err2:
    if (buf2 != NULL) free(buf2);
err1:
    if (buf1 != NULL) free(buf1);
```

---

# Ручное управление динамической памятью

```cpp
int *x = new int[100];
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

# }

* Лучшая фича C++ — закрывающаяся скобка

```cpp
} // Destructors are called, new better be paired with delete
```

---

# Динамическая память

* Не пользуйтесь `malloc`, `operator new`, `new expression` никогда.
* Используйте `std::unique_ptr` и `std::make_unique`, контейнеры
* Использование `new` — моветон, так плохо писать код; скорее всего, будут ошибки
  * Проверяйте свой код Address Sanitizerом

---

# Аллокаторы

```cpp
template<
    class T,
    class Allocator = std::allocator<T>
> class vector;
```

* Большинство контейнеров имеют дополнительный шаблонный аргумент.
* Удобная абстракция над выделением/освобождением памяти
* Стандартный `std::allocator` это обёртка над `new`.

---

# Аллокаторы. `std::allocator<T>`

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

# Аллокаторы. `OlympicAlocator<T>`

```cpp
char GlobalArena[128 * 1024 * 1024];
char* NextFree = &GlobalArena;

template <typename T>
struct OlympicAlocator {
    T* allocate(size_t n) {
        size_t count = n * sizeof(T);
        T* ptr = reinterpret_cast<T*>(NextFree);
        NextFree += n * sizeof(T);
        return ptr;
    }

    void deallocate(T* p, size_t n) noexcept {
        // ¯\_(ツ)_/¯
    }
}
```

---

# Аллокаторы. `StackAllocator<T>`

* https://github.com/catboost/catboost/blob/master/library/cpp/containers/stack_vector/stack_vec.h#L34-L93
*   ```cpp
    template <typename T, typename CountOnStack>
    class StackAllocator {
        // ... 
        T* allocate(size_type n) {
            if (!IsStorageUsed && CountOnStack >= n) {
                IsStorageUsed = true;
                return reinterpret_cast<T*>(&StackBasedStorage[0]);
            } else {
                return FallbackAllocator().allocate(n);
            }
        }

        T StackBasedStorage[CountOnStack]; // WTF is std::aligned_storage_t<sizeof(T), alignof(T)> ?!
        bool IsStorageUsed = false;
    }
    ```
* Хитрый вектор (`TStackVec<T, N>`): первые N элементов лежат прямо в объекте, без аллокации памяти, потом аллокация

---

# Аллокаторы

* Если вы видите шаблонный аргумент аллокатора, под капотом new и этот объект хранит в себе динамическую память
* Если нет, значит деструкторы не удаляют память (пример, `std::array`)

---

# Аллокаторы

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

* Как они хранятся?
* Ответ на семинаре :)

---

# Системные аллокаторы

* Коллизия в терминологии (`std::allocator` vs `jemalloc`)
* Как устроен `malloc` / `operator new`?
* Сложный механизм между системой и вашим приложением
* Под капотом аллокаторы сложны, это целая наука
* Paging, cache locality, thread/core local stack caches, arenas. Это всё про АКОС
* Но стоит знать, что new не всегда возвращает память из кучи, просто `heap` vs `stack` хорошее разделение
* Аллокаторы не всегда дешёвые, много мелких аллокаций хорошо могут так замедлить вашу систему
* Хороший аллокатор в сложных случаях дает свои проценты произодительности

---

# Системные аллокаторы. Правила

* Если нужны аллокации, аллоцируйте как можно больше одним куском памяти
* Не переусердствуйте, но и не давайте программе аллоцировать/деаллоцировать очень много (сотни тысяч в секунду)
* Непрерывные куски памяти (и вообще локальность) всегда быстрее работают
  * Так устроена память, кэши

---

# Динамическая память. Пример

```cpp
std::vector<int> x;
for (size_t i = 0; i < n; ++i) {
  x.push_back(f(g(i)));
}
```

---

# Динамическая память. Пример

```cpp
std::vector<int> x;
x.reserve(n); // Important, removes log(n) allocations
for (size_t i = 0; i < n; ++i) {
  x.push_back(f(g(i)));
}
```

---

# Автоматическая память
* Большинство объектов, которые вы создаёте, лежат на "стеке"

    ```cpp
    int x;
    int array[10];
    std::array<int, 10> std_array;
    std::string string_array[10];

    int* y = &x; // location of x, alternatively, you can use std::addressof(x)
    // std::addressof(x) is universal for types which do overload `operator&`
    ```

* Стек — это "преаллоцированная" память, часто ограниченная несколькими килобайтами (8KiB стек потока в ядре Linux) — парой мегабайт

---

# Автоматическая память
* Конструктор при объявлении, деструктор по выходе из scope (компилятор вставляет за нас)
  ```cpp
    void foo() {
        std::vector<int> vec(100, 42); // calls std::vector::vector(size_t, T)
    } // calls std::vector::~vector()
  ```
* Разрушаются в порядке, обратном объявлению
  ```cpp
    void foo() {
        Noisy a;
        Noisy b;
        // calls b.~Noisy()
        // calls a.~Noisy()
    }
  ```

---

# Стек
* Стек легко выесть:
    ```cpp
        void foo(int depth) {
            char buf[1024];
            if (depth > 0) {
                foo(depth - 1);
            }
        }

        foo(1024); /// BOOM! Used ~1MiB of stack
    ```
* Поэтому на стеке располагаются небольшие переменные, часто ссылки на большие объекты в куче
* Рекурсия в целом опасна, особенно если ее глубина зависит от пользовательских данных

---

# Глобальная память

* Инициализируется до `main` дефолтными значениями
* В неопределённом порядке создаются и разрушаются
* Static initialization order fiasco

```cpp
int x;
int main() {}
```

---

# Глобальная память

* `static` переменные инициализируются один раз и всегда могут быть использованы
* Такие переменные называют singleton, они thread safe, об этом ещё поговорим.
* Тоже глобальная память, чуть лучше, можно использовать в .h файлах
* `std::cin`, `std::cout` это глобальные переменные
* Все строки в `""` — глобальные переменные, то есть регионы памяти

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

# Глобальная память

* Используется в основном только для каких-то крайне унифицированных на всю программу вещей
* Например, ввод, вывод, метрики, конфиги (примеры библиотек, [glog](https://github.com/google/glog), [absl::Flag](https://abseil.io/docs/cpp/guides/flags))
* Старайтесь избегать, порядок создания и разрушения не гарантирован

```cpp
T& GetObj() {
  static T* x = new T(); // Leaky singleton
  return *x;
}
```

---

# Глобальная память

Глобальные переменные хорошо делать только константными, например, `constexpr`, так они инициализируются на этапе компиляции, поэтому гарантии есть

```cpp
static constexpr int kConstant = 42;
```

---

# Указатели, ссылки
Нужно уметь давать передавать "ссылки" на объекты без перемещения самих объектов
```cpp
class SocketReader {
public:
    SocketReader(Socket socket, std::array<char, BUF_SIZE> buffer);
};

void foo() {
    std::array<char, BUF_SIZE> buffer;
    SocketReader reader(socket, buffer); // копия
}
```


---

# Указатели, ссылки
Решения:
1. Указатели
1. Ссылки
1. View-объекты

---

# Указатели

* Указатели обычно указывают на память :)
* Указатель под капотом — число с указанием региона памяти

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

# Указатели

Указатели бывают нулевыми

```cpp
std::unique_ptr<int> ptr; // nullptr under the hood.
std::unique_ptr<int> ptr(nullptr); // equivalent.
int* raw_ptr = nullptr;
```

Разыменовывание нулевого указателя это всегда ошибка

```cpp
// Good taste.
if (ptr) {
  *ptr = 10;
}
```

---

# Указатели

Владеющие указатели с C++ — зло. Есть `std::unique_ptr`, `std::shared_ptr`.
Невладеющие указатели — OK: ссылка на объект, которая может быть пустой.

* Некоторые стайлгайды (Google, Yandex, Bloomberg) предпочитают такой код:
    ```cpp
    int value = 0;
    ComputeSomething(a, b, c, &value); // a, b, c — input, value — output
    ```

*
    ```cpp
    // Same but with a reference
    void ComputeSomething(int a, int b, int c, int& value) {
    }

    ComputeSomething(a, b, c, value); // What is output?
    ```

---

# Ссылки

* Ссылка — другое имя для объекта
    ```cpp
    int a;
    int& b = a;
    ```
* Ссылку нельзя переприсвоить
    ```cpp
    int a = 5;
    int b = 10;
    int& ref = a;
    ref = b;
    assert(a == 10);
    ```

---

# Ссылки vs указатели

* Ссылки под капотом реализуются через указатели
* Ссылка не может быть нулевой, указатель может
* Ссылки бывают rvalue (`int&&`), про это на следующей лекции
* Нельзя брать ссылки на ссылки
* Всё больше стайлгайдов говорят, что указатели никогда не владеют объектом (то есть, они не из кучи или кто-то другой за них ответственный / не надо на них звать `delete`). Подразумевайте это по умолчанию, иначе ошибётесь и запутаетесь

---

# Умные указатели

* `std::unique_ptr` хранит в себе указатель и разрушает его корректно, не боится исключений
* `std::shared_ptr` разделяет хранение и удалет только тогда, когда нет. Для этого есть counter
* Больше на семинарах и в первом дз

---

# View объектов

* `std::string` хранит в себе память, но при передаче `const std::string&` в функцию надо положить указатель в регистр и передать
* `std::string_view` это view объекта, он не ответственный за память, хранит два указателя на начало и конец
* $O(1)$ все операции типа вырезания подстрок, удаления начала и так далее. Через view нельзя модифицировать

---

# View объектов

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

# View объектов

Могут быть compile time константами

```cpp
constexpr std::string_view kFormatString = "Welcome to %s!\n";
```

---

# View объектов

Надо доказывать, что переживают вызовы

```cpp
int main() {
  std::string s = "Hellooooooooooooooo ";
  std::string_view sv = s + "World\n"; // Temporary and assigns to it.
  std::cout << sv; // Undefined behavior
}
```

---

# View объектов

* View — легкие объекты, обязательно должны жить меньше, чем объект, на который указывают
* Передавать по значению (на семинарах расскажут детали и сложности)
* Удобно манипулировать и не ошибиться сюрпризам линейной сложности

---

# Аналогичные примеры

* `std::function` -> `std::function_ref` (C++23, пока не принят)
* `std::vector`/любой continuous контейнер -> [`std::span`](https://en.cppreference.com/w/cpp/container/span)
