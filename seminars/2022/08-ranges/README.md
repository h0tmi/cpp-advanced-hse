# Ranges

Посмотрим сегодня на важную большую фичу из C++20, которая только недавно стало легко использовать: ranges.
С поддержкой в компиляторах только совсем недавно стало хорошо, поэтому примеры в основном на godbolt.

<!-- vim-markdown-toc GFM -->

* [Итераторы. Как мы жили до C++20?](#Итераторы-Как-мы-жили-до-c20)
    * [Пара итераторов](#Пара-итераторов)
    * [Split](#split)
    * [Stream iterators](#stream-iterators)
* [Ranges](#ranges)
    * [Range](#range)
    * [Range algorithms](#range-algorithms)
        * [Проекции](#Проекции)
            * [Темные времена. C++17, STL](#Темные-времена-c17-stl)
            * [Времена получше. Before C++20, Yandex](#Времена-получше-before-c20-yandex)
            * [С++20. Ranges](#С20-ranges)
        * [Borrowed range](#borrowed-range)
    * [Views](#views)
    * [Range adaptor objects](#range-adaptor-objects)
    * [Комбинирование range adaptor objects](#Комбинирование-range-adaptor-objects)

<!-- vim-markdown-toc -->

## Итераторы. Как мы жили до C++20?
### Пара итераторов
+ Посмотреть на простые примеры с парой итераторов, освежить в голове.
+ Идея пары итераторов в свое время стала прорывной, но с тех пор утекло много воды, стало понятно, что можно лучше
+ В частности, `#define all(v) v.begin(), v.end()` у олимпиадников
+ https://github.com/ydb-platform/ydb/blob/main/util/generic/algorithm.h#L47

### Split
+ Как написать split в C++? Какая у него сигнатура?
  1. `std::vector<std::string_view> split(std::string_view str, std::string_view delim);` – аллокации, не ленивый, delim хочется уметь делть функцией / одним байтиком
  2. `SplitRange<std::string_view> split(std::string_view str, std::string_view delim);` – что есть `SplitRange?` Что делают begin / end на нем, как собрать в `vector`?
  3. Развиваем SplitRange: `auto StringSplitter(std::string_view str);` --- объект, у которого много разных настроек про поведение `Split`, поддержаны все сценарии, нет аллокаций, когда не надо; итератор ленивый. Читаем `util/string/split.h` в ydb-platform/ydb: https://github.com/ydb-platform/ydb/blob/main/util/string/split.h#L460.

  Думаем про сценарий `for (auto token : StringSplitter(words).Split(' '))`. Что делает end() у этого ренжа?
  **Кажется, что end() не зависит от исходной строчки, все состояние итератора хранит результат begin().**

### Stream iterators
https://en.cppreference.com/w/cpp/iterator/istream_iterator
Странная древняя фича из C++98, важная для понимания ренжей.

Стоит разобрать каждую строчку здесь:
```cpp
// Python: print(list(map(lambda x: int(x) * 2, sys.stdin)))
// C++: Hold my beer

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
 
int main() {
    std::vector<int> ints;
    std::copy(std::istream_iterator<int>{std::cin}, std::istream_iterator<int>(), std::back_inserter(ints));
    std::transform(ints.begin(), ints.end(), ints.begin(), [](int x) { return x * 2; });
    std::copy(ints.begin(), ints.end(), std::ostream_iterator<int>(std::cout, " "));
}
```

## Ranges
Новая фича из С++20.
Полностью реализуема без поддержки компилятором: фича целиком про новый код в стандартной библиотеке, нет новых языковых штук (как ключевые слова в корутинах, например). До C++20 была большая качественная библиотека от автора ranges (Eric Niebler): [range-v3](https://github.com/ericniebler/range-v3), поддерживает C++14 и выше; пропозал по добавлению ranges в стандартную библиотеку основан на range-v3.
Много важных идей, рассмотрим ключевые.

Ranges в каком-то смысле переделка итераторов и алгоритмов из `<algorithm>`, теперь каждому алгоритму есть сосед в ranges.

### Range
То, что раньше было парой итераторов. Range можно сделать из пары итераторов, можно собрать из итератора и сингулярного значения --- end. Например:
```cpp
struct StreamEofSentinel {};

class StreamIterator {
public:
    const T& operator*() const {
        return value_;
    }

    void operator++() {
        stream_.Read(&value_);
    }

    bool operator==(const StreamEofSentinel& /* sentinel */) const {
        return stream_.Finished();
    }

private:
    InputStream* stream_;
    T value_;
};
```
В общем смысле range – последовательность элементов одного типа:

```cpp
// https://en.cppreference.com/w/cpp/ranges/range
template <class T>
concept range = requires(T& t) {
  ranges::begin(t); // equality-preserving for forward iterators
  ranges::end(t);
};
```

### Range algorithms
Переписывание `<algorithm>` на ренжи.
Сильно удобнее использовать.

Например, сигнатура `std::ranges::sort:`
```cpp
template <
    ranges::random_access_range R,
    class Comp = ranges::less,
    class Proj = std::identity
>
requires std::sortable<ranges::iterator_t<R>, Comp, Proj>
constexpr ranges::borrowed_iterator_t<R> sort(R&& r, Comp comp = {}, Proj proj = {});
```

Тут много новых сложных вещей, стоит почитать ее внимательно.
Из интересного можно выделить две детали:
#### Проекции
В Ranges используется удобная сущность _проекции_ – функция или похожий объект, применяемая ко всем элементам range перед использованием.
Например, это заметно упрощает частую задачу "отсортировать range по ключу":

https://godbolt.org/z/KP37PcMKW

Пусть есть
```cpp
namespace ranges = std::ranges;

struct Person {
    std::string name;
    int age;
};

std::vector<Person> persons{{"Alice", 20}, {"Bill", 48}, {"Ben", -1}};
```

Хотим отсортировать по age.

##### Темные времена. C++17, STL
```cpp
std::sort(persons.begin(), persons.end(), [](auto&& lhs, auto&& rhs) {
    return lhs.age < rhs.age;
});
```

##### Времена получше. Before C++20, Yandex
```cpp
SortBy(persons, [](auto&& person) {
    return person.age;
});
```

##### С++20. Ranges
```cpp
ranges::sort(persons, {}, &Person::age);
```

#### Borrowed range
`ranges::sort` возвращает `ranges::borrowed_iterator_t<R>`.
Что это за зверь? Это ссылка на тот же range, который был отсортирован; возвращаемое значение введено для удобства комбинирования.
Кажется, легко можно получить UB сейчас:
```cpp
std::vector<int> GetNebolshoiMassivchik();

for (int i : std::ranges::sort(GetNebolshoiMassivchik())) {
    // BOOM
}
```

Как быть? Во-первых, для починки в C++20 ввели новую форму for:
```cpp
std::vector<int> GetNebolshoiMassivchik();

for (auto range = std::ranges::sort(GetNebolshoiMassivchik()); auto i : range) {
    // NOT BOOM
}
```

А во-вторых, на самом деле, этот код просто не скомпилируется!
```cpp
std::vector<int> GetNebolshoiMassivchik();

for (int i : std::ranges::sort(GetNebolshoiMassivchik())) {
    // NOT BOOM, COMPILATION ERROR
}
```

Почему? Потому что ranges магией поняли, что нельзя отдавать ссылку на временный объект наружу, и вернули вместо этого `dangling range`:
https://godbolt.org/z/1P43Wr9Wa. Borrow checker в плюсах!

### Views
Легковесная обертка над Range. Например, можно отфильтровать Range:
```cpp
#define IS_ODD_USE_AVX_RUNTIME_DISPATCHING
#include <contrib/libs/is_odd/include/is_odd.h>

int ints[] {1, 5, 6, 1, 4};
for (auto&& oddInt : std::views::filter(ints, &isodd::v4::isodd)) {
    std::cout << oddInt << std::endl;
}
// 1 5 1
```

Важно, что обычно View занимает совсем немного памяти и легко копируется (не всегда, но это совсем детали): `std::vector<int>` не view, но range. Чтоб сделать view на вектор, берем `std::views::ref_view(vec)`.

### Range adaptor objects
Какие view можно придумать?
Их много в стандартной библиотеке, можно делать свои.
В стандартной библиотеке есть набор adaptors над range, которые делают view с разными свойствами:
Например,
1. `std::views::drop(range, count)` – пропустить первые `count` элементов
1. `std::views::filter(range, predicate)` – отфильтровать по предикату
1. `std::views::transform(range, func)` – сделать view из результатов применения `func` к каждому элементу `range`. Важно, что ranges ленивые: применение будет происходить в самый последний момент, когда это можно сделать. При этом, лень там странная: обычно под ленивостью понимают запоминание результатов, здесь его нет, все вычисляется каждый раз заново (почти все, O(1) стейта удобно запомнить):

    ```cpp
    // https://godbolt.org/z/17jcMTEsE

    int ints[] {1, 5, 6, 1, 4};

    int numCalls = 0;
    auto filtered = std::views::filter(ints, [&](int x) {
        ++numCalls;
        return x % 2 == 1;
    });

    std::cout << "numCalls: " << numCalls << std::endl; // Ok, 0
    for (auto&& oddInt : filtered) {
        std::cout << oddInt << std::endl;
    }
    std::cout << "numCalls: " << numCalls << std::endl; // Ok, 5
    for (auto&& oddInt : filtered) {
        std::cout << oddInt << std::endl;
    }
    std::cout << "numCalls: " << numCalls << std::endl; // 9?!
    ```
1. `std::views::reverse(range)` – перевернуть range. Тут, конечно, подходит не любой range, а минимум bidirectional (есть разные категории ренжей, по аналогии с категориями итераторов).
1. `std::views::zip(range1, range2)` – view по парам элементов из первого и второго range. В C++20 не успели принять, будет только в С++23.
    ```cpp
    auto first = {1, 2, 3}; // initializer_list<int> is a range
    auto second = {4, 5, 6};
    for (auto&& [lhs, rhs] : std::views::zip(first, second)) {
        std::cout << lhs << ' ' << rhs << '\n';
    }
    ```
1. `std::views::split(range, delimiter)`. По виду простой view, на практике породил кучу боли и часы споров. Первая принятая версия использовалась вот так:
    ```cpp
    auto view = str
        | std::views::split(' ')
        | std::views::transform([](auto&& rng) {
            return std::string_view{&*rng.begin(), static_cast<size_t>(std::ranges::distance(rng))};
        });
    std::vector<std::string_view> vec{view.begin(), view.end()};
    ```

    Эту проблему полечили уже после релиза C++20, пришлось замержить в стандарт исправление (механизм под названием Defect Reports). https://en.cppreference.com/w/cpp/ranges/split_view#Notes

    Из забавного, такой код тоже не работает правильно:
    ```cpp
    for (auto word : std::views::split("Hello/C++/20/!", "/")) {
        std::cout << std::string_view(word.begin(), word.end()) << ' '; // Prints "Hello/C++/20/!"
    }
    ```

    Надо явно обернуть delimiter в `string_view`:
    ```cpp
    using namespace std::string_view_literals;
    for (auto word : std::views::split("Hello/C++/20/!", "/"sv)) {
        std::cout << std::string_view(word.begin(), word.end()) << ' ';
    }
    ```

    [Ссылочка поиграться](https://godbolt.org/z/hd51nPjP9)

### Комбинирование range adaptor objects
При комбинировании range adaptor objects возникает понятная проблема: код становится сложно читаемым, так как выходит много вложенных функций:
```cpp
namespace views = std::views;
auto tokens = views::reverse(views::take(views::split(str, ' '), 3));
```

Для решения есть красивый синтаксический сахар – переопределенный `operator|`:
```cpp
auto tokens = str | views::split(' ') | views::take(3) | views::reverse();
```

Правила простые: `adaptor(range, args...)` можно записать как `range | adaptor(args...)`.
