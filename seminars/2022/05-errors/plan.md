[video](https://www.youtube.com/watch?v=_Ivd3qzgT7U)

1. **Catch**
  Поговорить про то что порядок отлова исключений важен из-за неявных кастов и наследований исключений [order_matters.cpp](order_matters.cpp)
2. **Stack unwinding** (запускать с -fsanitize=leak)
  * Исключения при использовании new-delete неудобно обрабатывать [new_and_unwinding.cpp](new_and_unwinding.cpp)
  * Еще один пример с new-delete и исключениями [destructors_and_catch.cpp](destructors_and_catch.cpp)
  * Повторное прокидывание той же самой ошибки в текущем try-catch блоке [rethrow_multi_catch.cpp](rethrow_multi_catch.cpp)
  * Порядок вызова дескрипторов и раструтка стека [long_exception.cpp](long_exception.cpp)

3. **noexcept**
  * важность ключевого слова **noexcept** при вызове resize у вектора (приоритеты) [move_noexcept.cpp](move_noexcept.cpp)
  * [**move_if_noexcept**](https://en.cppreference.com/w/cpp/utility/move_if_noexcept)
  * компилятор не рассчитывает на throw из **noexcept** и может соптимизировать код исходя из этого [lier.cpp](lier.cpp)
  * **noexcept(std::is_pod<T>)** и тд. [Оператор noexcept](https://en.cppreference.com/w/cpp/language/noexcept) и noexcept в зависимости от значения expression ([спецификатор noexcept](https://en.cppreference.com/w/cpp/language/noexcept_spec))

4. **Exception safety**
* Nothrow exception guarantee, e.g. destructor by default. Никогда не кидайте исключения из деструктора: [bad_destructor.cpp](bad_destructor.cpp)
* Strong exception guarantee --- state just before the function call (on exception), e.g. vector::push_back
* Basic exception guarantee --- valid state [throw_constructor.cpp](throw_constructor.cpp)
5. **Performance** [performance.cpp](performance.cpp)
```
perf 0  0.18s user 0.00s system 98% cpu 0.183 total
perf 1  15.58s user 0.00s system 99% cpu 15.587 total
```
Very simple try-catch block to [asm](https://godbolt.org/z/4f93nMcaP)

6. **Throw by value, catch by reference** [catch_reference_of_value.cpp](catch_reference_of_value.cpp), [some text](http://ptgmedia.pearsoncmg.com/images/0321113586/items/sutter_item73.pdf)
7. [Проблемы исключений](https://gitlab.com/danlark/cpp-advanced-hse/-/blob/main/lectures/2022/05-errors/lecture.md#проблемы-исключений)
8. [Другие способы работы с ошибками](https://gitlab.com/danlark/cpp-advanced-hse/-/blob/main/lectures/2022/05-errors/lecture.md#типы-суммы)
9. [Ошибки в других ЯП](https://gitlab.com/danlark/cpp-advanced-hse/-/blob/main/lectures/2022/05-errors/lecture.md#c-experience)

_optional_

10. Exceptions: where they live (heap or some memory for a small amount of std::bad_alloc)
11. Don't use **setjmp** and **longjmp** with С++
12. [**exception** header](https://en.cppreference.com/w/cpp/header/exception)
* rethrow_exception
* current_exception
* exception_ptr
* uncaught_exceptions [cppreference example](https://en.cppreference.com/w/cpp/error/uncaught_exception), seems to be enough
* terminate handler [cppreference](https://en.cppreference.com/w/cpp/error/terminate_handler)
  * set_terminate instead of std::abort, [cppreference](https://en.cppreference.com/w/cpp/error/set_terminate), [ibm](https://www.ibm.com/docs/en/zos/2.3.0?topic=only-terminate-function-c)
13. might be interesting [link](https://stackoverflow.com/questions/307610/how-do-exceptions-work-behind-the-scenes-in-c#307716)
14. [The true cost of zero-cost exceptions](https://mortoray.com/2013/09/12/the-true-cost-of-zero-cost-exceptions/)
