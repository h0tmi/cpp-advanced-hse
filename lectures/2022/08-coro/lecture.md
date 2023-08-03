---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Корутины

Сергей Скворцов. Лекция 8

---

# Генератор
Сгенерируем подряд идущие числа:
```cpp
for (int i : Range(100)) {
    printf("%d\n", i); // 0 .. 99
}
```

---

# Генератор
Как реализовать `Range`?
```cpp
std::vector<int> Range(int max) {
    std::vector<int> res(max, 0);
    std::iota(res.begin(), res.end(), 0);
    return res;
}

for (int i : Range(100)) {
    printf("%d\n", i); // 0 .. 99
}
```

---

# Генератор
Как реализовать `Range`?
```cpp
class IntIterator {
    // 50 lines of strange code
};

IteratorRange<IntIterator> Range(int max) {
    return {IntIterator{0}, IntIterator{max}};
}

for (int i : Range(100)) {
    printf("%d\n", i); // 0 .. 99
}
```

---

# Генератор
Как реализовать `Range`?
```cpp
class IntIterator {
    // 50 lines of strange code
};

IteratorRange<IntIterator> Range() {
    return {IntIterator{0}, IntIterator{}};
}

for (int i : Range()) {
    printf("%d\n", i); // 0 ..
}
```

---

# Генератор в Python
Как реализовать `Range`?
```python
def range():
    num = 0
    while True:
        yield num
        num += 1

for i in range():
    print(i)
```

---

# Обычные функции
Что можно делать с функциями? Какой у них интерфейс?
* Вызывать
* Вернуться (получить результат)

---

# Обычные функции
Обе операции можно выразить через несколько элементарных:
* Вызов функции – сохранение текущего состояния и активация состояния для вызываемой функции
* Возврат из функции – уничтожение текущего состояния и переход в контекст вызывающей функции

---

# Обычные функции
Обе операции можно выразить через несколько элементарных:
* Вызов функции – SUSPEND + ACTIVATE
* Возврат из функции – TERMINATE + YIELD

---

# Необычные функции
_Корутины_ (coroutines, сопрограммы):
* Вызов функции – SUSPEND + ACTIVATE
* Возврат из функции – TERMINATE + YIELD
* Переключение в родителя – SUSPEND + YIELD
* Переключение в ребенка – SUSPEND + YIELD

---

# Виды корутин
* Stackful vs stackless
* Симметричные или асимметричные
* C++20 – асимметричные (иногда симметричные) и stackless.

---

# Python
```python
def numbers():
    num = 0
    while True:
        yield num
        num += 1

for num in numbers():
    print(num)
```

---

# Python
```python
def numbers():
    num = 0
    while True:
        yield num
        num += 1

gen = numbers()
gen.send(None) # 0
gen.send(None) # 1
gen.send(None) # 2
```

---

# C++20
```cpp
nonstd::generator<int> numbers() {
    int num = 0;
    while (true) {
        co_yield num;
        num += 1;
    }
}

auto gen = numbers();
gen(); // 0
gen(); // 1
gen(); // 2
```

---

# C++20
```cpp
nonstd::generator<int> numbers() {
    int num = 0;
    while (true) {
        co_yield num++;
    }
}

for (int num : numbers())
    printf("%d\n", num);
```

---

# Корутины в C++20
Корутина – функция, где есть использование одного из трех `co_`-операторов:
* `co_return`
* `co_yield`
* `co_await`

* Первые два – сахар, интереснее третий

---

# Корутины в C++20
```cpp
// generated code
nonstd::generator<int> numbers(int from, int to, int step) {
// ^^^^^^^^^^^^^^^^^^^ return_object

    using return_object = nonstd::generator<int>;
    using promise_type = typename return_object::promise_type;
    using coroutine_handle = std::coroutine_handle<promise_type>;

    promise_type promise{from, to, step};
    return_object res = promise.get_return_object();

    // ...
}
```

---

# Корутины в C++20
* `co_yield <value> -> co_await promise.yield_value(<value>)`
* `co_return <value> -> co_await promise.return_value(<value>)`
* `co_await <value>`: приостанавливает управление и передает его вызывающей стороне.

---

# Корутины в C++20
```cpp
// generated code
nonstd::generator<int> numbers(int from, int to, int step) {
    using return_object = nonstd::generator<int>;
    using promise_type = typename return_object::promise_type;

    promise_type promise{from, to, step};
    return_object res = promise.get_return_object();

    co_await promise.initial_suspend();

    try {
        /* coroutine body */
    } catch (...) {
        promise.unhandled_exception();
    }

    co_await promise.final_suspend();
}
```

---

# Promise
Что должно быть в минимальном promise?
+ `promise.get_return_object()`
+ `promise.initial_suspend()`
+ `promise.final_suspend()`
+ `promise.unhandled_exception()`
Есть еще несколько продвинутых методов-точек расширения поведения.

---

# Hello world
```cpp
resumable hello_world() {
    std::cout << "Hello, ";
    co_await std::suspend_always{};
    std::cout << "world!\n";
}

int main() {
    auto coro = hello_world();
    coro.resume();
    coro.resume();
}
```

---

# Hello world. Resumable
```cpp
struct resumable {
    struct promise_type {
        resumable get_return_object() {
            return {std::coroutine_handle<promise_type>::from_promise(*this)};
        }

        std::suspend_always initial_suspend() const {
            return {};
        }

        std::suspend_always final_suspend() const noexcept {
            return {};
        }

        void unhandled_exception() {
            std::terminate();
        }
    };
};
```

---

# Hello world. Resumable
```cpp
struct resumable {
    resumable(std::coroutine_handle<> handle)
        : handle_{handle}
    {}

    struct promise_type {};

    bool resume() {
        if (!handle_.done()) {
            handle_.resume();
        }
        return !handle_.done();
    }

    ~resumable() {
        handle_.destroy();
    }

private:
    std::coroutine_handle<> handle_;
};
```

---

# std::coroutine_handle
+ Легковесный объект, позволяющий управлять корутиной извне.
* Шаблонизирован типом promise
* Специализация для void со стертым типом и меньшим набором методов
```cpp
template <> struct coroutine_handle<void> {
    // resume(), destroy(), ...
};

template <typename Promise>
struct coroutine_handle<Promise> : coroutine_handle<void> {
    // from_promise(), promise(), ...
};
```

---

# std::coroutine_handle
* После каждого suspend нужно позвать `handle.resume()`.
* Можно явно позвать `handle.destroy()`.

---

# Генератор

```cpp
generator<int> range(int max) {
    for (int n = 0; n < max; ++n) {
        co_yield n;
    }
}

int main() {
    auto gen = ints(10);
    while (gen.next()) {
        std::cout << gen.value() << std::endl;
    }
}
```

---

# Генератор
Как дополнить resumable?
```cpp
template <typename T>
struct generator {
    struct promise_type {
        // ...

        auto yield_value(T next) {
            value = std::move(next);
            return std::experimental::suspend_always{};
        }

        T value;
    };
};
```

---

# Генератор
Как дополнить resumable?
```cpp
template <typename T>
struct generator {
    struct promise_type {
        // ...

        auto yield_value(T next) {
            value = std::move(next);
            return std::experimental::suspend_always{};
        }

        T value;
    };
};
```

---

# Awaitable
+ На чем можно звать `co_await`? На любых Awaitable.
+ Awaitable определяет конкретное поведение при `co_await`. Это небольшой класс с тремя обязательными методами:
    ```cpp
    struct Awaitable {
        bool await_ready();
        void await_suspend(std::coroutine_handle<promise>);
        T await_resume();
    };
    ```

---

# co_await
Во что оно превращается?
```cpp
// co_await <expr>

auto&& awaiter = <expr>;

if (!awaiter.await_ready()) {
    awaiter.await_suspend();
    // suspend + yield
    // resume
}
awaiter.await_resume();
```

---

# Awaitable
+ Есть два стандартных Awaitable: `std::suspend_always` и `std::suspend_never`
    ```cpp
    struct suspend_always {
      bool await_ready() const { return false; }
      void await_suspend(coroutine_handle<>) const {}
      void await_resume() const {}
    };

    struct suspend_never {
      bool await_ready() const { return true; }
      void await_suspend(coroutine_handle<>) const {}
      void await_resume() const {}
    };
    ```

---

# Symmetric control transfer:
+ Корутины бывают симметричными, приняли незадолго до принятия корутин в стандарта
* `await_suspend` умеет возвращать `void`, `bool` или `std::coroutine_handle` (!)
* Последний вариант позволяет организовывать symmetric control transfer 
