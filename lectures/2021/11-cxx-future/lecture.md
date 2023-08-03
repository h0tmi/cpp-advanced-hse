---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# C++20 and beyond

Данила Кутенин. Лекция 11

---

## Modules

* Заменят хедер файлы
* Модули указывают, что должно быть экспортировано -- классы, функции и тд
* Модули могут прятать интерфейс от реализации (встроенный pimpl)
* Никаких `#include`
* Экспорт уменьшает время компиляции, но увеличивает использование диска
* Порядок модулей не важен

---

```cpp
export module hse;

import <string>;

namespace hse {
  std::string GetStringPrivate() { return "Deadline is extended"; }
  export auto GetString() { return GetStringPrivate(); }
}
```

---

```cpp
import hse;

import <iostream>;

int main() {
  std::cout << hse::GetString();
}
```

---

```console
clang++ -stdlib=libc++ \                  # use libcxx
        -std=c++2a \                      # c++20
        -fimplicit-modules \              # assume modules
        -fimplicit-module-maps \          # Dump all from hse to be exported
        -c hse.cpp -Xclang \
        -emit-module-interface -o hse.pcm # precompiled module
```

```console
clang++ -stdlib=libc++ \
        -std=c++2a \
        -fimplicit-modules \
        -fimplicit-module-maps \
        -fprebuilt-module-path=. \  # Find all .pcm files in the folder
        main.cpp hse.cpp -o main
```

---

## Avoid building .cpp file

```console
clang++ -stdlib=libc++ \
        -std=c++2a \
        -fimplicit-modules \
        -fimplicit-module-maps \
        -fmodules --precompile hse.cppm -o hse.pcm # .cppm

clang++ -fmodules -c hse.pcm -o hse.o
```

```console
clang++ -stdlib=libc++ \
        -std=c++2a \
        -fimplicit-modules \
        -fimplicit-module-maps \
        -fprebuilt-module-path=. \  # Find all .pcm files in the folder
        main.cpp hse.o -o main
```

---

```cpp
import hse;

import <iostream>;

int main() {
  std::cout << hse::GetStringPrivate();
}
```

Compile time error

---

## Modules

* Всё зависит от компилятора
* Пока нет нормального способа в системах сборки уметь в модули
* Сильно экономит сборку
  * Мы в Google уже года два на модулях -- его поддерживает тот, кто модули написал в clang
  * Как-то работает для стандартных библиотек и экономит тучу времени
* Можно переводить потихоньку, скажем, стандартные библиотеки уже начали это делать
  * На дворе 2021 и только std это сделали, мда
* Если писать с нуля и разобраться в команднах, в целом должно быть очень хорошо

---

## Ranges

* Range это объект, который указывает на какой-то массив данных, по которому можно итерироваться
* Похоже на .begin(), .end(), но меньше ошибок
* Зачем?
  * Удобнее писать(?)

```cpp
std::sort(data.begin(), data.end());
data = {data.begin(), data.begin() + 5};
// =>
ranges::sort(data) | views::drop(5);
```


---

## Ranges

* Любой контейнер с .begin(), .end() валидный range
* Алгоритмы поддерживают ranges
* Преобразования с помощью пайплайнов
* views -- невладеющие и не изменяющие range функции
* Фильтрация и больше читаемости кода
* Lazy исполнение

```cpp
std::vector<int> data{...};
auto filtered{
  data
    | views::filter([](int value) { return isprime(value); })
    | views::transform([](int value) { return value * 2; })
    | views::reverse
};
```

---

## Ranges

![Ranges](./ranges.png)

https://libcxx.llvm.org/Status/Ranges.html

---

## Ranges

```cpp
std::vector<int> il = {3, 1, 4, 1, 5, 9};
for (int i : il | std::views::reverse) {
  std::cout << i << ' ';
}
```

В 2021 мне надо

* Установить llvm-13 из исходников, получить только где-то на 30-40% готовые ranges
* Шаманить

```console
clang++ -std=c++20 -O3 -fmodules -nostdinc++ -nostdlib++ \
  -isystem /usr/local/include/c++/v1/ -L/usr/local/lib \
  -Wl,-rpath,/usr/local/lib \
  -lc++ example.cpp -o example
```

---

## Ranges

* Отвратительно долго компилируются, с модулями должно быть чуть полегче
  * Не сильно, шаблоны очень тяжелые даже с модулями
* Lazy исполнение может быть немного опасным
* Можно работать с бесконечными ranges (плюс или минус?)

---

## Корутины

* Как мы говорили, корутины сами передают управление
* Но немного предыстории

```cpp
std::vector<int> seq = {1, 2, 3};
for (int i : seq) {
  DoWork(i);
}
```

Удобно, согласны, узнали?

---

## Корутины

* Иногда хочется вызывать функции в цикле и менять стейт, типа функция что-то _гененирурует_

```cpp
DoWork(foo());
DoWork(foo());
DoWork(foo());
```

```cpp
int foo() {
  static int x = 0; // Global memory!
  return x++;
}
```

---

## Корутины

* Можно генератор или лямбду

```cpp
class foo {
  int i = 0;
public:
  int operator()() {
    return i++;
  }
};

auto foo = [i = 0]() mutable {
  //        ^^^^^ state
  return i++;
}
```

---

## Корутины

* Обычно функции имеют глобальный стейт + локальный стейт, при выходе локальный
  разрушается

```cpp
type func() {
  // vars
  another_type foo = ...;

  return bar; // everything is destroyed
}
```

---

## Корутины

* Корутины позволяют вам прерывать исполнение, сохраняя стейт
  * Его называют coroutine state
* Да, плодишь много корутин -- много используешь памяти, facebook с этим столкнулся, например

```cpp
type func() {
  // vars
  another_type foo = ...;

  // Yield
  co_yield foo;
  // Resume

  return bar; // everything is destroyed
}
```

---

## Корутины

* Генераторы писать самому в C++20, а в C++23 будут уже какие-то стандартные (P2168)

```cpp
cppcoro::generator<int> foo() {
  int i = 0;
  while (true) {
    i += 2;
    co_yield i;
  }
}

int main() {
  int x = 0;
  for (const auto& el : foo()) {
    std::cout << el << '\n';
    x++;
    if (x > 10) break;
  }
}
```

---

## Корутины

* Пользовательский код записывает значение в promise\_type
* std::coroutine_handle переходит в корутину обратно когда надо (в данном случае при ++ у итератора)
* Coroutine Frame это память на куче, но компиляторы там могут оптимизировать что-то

---

## Корутины

* Вспомните scheme
* Возможно иногда было бы полезнее пользоваться корутинами
  * Чтение делает yield токенайзеру
  * Токенайзер делает yield парсеру
  * Парсер вычислениям
* Так легче находить ошибки, нет как такого стейта между абстракциями
* Не пробовал, но мб реально легче будет

---

## Корутины

* Есть ещё co_await, co_return -- это suspension, чтобы избавиться от бесконечных коллбеков

```cpp
async_generator<T> func() {
  // Suspend, wait for the result of another function, for example, network
  // request
  auto value = co_await another_func();
  // Resume

  co_return bar;
}
```

* another_func даст сигнал, что можно продолжать исполнение в `func()`.
* Как обычно, не плодите слишком много корутин

---

## Концепты

* Поговорили на семинарх

---

## Lambda

* Уже поговорили, напомню, что теперь lamdbda функции могут быть шаблонными
* Небольшие изменения с parameters pack, не так важно, но полезная мелочь

```cpp
[] <typename T> (T x) { }
```

---

## Constexpr

* Compile time вычисления всё ещё живут
* Compile time std::string, std::vector
* Compile time вычисления
  * Это просто абстракция на уровень выше -- на уровне компилятора
  * Компилятор может выделить память -- может
  * Бинарь имеет статическую память? Имеет
  * Вроде нет проблем

```cpp
constexpr std::vector<int> gen() {
  std::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  v.push_back(4);
  v.push_back(5);
  return v;
}
static_assert(gen() == {1, 2, 3, 4, 5});
```

---

## Constexpr

* Зачем? Хороший вопрос
  * Почему тогда всё не constexpr?
  * Очень много бессмысленных дебатов в комитете

---

## Atomic smart pointers

* `shared_ptr<T>` потокобезопасный
  * Иногда да, гарантируется только одно разрушение
  * Нет, если есть перезапись внутри
* `atomic<shared_ptr<T>>`
  * Внутри спинлок по всех имплементациях (что-то я не знаю, почему)

---

## Threading

* `std::jthread`
  * Можно делать .request_stop() и другие треды могут иметь std::stop_token
  * `std::stop_source`
  * `std::stop_callback`

```cpp
void DoWork() {
  std::thread thr([] { ... });
  try {

  } catch (...) {
    thr.join();
    throw;
  }
  thr.join();
}
```

---

## Threading

```cpp
void DoWorkInCxx20() {
  std::jthread thr([] { ... });
  try {

  } catch (...) {
    throw;
  }
}
```

---

## Cooperative cancellation

```cpp
std::jthread thr{[](std::stop_token token) {
  while (!token.stop_requested()) {
    //...
  }
}};

// ...

job.request_stop();
```

---

## Semaphores

* Добавили семафоры
* Counting semaphore: non negative resource count, `std::counting_semaphore`
* Binary semaphore: mutex, `std::binary_semaphore`
* Много было споров, надо ли в стандарт, приняли в итоге

---

## Thread primitives

* `std::latch`
  * Это число, треды могут ждать на нём, пока оно не станет нулём
  * Например, вы что-то хотите почистить и проиницилизировать до какой-то секции
  * Скажем, сделать работу всеми тредами, а потом всеми тредами почистить свой стейт
  * Геймдев разработчики очень просили это :)
    * Разрушение может быть не самым дешёвым

---

```cpp
std::latch work_done(std::size(jobs));
std::latch start_clean_up{1};

auto work = [&](job& my_job) {
  // Working
  work_done.count_down();
  start_clean_up.wait();
  // Cleaning up
};
// Spawn threads
start_clean_up.count_down();
// Threads are cleaning up their state
```

---

`std::barrier` похож

```cpp
auto on_completion = [] {
  // Entering intermediate state.
};
std::barrier sync_point(std::size(jobs), on_completion);
auto work = [&](job& my_job) {
  // Working
  sync_point.arrive_and_wait();
  // on_completion is called
  // Cleaning up
  sync_point.arrive_and_wait();
};
```

---

## Thread primitives

* Можно делать wait/notify на изменение atomic.
* Сделано через futex
* Забавно, что futex работает только с 32 битными числами
* Это значит 64 битные атомики используют лок/cпинлок :)
* https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/atomic_wait.h
* В Linux 5.16 (буквально 2 недели назад) добавили futex на все размерности

---

## Thread primitives

```cpp
std::mutex m;
std::condition_variable cv;
std::atomic<bool> a(false);
std::atomic<bool> b(false);
std::thread t([&] {
  cv.notify_one();
  a.wait(false);
  if (a.load())
    b.store(true);
 });
std::unique_lock<std::mutex> l(m);
cv.wait(l);
std::this_thread::sleep_for(100ms);
a.store(true);
a.notify_one();
t.join();
VERIFY( b.load() );
```

---

## Readability

* Можно у простых типов указывать имена, designated initializers
* Очень повышает читаемость кода

```cpp
struct Config {
  bool use_feature_1;
  bool use_feature_2;
  std::string param;
};


Config config{true, false, "abacaba"};
// vs
Config config{.use_feature_1 = true, .use_feature_2 = false, .param = "abacaba"};
```

---

## Simplification

* Все методы сравнения теперь выразить можно через оператор `<=>`
* Единственное изменение стандарта удалившее текст, нежели добавившее

```cpp
constexpr auto X::operator<=>(const Y& y) const = default;
```

---

## Non-type template parameters

* Теперь в шаблонах можно некоторые типы
* В основном для всяких регулярок в compile time

```cpp
ctre::match<"[a-z]+([0-9]+)">(str)
```

---

## std::span

* То же самое, что и `std::string_view`, но для непрерывных контейнеров типа вектора и тд
* Finally

---

## std::format

* Теперь можно избавиться от дурацких printf/scanf и наслаждаться очень быстрым std::format
* Делает один наш хороший белорусский знакомый в Facebook

```cpp
std::cout << std::format("{0}{0}{0}", "he") // hehehe
std::cout << std::format("{:=^26}", "Deadline is extended") // ===Deadline is extended===
```

---

## Много по мелочи

* Расскажу, если успею
