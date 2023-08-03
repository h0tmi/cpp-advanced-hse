---
style: |
  {
    justify-content: start;
  }
paginate: true
---

# Этапы сборки

- Препроцессор (часть компилятора) раскрывает все директивы препроцессора. `#include` — просто вставка содержимого файла.
* Компилятор независимо собирает `*.cpp` (translation units) в объектные файлы. Это место отлично параллелится.
* Линкер (компоновщик) компонует все объектные файлы в финальную программу.

---

# Этапы сборки. Препроцессор

- Можно выполнить все подстановки руками, результат не поменяется.
* Да, каждый `#include <iostream>` вставляет кучу кода в ваш `.cpp`-файлик.
* Как посмотреть на выхлоп препроцессора? `clang++ -E kek.cpp`

---

# Этапы сборки. Препроцессор

```cpp
λ cat kek.cpp
#include <iostream>

int main() {
}

λ clang++ -E kek.cpp | rg -v '^# \d+' | wc -c
 1690652
```

---

# Объявления и определения

- _Объявления_ вносят новое имя для сущности (функции / класса / глобальной переменной).
```cpp
int foo();
int bar() {
    return 42;
}

class Vector;

struct StringView {
    const char* begin;
    const char* end;
};

static constexpr int kBlockSize = 512;
extern const int SomeExternallyDefinedInt;
```

---

# Объявления и определения

- _Определения_ целиком определяют сущность.
Каждое определение является объявлением, но не наоборот.
```cpp
int bar() {
    return 42;
}

struct StringView {
    const char* begin;
    const char* end;
};

namespace NFoo {

} // namespace NFoo

const int a = 19;
```

---

# Объявления и определения. One definition rule
- Перед тем, как использовать сущность, нужно ее объявить.
```cpp
void bar() {
  foo(); // Error: Use of undeclared identifier 'foo'
}

void foo(); // Declaration

void baz() {
  foo(); // OK
}
```

---

# Объявления и определения. One definition rule
- В финальной программе должно быть как минимум одно определение каждой используемой сущности.
- Почти всегда больше одного определения во всей программе -- ошибка.
* Какие исключения?

---

# Объявления и определения. One definition rule
- В каждом TU должно быть не больше одного определения каждого типа
- Во всех TU должно быть не больше одного уникального определения каждого типа
- Во всех TU должно быть не больше одного определения функции
- Во всех TU должно быть не больше одного уникального определения inline функции

---

# Inline функции
- Так нельзя:
```cpp
// a.cpp
int Foo() {
    return 42;
}

// b.cpp
int Foo() {
    return 42;
}
```

---

# Inline функции
- Так можно:
```cpp
// a.cpp
inline int Foo() {
    return 42;
}

// b.cpp
inline int Foo() {
    return 42;
}
```

---

# Inline функции
- Так все равно нельзя:
```cpp
// a.cpp
inline int Foo() {
    return 42;
}

// b.cpp
inline int Foo() {
    return 19;
}
```

---

# Inline функции
- Шаблонные функции неявно inline
- Методы класса, определенные в теле, неявно inline
- Полные специализации функций не inline: это обычная функция

---

# External и internal linkage
1. Бывают сущности разных linkage.
1. По умолчанию, все external: можно определить в одном TU и использовать в другом.
1. Можно сделать сущности internal linkage: приватные для TU.
1. Ключевое слово `static`, unnamed namespace.

---

# External и internal linkage
- static:
```cpp
static void foo() {
    // visible only in the current .cpp file
}
```

- Unnamed namespace:

```cpp
namespace {

void bar() {
    // visible only in the current .cpp file
}

} // anonymous namespace
```

---

# ODR. Правила
1. Пишите шаблоны целиком в `.h`. Можно сделать `.impl.h`.
1. Пишите методы классов либо целиком в `.h` (редко), либо в `.cpp`
1. Пишите объявления обычных функций в `.h`, остальное в `.cpp`
1. Внутри `.cpp` пишите все локальные для модуля сущности в unnamed namespaces
