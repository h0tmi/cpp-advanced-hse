# Lambdas

## Mutable lambda

```cpp
int foo = 0;
[=](){ foo++; };
```

В этом случае генерируется код
```cpp
class lambda {
int foo;
public:
void operator()() const { foo++; }
};
```

Поэтому надо писать 
```cpp
[=]() mutable { foo++; }
```

Почему работает это?
```cpp
[&]() { foo++; }
```

Потому что генерируется код

```cpp
class lambda {
int& foo;
public:
void operator()() const { foo++; }
};
```

В данном случае foo++ меняет данные, но не саму ссылку.
Сделать философский вывод о том, что const на самом деле имеет немного неинтуитивную семантику

## Recursive lambda

Рекурсивные лямбды всё таки можно делать трюками с C++14,
для этого можно создавать лямбды внутри лямбд. 

Но нельзя писать `auto l = [] { /*вызов l*/ }`, потому что компилятор не знает
типа на момент вызова внутри. 
Зато можно с std::function, потому что тип есть. 
Показать, что это дефекты `int x = x;` и что в целом так лучше не делать.

## Template lambdas in C++17 (stateless)

В C++17 лямбды можно было тоже делать шаблонными, но только stateless. 
Это трюк с глобальными переменными

```cpp
template <class T, class U>
constexpr auto kLambda = [](const T& a, const U& b) {
    return a + b;
};
```

```cpp
int main() {
    return kLambda<int, long>(1, 2);
}
```

## Compile time lambda

Напишите lambda функцию, которая на этапе компиляции вычисляет
первые N простых чисел

```cpp
template <size_t N>
static constexpr size_t kCountDistinctPrimeDivisors = []{}();
```

Подметить, что таким образом можно писать мелкие функции для вычисления чего-то 
и сразу их вызывать, но больше на семинаре про метапрограммирование.

## Stateless lambda as default template argument

Stateless Lambda с C++20 может уходить как дефолтный аргумент в шаблон через decltype

```cpp
std::unique_ptr<FILE, decltype([](FILE* f ) { fclose(f); })> ptr(fopen("file.txt", "r"));
```

https://godbolt.org/z/vqs4PWzz5

## IILE

Immediately invoked lambda

```cpp
std::string s;
if (cond) {
    s = this_string;
} else {
    s = that_string;
}
```

Вызывает оператор копирования, а вот
```cpp
std::string s = [&] {
    if (cond) {
        return this_string;
    } else {
        return that_string;
    }
}();
```

конструктор копирования.

На самом деле так мало кто делает, лучше выносить в отдельную функцию,
но смысл, что lambda оборачивают куски куда изнутри

## Lambda as template argument

```cpp
// Automatic deduction, до C++17 всегда выносили в функции или делали шаблонным
// конструктор, что немного было не очень
Foo f(std::move(lambda));
```

Так параметризуют какие-то супер high performance вещи
избегая виртуальных функций, лямбда как шаблон и правда классно работает,
но разрастается код и люди должны знать зачем они это делают

## Capture this

Ад c capture this (опционально)

`[=, this]` разрешили только с C++20

https://www.nextptr.com/tutorial/ta1430524603/capture-this-in-lambda-expression-timeline-of-change

## Void of std::function

Факап на миллион: std::function умеет менять возвращаемый тип на void

https://gcc.godbolt.org/z/xsYe8TEW4

В итоге
```cpp
std::function<void()> f = []() -> Status { ... return Error(); };
```

В одном из кодов было проигнорировано

## Syntax complications

Когда давным давно когда пытались принять structured bindings столкнулись с проблемой, что

```cpp
const [a, b, c]{f()};
```

имеет двоякое понимание из-за лямбд, поэтому решили сделать, что они могут только инициализироваться через =
